#include "stdafx.h"
#include "MQTTCommunicator.h"
#include "dbg.h"

#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <time.h>


using namespace std;

string machTopics = "/mach3/dro/+";
string deviceTopics = "/dev/dro/+";

/**
 *
 */
MQTTCommunicator::MQTTCommunicator(CString& config) {
    mqttServerAddress = config;
    setupClient(config);
}

/**
 *
 */
MQTTCommunicator::~MQTTCommunicator() {
    disconnect();
}

/**
 *
 */
void MQTTCommunicator::connect() {
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.keepAliveInterval = 60;
    conn_opts.cleansession = 1;
    conn_opts.retryInterval = 0;
    conn_opts.connectTimeout = 5;

    CString errorMsg = "Failed to connect to MQTT server ";
    errorMsg.Append(mqttServerAddress);

    while (MQTTClient_connect(mqttClient, &conn_opts) != MQTTCLIENT_SUCCESS) {
        int rc = MessageBox(NULL, errorMsg, "Ooops", MB_ABORTRETRYIGNORE | MB_ICONWARNING | MB_TASKMODAL);
        if (rc == IDABORT) {
            DbgMsg(("User decided to quit"));
            exit(1);
        } else if (rc == IDIGNORE) {
            DbgMsg(("User ignored"));
            return;
        }

    }

    DbgMsg(("Connected!"));
    //ShowMachMessage(CString("Connected"));
}

/**
*
*/
void MQTTCommunicator::disconnect() {
    MQTTClient_disconnect(mqttClient, 10000);
    MQTTClient_destroy(&mqttClient);
}

/**
*
*/
void MQTTCommunicator::subscribe() {
    MQTTClient_subscribe(mqttClient, deviceTopics.c_str(), 1);
}

/**
*
*/
void MQTTCommunicator::externalDroChanged(int dro, double value) {
    //this->setDro((short)dro, value);
    SetDRO(dro + 800, value);
}

/**
*
*/
void MQTTCommunicator::machDroChanged(int dro, double value) {
    MQTTClient_message msg = MQTTClient_message_initializer;
    msg.qos = 2;
    msg.retained = 0;

    string topic = machTopics;
    stringstream ss;
    ss << dro;
    topic.replace(topic.length() - 1, 1, ss.str());

    ss.str("");
    ss << value;

    string sval = ss.str();

    msg.payload = (void*) sval.c_str();
    msg.payloadlen = sval.length();

    DbgMsg(("Sending %f to %s", value, topic.c_str()));

    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(mqttClient, topic.c_str(), &msg, &token);
    int rc = MQTTClient_waitForCompletion(mqttClient, token, 60000);
}

/**
*
*/
int onMessage(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    string data((char*) message->payload, message->payloadlen);

    DbgMsg(("Message arrived on %s with value of %s", topicName, data.c_str()));

    string topic(topicName);
    int lastSlash = topic.rfind('/');
    int dro;
    if (string::npos == lastSlash) {
        // ignore message
    } else {
        string droString = topic.substr(lastSlash + 1);
        dro = stoi(droString);
    }
    double value = stod(data);

    ((MQTTCommunicator*)context)->externalDroChanged(dro, value);

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

    return 1;
}

/**
*
*/
void onConnectionLost(void* context, char* cause) {
    DbgMsg(("Connection lost!")); // TODO: use mach3 message
    ((MQTTCommunicator*)context)->connect();
}

/**
*
*/
void MQTTCommunicator::setupClient(CString& config) {
    stringstream clientNameStr;
    clientNameStr << "Mach3-" << rand();
    MQTTClient_create(&mqttClient, config, clientNameStr.str().c_str(), MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(mqttClient, this, onConnectionLost, onMessage, NULL);
}