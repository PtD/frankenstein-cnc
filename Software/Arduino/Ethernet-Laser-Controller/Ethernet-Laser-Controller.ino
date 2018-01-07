#include <PinChangeInt.h>
#include <Ethernet.h>

#define DEBUG 0

#define PIN_X 2
#define PIN_Y 3
#define PIN_Z 4

#define PIN_DIR_X 6
#define PIN_DIR_Y 7
#define PIN_DIR_Z 8

#define PIN_LASER_TRIGGER 9

#define ON 255
#define OFF 0

// 0.1 mm , if steps per mm are 400
// #define TRANSMIT_THRESHOLD 10
#define TRANSMIT_THRESHOLD 1

#define INIT_PWM_DISTANCE 50

#define MAC_ADDRESS 0x39,0xE7,0xE4,0xBA,0x69,0x96 
//#define 0x7F,0x01,0x02,0x03,0x04,0x66
#define IP_ADDRESS 10,9,9,101
//#define IP_ADDRESS 192,168,13,133
#define SERVER_IP_ADDRESS 10,9,9,2

volatile long countX = 0;
volatile long countY = 0;
volatile long countZ = 0;

volatile boolean backwardX;
volatile boolean backwardY;
volatile boolean backwardZ;

volatile boolean backwardZChanged = false;

long lastTransmittedX = 0;
long lastTransmittedY = 0;
long lastTransmittedZ = 0;

long whereLaserChangedX = 0;
long whereLaserChangedY = 0;

unsigned char laserPwmValue = 0;
unsigned char initLaserPwmValue = 0;
unsigned char writtenLaserPwmValue = 0;

boolean forceLaser = false;

boolean reportXYZ = false;

unsigned long dump = millis();

const IPAddress ipAddress(IP_ADDRESS);
const IPAddress serverIpAddress(SERVER_IP_ADDRESS);
const uint8_t macAddress[6] = { MAC_ADDRESS };
EthernetServer server(4004);

void forceUpdateLaserState(unsigned char pwm, unsigned char initPwm = 0);
void conditionalUpdateLaserState(unsigned char pwm, unsigned char initPwm = 0);

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


void loop() {
    EthernetClient client = server.available();
    if (client) {
        while (client.connected()) {
            long deltaX = countX - lastTransmittedX;
            long deltaY = countY - lastTransmittedY;
            long deltaZ = countZ - lastTransmittedZ;

            if (abs(countX - whereLaserChangedX) > INIT_PWM_DISTANCE || abs(countY - whereLaserChangedY) > INIT_PWM_DISTANCE) {
                // take care to swap the initial PWM value for the regular one after some axis moved
                if (
                    initLaserPwmValue != laserPwmValue
                    && writtenLaserPwmValue != OFF
                    && writtenLaserPwmValue != laserPwmValue
                 ) {
                    setLaserPwm(laserPwmValue);
                }
            }

            if (reportXYZ && 
                (abs(deltaX) > TRANSMIT_THRESHOLD || abs(deltaY) > TRANSMIT_THRESHOLD || abs(deltaZ) > TRANSMIT_THRESHOLD)
             ) {
                writeXYZ(client);
            }

            while (client.available() > 0) {
                int cmd = client.read();
                switch (cmd) {
                    case 'z':
                        resetCoordinatesAndStopLaser();
                        break;

                    case '1':
                        forceUpdateLaserState(ON);
                        break;

                    case '0':
                        forceUpdateLaserState(OFF);
                        break;

                    case '=':
                    {
                        String s = client.readStringUntil('\n');
                        int initIx = s.indexOf(':');
                        if (initIx > -1 && initIx < s.length() - 1) {
                            String pwm = s.substring(0, initIx);
                            String initPwm = s.substring(initIx + 1);
                            Serial.print(pwm); Serial.print(":"); Serial.println(initPwm);
                            forceUpdateLaserState((uint8_t) pwm.toInt(), (uint8_t) initPwm.toInt());
                        } else {
                            forceUpdateLaserState((uint8_t) s.toInt());
                        }
                        break;
                    }

                    case 'p':
                    {
                        // wait until there's something in the buffer
                        while (client.available() <= 0); // do nothing
                        int val = client.read();
                        conditionalUpdateLaserState(val);
                        break;
                    }

                    case 'd':
                        writeXYZ(client);
                        break;

                    case 'a':
                        conditionalUpdateLaserState(255);
                        break;

                    case 'A':
                    {
                        String s = client.readStringUntil('\n');
                        int initIx = s.indexOf(':');
                        if (initIx > -1 && initIx < s.length() - 1) {
                            String pwm = s.substring(0, initIx);
                            String initPwm = s.substring(initIx + 1);
                            Serial.print(pwm); Serial.print(":"); Serial.println(initPwm);
                            conditionalUpdateLaserState((uint8_t) pwm.toInt(), (uint8_t) initPwm.toInt());
                        } else {
                            conditionalUpdateLaserState((uint8_t) s.toInt());
                        }
                        break;
                    }

                    case 'n':
                        reportXYZ = false;
                        break;

                    case 't':
                        reportXYZ = true;
                        break;
                }
            }

            if (backwardZChanged) {
                backwardZChanged = false;
                updateLaserOnBackwardZChange();
            }

            if (millis() - dump > 1000) {
                dumpXYZ();
                dump = millis();
            }

        }
    } else {
        // no client connected
        forceUpdateLaserState(OFF);
        delay(1000);
    }

}

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

