## Synopsis

Arduino-based laser control board, using W5100 Ethernet chipset. The board will intercept **pulse** and **direction** signals sent from the motion controller to the motor drivers and will report them on the connected clients. On the other hand, the clients can send commands to turn off or on a laser, or to enter an automatic mode, where laser on/off is controlled by the Z direction signal (when Z is up, laser is off, and when Z is down, laser is on).

## Why do I need it?

Mach3 M10/M11 commands are not suitable for controlling the laser in sufficiently fast manner. No matter what methods I tried, the results were always disappointing - either certain pixels were not burned, or they were overburned, by the laser. With this board, the laser control is totally externalized from Mach3 and can be controlled via Ethernet, which allows much better results. 

## Operation

### Inputs
* X pulse - pin D2
* Y pulse - pin D3
* Z pulse - pin D4
* X direction - pin D6
* Y direction - pin D7
* Z direction - pin D8
* W5100 - SPI pins

### Outputs
* Laser trigger - pin D9 - high means laser off, low means laser on

### Operation modes
1. Laser ON - laser is always on
2. Laser OFF - laser is always off
3. Laser AUTO - laser is on when Z direction is 0, and off, when Z direction is 1

Usually we control Laser ON/OFF when engraving from an image - the controlling software will calculate the color at the position and will send ON/OFF commands. We use AUTO mode when engraving/cutting from a vector - we use CAM software and give Z of 0 for trajectories where laser should be ON and Z of 0.005 for trajectories, where laser should be OFF. This is very convenient with the new Cutting paths of Autodesk Fusion 360.

### Ethernet input
Commands list:
z - resets X/Y/Z counters to 0 and stops the laser
1 - turns laser on with full power
0 - turns laser off
=<0-255>:<0-255> - turns laser of with PWM value between 0 and 255. The second value is optional (including the :) and is used for initial PWM value. This helps start the burning process (e.g. you use =127:255, which will use full power for the very first dot, and the it will go down to 50%)
p<byte> - puts laser in automatic mode with PWM of given value (0-255, obviously)
d - dump current X/Y/Z counters
a - puts laser in AUTO mode and full power
A<0-255>:<0-255> - puts laser in AUTO mode with PWM value between 0 and 255. The second value is optional (including the :) and is used for initial PWM value. This helps start the burning process (e.g. you use =127:255, which will use full power for the very first dot, and the it will go down to 50%)

### Ethernet output
<4 bytes X counter><4 bytes Y counter><4 bytes Z counter>\n

## Configurations 
* macro *MAC_ADDRESS* specifies the MAC address of the control board
* macro *IP_ADDRESS* specifies the IP address of the control board
Port is always 4004

## See also
Python control UI



