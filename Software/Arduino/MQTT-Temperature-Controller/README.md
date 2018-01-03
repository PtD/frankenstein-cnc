## Synopsis

Arduino-based PID temperature controller, controlled via MQTT. Ethernet connectivity is based on ENC28J60 chipset. Temperature measurement is done via thermistor.

## Inputs and outputs

### Inputs
Thermistor is read on pin A3

### Outputs
* ENC28J60 select is on pin D10
* Heater control is on pin D3
* Fan PWM control is on pin D5

### ENC28J60 
* Connected to respective SPI pins


## Topics and messages

### Inputs
* /mach3/dro/1010 - set temperature. Temperature of 0 means turn off heater. Temperature is in deg.C)
* /mach3/dro/1020 - turn fan on or off (message '1' means turn it on, '0' is turn it off)

### Outputs
* /dev/dro/1011 - current set temperature
* /dev/dro/1012 - set temperature was reached
* /dev/dro/1013 - heater state
* /dev/dro/1014 - current PID output
* /dev/dro/1021 - fan state


## IP config
You can change IP and MAC addresses by editing *MAC_ADDRESS* and *IP_ADDRESS* macros.

The MQTT server address is defined by *SERVER_IP_ADDRESS* macro.

## PID config
PID parameters are defined by Kp, Ki and Kd variable. You have to fine tune them to your specific extruder.

