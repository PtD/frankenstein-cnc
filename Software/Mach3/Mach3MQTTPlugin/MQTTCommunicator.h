#include "Mach3MQTT.h"
#include <MQTTClient.h>

#pragma once


class MQTTCommunicator {
private:
    DoubleShort getDro;
    VoidShortDouble setDro;
    BoolShort getLed;
    VoidShortBool	setLed;

    MQTTClient mqttClient;
    CString mqttServerAddress;

public:
    MQTTCommunicator(CString& config);
    ~MQTTCommunicator();
    void connect();
    void subscribe();
    void externalDroChanged(int dro, double value);
    void machDroChanged(int dro, double value);

private:
    void disconnect();
    void setupClient(CString& config);
};

