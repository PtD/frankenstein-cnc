#include <SPI.h>
#include <UIPEthernet.h>
#include <PubSubClient.h>
#include <PID_v1.h>
#include <EEPROM.h>
#include <avr/wdt.h>

// #undef DEBUG
#define DEBUG 1

// *************************************************************
// NON Configurable stuff
// *************************************************************
#define ETHERNET_CS 10
#define HEATER_OUT  3
#define FAN_OUT     5

#define THERMISTOR_IN A3

#define TOPIC_LENGTH 32
#define VALUE_LENGTH 32
#define VALUES 16

// these are inputs
#define DRO_SET_TEMP 1010
#define DRO_SET_FAN 1020
// these are outputs
#define DRO_FEEDBACK_CURRENT_TEMP 1011
#define DRO_FEEDBACK_TEMP_REACHED 1012
#define DRO_FEEDBACK_HEATER_ON 1013
#define DRO_FEEDBACK_PID_OUTPUT 1014
#define DRO_QUERY_FAN 1021

// how often do we publish current T to subscribers, millis
#define TRANSMIT_TEMP_PERIOD 1000
// how often do we publish current PID value to subscribers, millis
#define TRANSMIT_PID_OUTPUT_PERIOD 1000


// serial comm speed
#define SERIAL_BAUD_RATE 115200

// until abs(temperature - target) >= this value, we'll not employ PID
#define PID_BYPASS_THRESHOLD 7.5
// what's one unit of control, in millis
#define HEAT_QUANT 500

// send READY signal if abs(temperature - target) is within this value
#define EPSILON_TEMP_REACHED 2
// what's the nominal resistence of the thermistor at nominal T
#define THERMISTOR_NOMINAL 100000    
// what's the nominal T of the thermistor
#define TEMPERATURE_NOMINAL 25   
// how many measurements to take for T value
#define THERMISTOR_SAMPLES 5
// B-coefficient of the thermistor
#define THERMISTOR_BCOEFFICIENT 3950
// the resistor value of the voltage divider
#define SERIESRESISTOR 995


// *************************************************************
// End of NON Configurable stuff
// *************************************************************


// *************************************************************
// Configurable stuff
// *************************************************************
#define MAC_ADDRESS 0x7F,0x01,0x02,0x03,0x04, 0x64
#define IP_ADDRESS 10,9,9,100
#define SERVER_IP_ADDRESS 10,9,9,2

// *************************************************************
// End of configurable stuff
// *************************************************************

const IPAddress ipAddress(IP_ADDRESS);
const IPAddress serverIpAddress(SERVER_IP_ADDRESS);
const uint8_t macAddress[6] = { MAC_ADDRESS };


// parameters of the PID controller
// const double Kp = 60.0, Ki = 0.45, Kd = 0.0; -> good
// const double Kp = 0.5 * 100.0, Ki = 40.0 / 40.0, Kd = 40.0 / 2.0; -> not bad

// !!!!!!!!!!!!!!!! VERY GOOD !!!!!!!!!!!!!!!
// const double Kp = 50.0, Ki = 1.0, Kd = 50.0;

// ++++++++++++++++ EVEN BETTER - SMALL OVERSHOOT +++++++++++++++
const double Kp = 13.0, Ki = 0.35, Kd = 100.0;


void executeRarely(void(*)(), unsigned long* , unsigned long );

bool fanIsOn = false;
double setTemperature = 0;
unsigned int startFromTemperature = 0;
unsigned int temperatureReached = 0;
unsigned long heatQuantStart = millis();
unsigned long heatQuantEnd = heatQuantStart + HEAT_QUANT;
unsigned long turnOffAt = 0;
double currentTemperature = 0.0;

// these 
unsigned long lastTemperatureMeasure = 0;
unsigned long lastTemperatureMeasureTransmission = 0;
unsigned long lastPidOutputTransmission = 0;

typedef void (*Runnable)();

const char* SUBSCRIPTION_TOPICS = "/mach3/dro/+";

EthernetClient ethernetClient;
PubSubClient mqttClient(ethernetClient);
double pidOutput;
PID pid(&currentTemperature, &pidOutput, &setTemperature, Kp, Ki, Kd, DIRECT);



// -----------------------------------------------------------
//
// -----------------------------------------------------------
void loop() {
    wdt_reset();

	controlTemperature();
	loopMqtt();

    executeRarely(
        []() {notifySlaveValueChange(DRO_FEEDBACK_CURRENT_TEMP, currentTemperature); },
        &lastTemperatureMeasureTransmission,
        TRANSMIT_TEMP_PERIOD
    );
}

// -----------------------------------------------------------
// Callback: Called when incoming MQTT message is received about Mach3 DRO change
// -----------------------------------------------------------
void onMasterValueChange(short index, float value) {
    switch (index) {
        case DRO_SET_TEMP:
            setTargetTemperature(value);
            break;
        case DRO_SET_FAN:
            // fan on/off
#ifdef DEBUG
            Serial.print(F("Seting fan PWM to ")); Serial.println((int)value);
#endif // DEBUG
            analogWrite(FAN_OUT, (int)value);
            break;
    }
}

