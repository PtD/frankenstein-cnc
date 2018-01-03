## Synopsis

Mach3MQTT plugin is a plugin (duh!) for Mach3 motion control software, that exposes changes in DRO variables to MQTT clients via specific topics, and also listens for topics and changes DROs based on the passed messages. This allows for (admittedly limited) integration of Mach3 with other devices that speak MQTT, bypassing the limitation of only one TCP ModBus device that Mach3 has.

## How to build and install

### Prerequisites
* Paho MQTT Client https://www.eclipse.org/paho/clients/c/
* Mach3 Plugin Template (https://www.machsupport.com/forum/index.php/topic,10733.0.html)
* Visual Studio 2017 (we use Community Edition) (it can be build with older versions as well, but may require re-configuration of the project, YMMV)

### Steps
1. Build Paho client
2. Edit Mach3MQTT.vcxproj and set the proper paths for *Paho_IncludePath*, *Paho_LibraryPath* and *Mach3_IncludePath*
3. Open the project in Visual Studio and build it
4. Either copy *paho-mqtt3c.dll* to your Mach3 directory, or add its location to the system PATH variable
5. Copy *Mach3MQTT.dll* to your Mach3\plugins directory

## How to use
1. Run a MQTT server (we use Mosquitto https://mosquitto.org/)
2. Edit your desired Mach3 profile XML file (e.g. Mach3Mill.xml)
  * in <profile> <preferences> ... add <MQTTServer>**MQTT-server-IP:port</MQTTServer>
3. Enable the plugin from Mach3 plugin options
4. Save options and restart Mach3
5. Restart Mach3

If you are successful, you'll see that a client named *Mach3-<id>* will subscribe to the MQTT server.

## DRO notifications and assignment

The plugin will subscribe for the topics on */dev/dro/+*. The incoming data on these topics is treated as a new DRO value, and the DRO number is determined from the last part of the topic. For example, if *1.234* is sent to */dev/dro/200*, *DRO 200* will be set to *1.234* (esentially, this is equivalent to *SetDRO(200, 1.234)*)

On the other hand, the plugin will scan for changes in the monitored DROs and will send a message to */mach3/dro/xxxx* topic with the DRO value formatted as a string. For example if the plugin determines that DRO 201 has changed to a value of 4.321, it will send message *4.321* to topic */mach3/dro/201*.

The DRO range, corresponding to SetDRO Basic call is *200 to 1454*. 
The DRO range, corresponding to SetUserDRO Basic call is *1000 to 2254*.
In other words SetUserDRO is offset by 800 to SetDRO.

These were verified experimentally, since Mach3 internal workings information is not exactly abundant.

