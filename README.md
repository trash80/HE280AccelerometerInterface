# HE280AccelerometerInterface

This is a simple Arduino program for interfacing a Teensy 3/3.2 to Seemecnc's HE280 hotend probe for use with a Duet controller or other 3D printer controllers that do not have i2c probing options.

This can easily work with other micro controllers. I used a 3.2 because it's both 5v tolerant and 3v3 output, which is what the Duet needs. The HE280 uses a 3v Accelerometer IC, but the PCB has logic level shifting to 5v.

Pin configuration is commented in the main source file.