// -----------------------------------------------------------
// Handles PID control of temperature
// -----------------------------------------------------------
void controlTemperature() {
    calculateTemp();

    if (setTemperature == 0) {
        heat(false);
		if (pid.GetMode() == AUTOMATIC) 
            pid.SetMode(MANUAL);
        return;
    }

    if (
        setTemperature &&  !temperatureReached && (
            // we've been asked to increase T
            (startFromTemperature <= setTemperature && currentTemperature + EPSILON_TEMP_REACHED >= setTemperature)
            ||
            // we've been asked to decrease T
            (startFromTemperature >= setTemperature && currentTemperature - EPSILON_TEMP_REACHED <= setTemperature)
        )
    ) {
        temperatureReached = 1;
        notifySlaveValueChange(DRO_FEEDBACK_TEMP_REACHED, 1.0);
    }


    // from this point on we have a set temperature

    // bypass PID if T is less than <threshold> degrees below the target one
    if (abs(setTemperature - currentTemperature) > PID_BYPASS_THRESHOLD) {
        pid.SetMode(MANUAL);
        if (setTemperature > currentTemperature) {
            heat(true);
        } else {
            heat(false);
        }
        return;
    }

    if (pid.GetMode() == MANUAL) {
        pid.SetMode(AUTOMATIC);
    }

    unsigned long now = millis();
    boolean quantChanged = false;

    if (!(now >= heatQuantStart && now <= heatQuantEnd)) {
        heatQuantStart = millis();
        heatQuantEnd = heatQuantStart + HEAT_QUANT;
        quantChanged = true;
    }


    pid.Compute();

    if (quantChanged)
        turnOffAt = heatQuantStart + pidOutput;

    if (now <= turnOffAt) {
        heat(true);
    } else {
        heat(false);
    }

#ifdef DEBUG
    executeRarely(
        []() { notifySlaveValueChange(DRO_FEEDBACK_PID_OUTPUT, pidOutput); },
        &lastPidOutputTransmission,
        TRANSMIT_PID_OUTPUT_PERIOD
    );
#endif // DEBUG
}

void setTargetTemperature(float value) {
#ifdef DEBUG
    Serial.print(F("Changing T to ")); Serial.println(value);
#endif // DEBUG
    // change of set temperature
    setTemperature = (double)value;
    startFromTemperature = (unsigned int)currentTemperature;
    temperatureReached = 0;
    notifySlaveValueChange(DRO_FEEDBACK_TEMP_REACHED, 0.0);
}



// -----------------------------------------------------------
//
// -----------------------------------------------------------
void setup() {
    wdt_disable();
#ifdef DEBUG
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println(F("Started!"));
#endif // DEBUG
    pinMode(ETHERNET_CS, OUTPUT);
    pinMode(THERMISTOR_IN, INPUT);
    pinMode(HEATER_OUT, OUTPUT);
    pinMode(FAN_OUT, OUTPUT);

    digitalWrite(ETHERNET_CS, HIGH);
    digitalWrite(HEATER_OUT, LOW);
    digitalWrite(FAN_OUT, LOW);

    // initial T' fill
    calculateTemp();
    

    setupMqtt();
    setupPid();
	setupWatchdog();

    recoverFromWatchdogRestart();
}

// -----------------------------------------------------------
// Call this when you need to notify Mach3 about a change in parameters
// -----------------------------------------------------------
void notifySlaveValueChange(short index, float value) {
    char topic[TOPIC_LENGTH];
    sprintf_P(topic, PSTR("/dev/dro/%d"), index);
    char val[VALUE_LENGTH];
    ftoa(val, value);
#ifdef DEBUG
    // Serial.println(value);
    Serial.print(F(">> ")); Serial.print(val); Serial.print(F(" to ")); Serial.println(topic);
#endif
    mqttClient.publish(topic, val);
}

// -----------------------------------------------------------
// Call this when you need to notify Mach3 about a change in parameters
// -----------------------------------------------------------
void executeRarely(void (*action)(), unsigned long* timestamp, unsigned long period) {
    if (millis() - *timestamp > period) {
        action();
        *timestamp = millis();
    }
}

// -----------------------------------------------------------
// Callback: handles incoming MQTT messages and processes them
// -----------------------------------------------------------
void mqttCallback(char* topic, uint8_t* data, unsigned int length) {
    char* lastSlash = strrchr(topic, '/');
    if (!lastSlash || lastSlash == topic + length - 1) return;

    char buff[VALUE_LENGTH];
    memset(buff, 0, VALUE_LENGTH);
    memcpy(buff, data, (length > VALUE_LENGTH - 1 ? VALUE_LENGTH - 1 : length));

    short index = (short) atoi(lastSlash + 1);
    float value = atof((char*) buff);

#ifdef DEBUG
    Serial.print(F("Message->"));
    Serial.print(F("[Index:")); Serial.print(index);
    Serial.print(F("][Value:")); Serial.print(value); Serial.println(F("]"));
#endif // DEBUG

    onMasterValueChange(index, value);
}


