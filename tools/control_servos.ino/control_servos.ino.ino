
/*************************************************** 
  This is an example for our Adafruit 16-channel PWM & Servo driver
  Servo test - this will drive 8 servos, one after the other on the
  first 8 pins of the PCA9685

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815
  
  These drivers use I2C to communicate, 2 pins are required to  
  interface.

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
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

#define THUMB 0
#define INDEX 2
#define MIDDLE 4
#define LITTLE 6

void setup() {
  pwm.begin();  
  pwm.setPWMFreq(50);  // Digital servos run at 50 Hz updates

  delay(10);

  neutral();

  // Clench the fist, then unclench.
  homeAllServos();
  delay(2000);
  pullBackAllServos();
  delay(2000);
  homeAllServos();
}

// you can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 50;   // 50 Hz
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit");
  pulse *= 1000000;  // convert to us
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

void loop() {

}

void neutral() {
  pwm.setPWM(THUMB, 0, SERVONEUTRAL);
  pwm.setPWM(INDEX, 0, SERVONEUTRAL);
  pwm.setPWM(MIDDLE, 0, SERVONEUTRAL);
  pwm.setPWM(LITTLE, 0, SERVONEUTRAL);
}

void allMaxHalf() {
  pwm.setPWM(THUMB, 0, SERVOHALFMAX);
  pwm.setPWM(INDEX, 0, SERVOHALFMAX);
  pwm.setPWM(MIDDLE, 0, SERVOHALFMAX);
  pwm.setPWM(LITTLE, 0, SERVOHALFMAX);
}

void allMinHalf() {
  pwm.setPWM(THUMB, 0, SERVOHALFMIN);
  pwm.setPWM(INDEX, 0, SERVOHALFMIN);
  pwm.setPWM(MIDDLE, 0, SERVOHALFMIN);
  pwm.setPWM(LITTLE, 0, SERVOHALFMIN);
}

void allMax() {
  pwm.setPWM(THUMB, 0, SERVOMAX);
  pwm.setPWM(INDEX, 0, SERVOMAX);
  pwm.setPWM(MIDDLE, 0, SERVOMAX);
  pwm.setPWM(LITTLE, 0, SERVOMAX);
}

void allMin() {
  pwm.setPWM(THUMB, 0, SERVOMIN);
  pwm.setPWM(INDEX, 0, SERVOMIN);
  pwm.setPWM(MIDDLE, 0, SERVOMIN);
  pwm.setPWM(LITTLE, 0, SERVOMIN);
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
