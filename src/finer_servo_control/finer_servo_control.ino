
/*************************************************** 
  This is the control program for a Adafruit 16-channel 
  PWM & Servo driver.
  
  This will drive 4 servos, each connected to a different 
  finger of the robotic arm. The control will come from
  a serial connection to the Arduino, which can be 
  from a PC or Wifi Adapter.

  Commands are as follows:
   - 

  Written by Miles Bardon for the Open Source Robot Arm 
  project found at https://www.instructables.com/id/3D-Printed-Robotic-Arm-2/.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN 150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOHALFMIN 229
#define SERVONEUTRAL 307
#define SERVOHALFMAX 394
#define SERVOMAX 480 // this is the 'maximum' pulse length count (out of 4096)

// Define which pins your finger servos will be connected to. As there are 16 
// pins on the board, I chose to space them out for ease of access.
#define THUMB 0
#define INDEX 2
#define MIDDLE 4
#define LITTLE 6

int data;
String inString = "";

void setup() {
  pwm.begin();  
  pwm.setPWMFreq(50);  // Digital servos run at 50 Hz updates

  Serial.begin(9600);
  Serial.println("Ready");

  delay(10);
  homeAllServos(); // Start in the un-clenched position.
}

void loop() {
  int rec;
  int CR = '\r';  // Carriage return
  int NL = '\n';  // New Line

  // This loop reads individual bytes into a buffer to be extracted.
  if (Serial.available() > 0) {
    int inChar = Serial.read();

    if (isDigit(inChar)) {
      inString += (char)inChar;   //Forming the string
    }

    if ((inChar == CR) || (inChar == NL)) { // Wait until either a new line or a carriage return indicating the end of the data.
      if (inString != "") {  // Only attempt to parse when data has been read.
        rec = inString.toInt();
        parseData(rec);
        
        Serial.println(constructMessage(cmd, data));  // Print what the Arduino received to check if they are identical.

        checkData();

        digitalWrite(13, HIGH); // Flashes to show a message was received.

        inString = "";  // Reset the string to accept more data.
      }
    }
  }
}

void parseData(int rec) {
  if (rec <= 255) {
    cmd = bitRead(rec, 0) | bitRead(rec, 1) << 1 | bitRead(rec, 2) << 2 | bitRead(rec, 3) << 3;
    data = bitRead(rec, 4) | bitRead(rec, 5) << 1 | bitRead(rec, 6) << 2;
    dir = bitRead(rec, 7);
  }
}

void checkData() {
  if ((cmd >= 0) && (cmd <= 15) && (data >= 0) && (data <= 7))  // Makes sure the commands and data received are within specifed bounds.
    carryOutCommand();
}

void neutral() {
  pwm.setPWM(THUMB, 0, SERVONEUTRAL);
  pwm.setPWM(INDEX, 0, SERVONEUTRAL);
  pwm.setPWM(MIDDLE, 0, SERVONEUTRAL);
  pwm.setPWM(LITTLE, 0, SERVONEUTRAL);
}

// These configurations are based entirely on the physical setup of your arm. 
// Change the order if need be.
void pullBackAllServos() {
  pwm.setPWM(THUMB, 0, SERVOMIN);
  pwm.setPWM(INDEX, 0, SERVOMIN);
  pwm.setPWM(MIDDLE, 0, SERVOMAX);
  pwm.setPWM(LITTLE, 0, SERVOMAX);
}

void homeAllServos() {
  pwm.setPWM(THUMB, 0, SERVOMAX);
  pwm.setPWM(INDEX, 0, SERVOMAX);
  pwm.setPWM(MIDDLE, 0, SERVOMIN);
  pwm.setPWM(LITTLE, 0, SERVOMIN);
}
