#include <PinChangeInt.h>
#include <Ethernet.h>

enum Mode {
    ALWAYS_OFF, ALWAYS_ON, AUTO
};

#define DEBUG 1

#define PIN_X 2
#define PIN_Y 3
#define PIN_Z 4

#define PIN_DIR_X 6
#define PIN_DIR_Y 7
#define PIN_DIR_Z 8

#define PIN_LASER_TRIGGER 9

#define ON 255
#define OFF 0

// How many counts by X or Y to transmit X/Y/Z coordinates
// 1 count is 0.0025mm
#define TRANSMIT_THRESHOLD 1

// How many impulses before switching from initial to nominal PWM
// 1 count is 0.0025 mm
#define INIT_PWM_DISTANCE 100

// how many milliseconds without movement, before laser is shut off
// (prevents excessive burns at beginning/end of trajectory)
#define STATIONARY_HEAD_THRESHOLD 10

#define MAC_ADDRESS 0x39,0xE7,0xE4,0xBA,0x69,0x96 
//#define 0x7F,0x01,0x02,0x03,0x04,0x66
#define IP_ADDRESS 10,9,9,101
//#define IP_ADDRESS 192,168,13,133
#define SERVER_IP_ADDRESS 10,9,9,2

// === stuff related to movement counters ===
volatile long countX = 0;
volatile long countY = 0;
volatile long countZ = 0;

volatile boolean backwardX;
volatile boolean backwardY;
volatile boolean backwardZ;

long lastTransmittedX = 0;
long lastTransmittedY = 0;
long lastTransmittedZ = 0;

// === stuff related to initial pwm application ===
long whereLaserChangedX = 0;
long whereLaserChangedY = 0;

// === stuff related to pwm values ===
unsigned char nominalSetLaserPwmValue = OFF;
unsigned char initialSetLaserPwmValue = OFF;
unsigned char currentSetLaserPwmValue = OFF;
unsigned char actualLaserPwmValue = OFF;
enum Mode mode = ALWAYS_OFF;

// === stuff related to dashed lines ===
uint32_t setDashLength = 0;
long dashX = 0;
long dashY = 0;
boolean dashHigh = false;

// === stuff related to stationary power prevention ===
volatile unsigned long lastMovementStamp = millis();

// === stuff related to X/Y/Z dump ===
boolean reportXYZ = false;
unsigned long lastDump = millis();

// === stuff related to ethernet comm ===
const IPAddress ipAddress(IP_ADDRESS);
const IPAddress serverIpAddress(SERVER_IP_ADDRESS);
uint8_t macAddress[6] = { MAC_ADDRESS };
EthernetServer server(4004);

////////////////////////////////////////////////////////////////////////////////////
// Initialization routines
////////////////////////////////////////////////////////////////////////////////////
void setup() {
#ifdef DEBUG
    Serial.begin(115200);
#endif
    setupCounters();
    setupEthernet();
    setupLaser();
#ifdef DEBUG
    Serial.println("Done!");
#endif
}

////////////////////////////////////////////////////////////////////////////////////
// Main loop
////////////////////////////////////////////////////////////////////////////////////
void loop() {
    EthernetClient client = server.available();

    if (client) {
        while (client.connected()) {
            if (reportXYZ) {
                doReportXYZ(client);
            }

            while (client.available() > 0) {
                handleCommands(client);
            }

            // initiate coordinate dump every second
            if (millis() - lastDump > 1000) {
                dumpXYZ();
                lastDump = millis();
            }

            updateLaser();
        }

        // connection is gone, turn it off
        mode = ALWAYS_OFF;
        updateLaser();
    } else {
        // no client connected, always turn off for safety reasons
        mode = ALWAYS_OFF;
        updateLaser();
        delay(1000);
    }
}

