# Robotic Hand Firmward
Firmware and Tools for constructing and programming the Robotic Arm Project available on [Thingiverse](https://www.thingiverse.com/thing:3105948).

## Requirements
The firmware in this project is designed to work on an Arduino Uno. The Servo control and WiFi functions require additional hardware. I used [this](https://www.adafruit.com/product/1411) for the Servo control board, and [this](https://www.amazon.co.uk/ARCELI-ESP8266-Development-Compatible-Arduino/dp/B07J2QKNHB) board to allow WiFi access to the hand. 

Whilst the instructions and code worked for me, there are all sorts of things that will require you to experiment with the code and hardware on your own. If something really isn't working, and you can't find what you need online, feel free to create a GitHub issue. 

## Compilation
Open any of the `.ino` files in [Arduino Studio](https://www.arduino.cc/en/Main/Software). Choose your board from the Board Manager and click `Upload`. 
If you choose not to use the WiFi functionality, in [`config.h`](src/servo_control/config.h) you can comment out `#define WIFI` and the specific code won't compile. Use this file to specify your own hardware parameters too.
