import paho.mqtt.client as mqtt
import threading


class MqttComm:
    ###################################################
    #
    def __init__(self, ui_hook):
        self.mqtt_client = None
        self.ui_hook = ui_hook
        self.connected = False

    ####################################################
    #
    def connect(self, ipPort):
        self.mqtt_client = mqtt.Client(client_id="python-ui", userdata=self)
        self.mqtt_client.on_connect = self.on_connect
        self.mqtt_client.on_message = self.on_message
        self.mqtt_client.on_disconnect = self.on_disconnect
        (host, port) = (None, '1883')
        try:
            (host, port) = ipPort.split(':')
        except ValueError:
            host = ipPort
        self.mqtt_client.connect(host, int(port))
        threading.Thread(target=self).start()

    ####################################################
    # The callback for when the client receives a CONNACK response from the server.
    @staticmethod
    def on_connect(client, userdata, flags, rc):
        print("Connected to MQTT connected with result code "+str(rc))

        # Subscribing in on_connect() means that if we lose the connection and
        # reconnect then subscriptions will be renewed.
        client.subscribe("/mach3/dro/+")
        userdata.connected = True
        userdata.ui_hook.on_mqtt_connected()

    ####################################################
    # The callback for when a PUBLISH message is received from the server.
    @staticmethod
    def on_message(client, userdata, msg):
        print(msg.topic+" "+str(msg.payload))
        dro = msg.topic.rsplit('/', 1)[1]
        if dro == '1030':
            userdata.ui_hook.change_laser_mode(int(msg.payload))
        elif dro == '1035':
            userdata.ui_hook.set_pwm_value(int(float(msg.payload) * 255))
        elif dro == '1036':
            userdata.ui_hook.set_init_pwm_value(int(float(msg.payload) * 255))
        elif dro == '1037':
            userdata.ui_hook.set_dash_length(int(float(msg.payload)))

    ####################################################
    #
    @staticmethod
    def on_disconnect(client, userdata, rc):
        userdata.connected = False
        userdata.ui_hook.on_mqtt_disconnected()

    ####################################################
    #
    def __call__(self, *args, **kwargs):
        self.mqtt_client.loop_forever()

    ####################################################
    #
    def is_connected(self):
        return self.connected

    ####################################################
    #
    def disconnect(self):
        self.mqtt_client.disconnect()