////////////////////////////////////////////////////////////////////////////////////
// Deals with commands sent over the ethernet
////////////////////////////////////////////////////////////////////////////////////
void handleCommands(EthernetClient client) {
    int cmd = client.read();
    switch (cmd) {
        case 'z':
            // X = Y = Z = 0, laser OFF
            countX = 0;
            countY = 0;
            countZ = 0;
            mode = ALWAYS_OFF;
#ifdef DEBUG
            Serial.println("Mode: OFF; x,y,z=0");
#endif
            break;

        case '1':
            // non-conditional laser on MAX power
            mode = ALWAYS_ON;
            nominalSetLaserPwmValue = initialSetLaserPwmValue = ON;
#ifdef DEBUG
            Serial.println("Mode: ALWAYS_ON; Power: MAX");
#endif
            break;

        case '0':
            // non-conditional turn laser off
            mode = ALWAYS_OFF;
#ifdef DEBUG
            Serial.println("Mode: ALWAYS_OFF");
#endif
            break;

        case '=':
        {
            // human readable command - non-conditional laser power
            // =<pwm>:<initial_pwm>\n
            // example:
            // =127:32<newline>
            String s = client.readStringUntil('\n');
            int initIx = s.indexOf(':');
            if (initIx > -1 && initIx < s.length() - 1) {
                String pwm = s.substring(0, initIx);
                String initPwm = s.substring(initIx + 1);
                Serial.print(pwm); Serial.print(":"); Serial.println(initPwm);
                nominalSetLaserPwmValue = (uint8_t) pwm.toInt();
                initialSetLaserPwmValue = (uint8_t) initPwm.toInt();
            } else {
                nominalSetLaserPwmValue = initialSetLaserPwmValue = (uint8_t) s.toInt();
            }
            mode = ALWAYS_ON;
#ifdef DEBUG
            Serial.print("Mode: ALWAYS_ON; Initial PWM: "); Serial.print(initialSetLaserPwmValue);
            Serial.print("; Nominal PWM: "); Serial.println(nominalSetLaserPwmValue);
#endif
            break;
        }

        case 'p':
        {
            // conditional laser power
            // p<byte>
            // example:
            //    p<\xFF>
            // wait until there's something in the buffer
            while (client.available() <= 0); // do nothing
            nominalSetLaserPwmValue = initialSetLaserPwmValue = (uint8_t) client.read();
            mode = AUTO;
#ifdef DEBUG
            Serial.print("Mode: AUTO; Initial/Nominal PWM: "); Serial.println(nominalSetLaserPwmValue);
#endif
            break;
        }

        case 'd':
            // dump X/Y/Z to client
            writeXYZ(client);
            break;

        case 'a':
            // conditional power to MAX
            nominalSetLaserPwmValue = initialSetLaserPwmValue = ON;
            mode = AUTO;
#ifdef DEBUG
            Serial.print("Mode: AUTO; Initial/Nominal PWM: MAX");
#endif
            break;

        case 'A':
        {
            // human readable command
            // A<pwm>:<initial pwm>\n
            // Example:
            //  A127:32<newline>
            String s = client.readStringUntil('\n');
            int initIx = s.indexOf(':');
            if (initIx > -1 && initIx < s.length() - 1) {
                String pwm = s.substring(0, initIx);
                String initPwm = s.substring(initIx + 1);
                Serial.print(pwm); Serial.print(":"); Serial.println(initPwm);
                nominalSetLaserPwmValue = (uint8_t) pwm.toInt();
                initialSetLaserPwmValue = (uint8_t) initPwm.toInt();
            } else {
                nominalSetLaserPwmValue = initialSetLaserPwmValue = (uint8_t) s.toInt();
            }
            mode = AUTO;
#ifdef DEBUG
            Serial.print("Mode: AUTO; Initial PWM: "); Serial.print(initialSetLaserPwmValue);
            Serial.print("; Nominal PWM: "); Serial.println(nominalSetLaserPwmValue);
#endif
            break;
        }

        case 'n':
            // stop reporting on X/Y/Z
            reportXYZ = false;
#ifdef DEBUG
            Serial.println("Don't report xyz");
#endif
            break;

        case 't':
            // start reporting on X/Y/Z
            reportXYZ = true;
#ifdef DEBUG
            Serial.println("Report xyz");
#endif
            break;

        case 'h':
            // dash mode 
            // h<length>\n
            // example
            // h<1234>\n -> sets dash length to 1234 counts
            // the number is how many counts between flipping the laser on/off
            String s = client.readStringUntil('\n');
            setDashLength = s.toInt();
            dashX = countX; dashY = countY;
#ifdef DEBUG
            Serial.print("Dash length: "); Serial.println(setDashLength);
#endif
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////////
// Changes laser power levels
////////////////////////////////////////////////////////////////////////////////////
void updateLaser() {
    calculateSetPower();

    boolean shouldTurnOn = false;
    //Serial.println("Turn on = false");
    switch (mode) {
        case ALWAYS_OFF:
            shouldTurnOn = false;
            break;
        case ALWAYS_ON:
            shouldTurnOn = true;
            break;
        case AUTO:
            shouldTurnOn = !backwardZ;
            break;
        default:
            break;
    }
    //Serial.print("Turn on (1) = "); Serial.println(shouldTurnOn);
    if (shouldTurnOn && mode == AUTO && millis() - lastMovementStamp > STATIONARY_HEAD_THRESHOLD) {
        // turn off based on the time during which head is not moving
        shouldTurnOn = false;
    }
    //Serial.print("Turn on (2) = "); Serial.println(shouldTurnOn);
    if (setDashLength > 0) {
        unsigned long deltaX = abs(countX - dashX);
        unsigned long deltaY = abs(countY - dashY);
        if (deltaX * deltaX + deltaY * deltaY > setDashLength * setDashLength) {
            dashHigh = !dashHigh;
            dashX = countX;
            dashY = countY;
        }
        shouldTurnOn = shouldTurnOn && dashHigh;
    }
    //Serial.print("Turn on (3) = "); Serial.println(shouldTurnOn);

    if (shouldTurnOn) {
        // when switching from OFF to ON, remember where did it happen, so that we can switch up from initial PWM
        if (actualLaserPwmValue == OFF) {
            updateLaserLevelChangeXY();
        }
        // only actually do something if 'set' value is different than already applied one
        if (actualLaserPwmValue != currentSetLaserPwmValue) {
            actualLaserPwmValue = currentSetLaserPwmValue;
            analogWrite(PIN_LASER_TRIGGER, 255 - actualLaserPwmValue);
#ifdef DEBUG
            Serial.print("PWM:"); Serial.println((int) 255 - actualLaserPwmValue);
#endif
        }
    } else {
        if (actualLaserPwmValue != OFF) {
            actualLaserPwmValue = OFF;
            analogWrite(PIN_LASER_TRIGGER, 255 - actualLaserPwmValue);
#ifdef DEBUG
            Serial.print("PWM:"); Serial.println((int) 255 - actualLaserPwmValue);
#endif
        }
    }

    // delay(1000);
}

////////////////////////////////////////////////////////////////////////////////////
// Sets the desired PWM value for the laser
// Handles transition from initial to nominal PWM 
////////////////////////////////////////////////////////////////////////////////////
void calculateSetPower() {
    if (initialSetLaserPwmValue == nominalSetLaserPwmValue) {
        if (currentSetLaserPwmValue != nominalSetLaserPwmValue) {
            currentSetLaserPwmValue = nominalSetLaserPwmValue;
        }
    } else {
        boolean xMoved = abs(countX - whereLaserChangedX);
        boolean yMoved = abs(countY - whereLaserChangedY);

        if (xMoved * xMoved + yMoved * yMoved > INIT_PWM_DISTANCE * INIT_PWM_DISTANCE) {
            currentSetLaserPwmValue = nominalSetLaserPwmValue;
        } else {
            currentSetLaserPwmValue = initialSetLaserPwmValue;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
// dump X/Y/Z to the client
////////////////////////////////////////////////////////////////////////////////////
void writeXYZ(EthernetClient client) {
    client.write((const char*) &countX, 4);
    client.write((const char*) &countY, 4);
    client.write((const char*) &countZ, 4);
    client.write('\n');
    lastTransmittedX = countX;
    lastTransmittedY = countY;
    lastTransmittedZ = countZ;
    dumpXYZ();
}


////////////////////////////////////////////////////////////////////////////////////
// invoked whenever we trigger the laser on, so we can apply initial PWM
////////////////////////////////////////////////////////////////////////////////////
void updateLaserLevelChangeXY() {
    whereLaserChangedX = countX;
    whereLaserChangedY = countY;
}


////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void setupEthernet() {
    Ethernet.begin(macAddress, ipAddress);

#ifdef DEBUG
    Serial.print(F("Local IP:")); Serial.println(Ethernet.localIP());
    Serial.print(F("Gateway IP:")); Serial.println(Ethernet.gatewayIP());
#endif // DEBUG

    server.begin();
}

////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void setupCounters() {
    pinMode(PIN_X, INPUT);
    pinMode(PIN_Y, INPUT);
    pinMode(PIN_Z, INPUT);
    pinMode(PIN_DIR_X, INPUT);
    pinMode(PIN_DIR_Y, INPUT);
    pinMode(PIN_DIR_Z, INPUT);

    attachInterrupt(digitalPinToInterrupt(PIN_X), countImpulsesX, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_Y), countImpulsesY, RISING);
    PCintPort::attachInterrupt(PIN_Z, countImpulsesZ, CHANGE);

    PCintPort::attachInterrupt(PIN_DIR_X, directionChangedX, CHANGE);
    PCintPort::attachInterrupt(PIN_DIR_Y, directionChangedY, CHANGE);
    PCintPort::attachInterrupt(PIN_DIR_Z, directionChangedZ, CHANGE);

    backwardX = !digitalRead(PIN_DIR_X);
    backwardY = !digitalRead(PIN_DIR_Y);
    backwardZ = !digitalRead(PIN_DIR_Z);
}

////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void setupLaser() {
    pinMode(PIN_LASER_TRIGGER, OUTPUT);
    mode = ALWAYS_OFF;
    updateLaser();
}


////////////////////////////////////////////////////////////////////////////////////
// Interrupt function
////////////////////////////////////////////////////////////////////////////////////
void countImpulsesX() {
    if (backwardX)
        countX--;
    else
        countX++;

    lastMovementStamp = millis();
}

////////////////////////////////////////////////////////////////////////////////////
// Interrupt function
////////////////////////////////////////////////////////////////////////////////////
void countImpulsesY() {
    if (backwardY)
        countY--;
    else
        countY++;

    lastMovementStamp = millis();
}

////////////////////////////////////////////////////////////////////////////////////
// Interrupt function
////////////////////////////////////////////////////////////////////////////////////
void countImpulsesZ() {
    if (backwardZ)
        countZ++;
    else
        countZ--;
}

////////////////////////////////////////////////////////////////////////////////////
// Interrupt function
////////////////////////////////////////////////////////////////////////////////////
void directionChangedX() {
    backwardX = !digitalRead(PIN_DIR_X);
}

////////////////////////////////////////////////////////////////////////////////////
// Interrupt function
////////////////////////////////////////////////////////////////////////////////////
void directionChangedY() {
    backwardY = !digitalRead(PIN_DIR_Y);
}

////////////////////////////////////////////////////////////////////////////////////
// Interrupt function
////////////////////////////////////////////////////////////////////////////////////
void directionChangedZ() {
    backwardZ = !digitalRead(PIN_DIR_Z);
}

void doReportXYZ(EthernetClient client) {
    unsigned long deltaX = abs(countX - lastTransmittedX);
    unsigned long deltaY = abs(countY - lastTransmittedY);
    unsigned long deltaZ = abs(countZ - lastTransmittedZ);

    if (deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ > TRANSMIT_THRESHOLD * TRANSMIT_THRESHOLD * TRANSMIT_THRESHOLD) {
        writeXYZ(client);
    }


}

////////////////////////////////////////////////////////////////////////////////////
// Debug function
////////////////////////////////////////////////////////////////////////////////////
void dumpXYZ() {
#ifdef DEBUG
    Serial.print(F("X:")); Serial.print(countX);
    Serial.print(F(",Y:")); Serial.print(countY);
    Serial.print(F(",Z:")); Serial.println(countZ);
#endif
}