// -----------------------------------------------------------
//
// -----------------------------------------------------------
void setupMqtt() {
    Ethernet.begin(macAddress, ipAddress);

#ifdef DEBUG
    Serial.print(F("Local IP:")); Serial.println(Ethernet.localIP());
    Serial.print(F("Gateway IP:")); Serial.println(Ethernet.gatewayIP());
#endif // DEBUG

    mqttClient.setServer(serverIpAddress, 1883);
    mqttClient.setCallback(mqttCallback);
}

// -----------------------------------------------------------
//
// -----------------------------------------------------------
void setupPid() {
    pid.SetOutputLimits(0, HEAT_QUANT);
    pid.SetSampleTime(HEAT_QUANT);
    pid.SetMode(MANUAL);
}

// -----------------------------------------------------------
// Handle MQTT loop and re-connecting
// -----------------------------------------------------------
void loopMqtt() {
    while (!mqttClient.connected()) {
#ifdef DEBUG
        Serial.println(F("Trying to connect to MQTT server..."));
#endif // DEBUG

		char buffer[32];
        sprintf_P(buffer, PSTR("Arduino-MQTT-%d"), ipAddress[3]);
        if (mqttClient.connect(buffer)) {

#ifdef DEBUG
			Serial.println(F("Connected to MQTT server!"));
#endif // DEBUG

            mqttClient.subscribe(SUBSCRIPTION_TOPICS);
        }
    }

    mqttClient.loop();
}


void calculateTemp() {
    double average = 0.0;
    for (int i = 0; i < THERMISTOR_SAMPLES; i++) {
        average += analogRead(THERMISTOR_IN);
        delay(2);
    }

    average = 1023 - (average / THERMISTOR_SAMPLES);
    //Serial.print("Average: "); Serial.println(average);
    average = 1023 / average - 1;
    // Serial.print("Average 2: "); Serial.println(average);
    average = SERIESRESISTOR / average;
    // Serial.print("Average 3: "); Serial.println(average);

    float steinhart;
    steinhart = average / THERMISTOR_NOMINAL;     // (R/Ro)
    steinhart = log(steinhart);                  // ln(R/Ro)
    steinhart /= THERMISTOR_BCOEFFICIENT;                   // 1/B * ln(R/Ro)
    steinhart += 1.0 / (TEMPERATURE_NOMINAL + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart;                 // Invert
    steinhart -= 273.15;                         // convert to C

    currentTemperature = steinhart;
}

// -----------------------------------------------------------
// Turns heater on and off
// -----------------------------------------------------------
void heat(boolean on) {
    boolean isHigh = digitalRead(HEATER_OUT) == HIGH;
    if (isHigh != on) {
        // temperatureAtHeaterChange = currentTemperature;
#ifdef DEBUG
        // notifySlaveValueChange(DRO_FEEDBACK_HEATER_ON, on ? 1.0 : 0.0);
#endif // DEBUG
	} else {
		return;
	}

    if (on) {
        digitalWrite(HEATER_OUT, HIGH);
    } else {
        digitalWrite(HEATER_OUT, LOW);
    }
}


// -----------------------------------------------------------
// Watchdog timer setup to restart after 8 second if there's no reset called
// -----------------------------------------------------------
void setupWatchdog() {
    cli();
    wdt_reset();
    MCUSR &= ~_BV(WDRF);
    /*
    WDTCSR configuration:
    WDIE = 1: Interrupt Enable
    WDE = 1 :Reset Enable
    WDP3 = 1, WDP2 = 0, WDP1 = 0, WDP0 = 1 -> 8 seconds
    */
    // Enter Watchdog Configuration mode:
    WDTCSR |= (1 << WDCE) | (1 << WDE);
    // Set Watchdog settings:
    WDTCSR = (1 << WDIE) | (1 << WDE) |
        (1 << WDP3) | (0 << WDP2) | (0 << WDP1) | (1 << WDP0);

    sei();
}

void recoverFromWatchdogRestart() {
    if (EEPROM.read(1) == 1) {
        unsigned int temperature = EEPROM.read(2) | EEPROM.read(3) << 8;
        setTargetTemperature(temperature);
        EEPROM.write(1, 0);
#ifdef DEBUG
        Serial.print(F("Recovered from watchdog restart. Setting T=")); Serial.print(temperature);
#endif
    }
}

ISR(WDT_vect) {
    if (setTemperature == 0 || EEPROM.read(1) != 1) return;

    int setTemp = (int) setTemperature;

    EEPROM.write(1, 1);
    EEPROM.write(2, setTemp & 0xFF);
    EEPROM.write(3, setTemp >> 8);
}


// -----------------------------------------------------------
//
// -----------------------------------------------------------
char *ftoa(char *a, double f) {
    char *ret = a;
    long intPart = (long)f;
    itoa(intPart, a, 10);
    while (*a != '\0') a++;
    *a++ = '.';
    unsigned int decimal = abs((unsigned int)((f - intPart) * 100));
    itoa(decimal, a, 10);
    return ret;
}