void forceUpdateLaserState(unsigned char pwm, unsigned char initPwm = 0) {
    forceLaser = true;
    laserPwmValue = pwm;
    if (initPwm == 0)
        initLaserPwmValue = pwm;
    else
        initLaserPwmValue = initPwm;

    updateLaserLevelChangeXY();
    setLaserPwm(initLaserPwmValue);
}

void conditionalUpdateLaserState(unsigned char pwm, unsigned char initPwm = 0) {
    forceLaser = false;
    laserPwmValue = pwm;
    if (initPwm == 0)
        initLaserPwmValue = pwm;
    else
        initLaserPwmValue = initPwm;

    if (!backwardZ) {
        updateLaserLevelChangeXY();
        setLaserPwm(initLaserPwmValue);
    } else {
        setLaserPwm(OFF);
    }

}

void updateLaserOnBackwardZChange() {
    if (forceLaser)
        return;

    if (!backwardZ) {
        updateLaserLevelChangeXY();
        setLaserPwm(initLaserPwmValue);
    } else {
        setLaserPwm(OFF);
    }
}

void setLaserPwm(unsigned char value) {
#ifdef DEBUG
    Serial.print("PWM:"); Serial.println((int) 255 - value);
#endif
    analogWrite(PIN_LASER_TRIGGER, 255 - value);
    writtenLaserPwmValue = value;
}

void updateLaserLevelChangeXY() {
    whereLaserChangedX = countX;
    whereLaserChangedY = countY;
}


void setupEthernet() {
    Ethernet.begin(macAddress, ipAddress);

#ifdef DEBUG
    Serial.print(F("Local IP:")); Serial.println(Ethernet.localIP());
    Serial.print(F("Gateway IP:")); Serial.println(Ethernet.gatewayIP());
#endif // DEBUG

    server.begin();
}

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

void setupLaser() {
    pinMode(PIN_LASER_TRIGGER, OUTPUT);
    forceLaser = true;
    forceUpdateLaserState(OFF);
}

void countImpulsesX() {
    if (backwardX)
        countX--;
    else
        countX++;
}

void countImpulsesY() {
    if (backwardY)
        countY--;
    else
        countY++;
}

void countImpulsesZ() {
    if (backwardZ)
        countZ++;
    else
        countZ--;
}

void directionChangedX() {
    backwardX = !digitalRead(PIN_DIR_X);
}

void directionChangedY() {
    backwardY = !digitalRead(PIN_DIR_Y);
}

void directionChangedZ() {
    boolean newValue = !digitalRead(PIN_DIR_Z);
    if (newValue != backwardZ)
        backwardZChanged = true;
    backwardZ = newValue;
}

void resetCoordinatesAndStopLaser() {
    countX = 0;
    countY = 0;
    countZ = 0;
    forceUpdateLaserState(OFF);
}

void dumpXYZ() {
#ifdef DEBUG
    Serial.print(F("X:")); Serial.print(countX);
    Serial.print(F(",Y:")); Serial.print(countY);
    Serial.print(F(",Z:")); Serial.println(countZ);
#endif
}
