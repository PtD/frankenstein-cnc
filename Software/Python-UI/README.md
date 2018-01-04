## Synopsis

UI for controlling the laser control board via Ethernet


## How to run
It uses Python 2.7 with PIL (Python Imaging Library). Just execute:

python ui.py

in the directory where ui.py resides.

## Functionality

1. Directly set the laser mode (on/off/auto), and laser PWM (and also initial PWM - it will be used immediately after the laser has been turned on, and then it will be replaced with the regular PWM)

2. Control image engraving (image must be monochrome!)
  * Proper X/Y resolution must be configured (number of impulses the stepper motor drivers receive to move one mm)
  * Proper synchronization must be done between CNC controller coordinates and the Laser controller UI - before the job start, enter the X/Y DRO values in *Initial coordinates* fields and press *Reset*
  * Open the image file
  

3. Generate optimized G-code for image engraving
  * If you don't have G-code for the job, you can generate one by setting the desired width/height, vertical step and feedrate and then press *Save* to store the generated G-code. Note that the image must be opened to be able to generate G-code for it.
