
/*************************************************** 
  This is the control program for a Adafruit 16-channel 
  PWM & Servo driver. It includes code to accept commands
  over both HTTP and USB Serial. If you wish to use Wifi
  on a suitable board, uncomment "#define WIFI"
  
  This will drive 4 servos, each connected to a different 
  finger of the robotic arm. The control will come from
  a serial connection to the Arduino, which can be 
  from a PC or Wifi Adapter.

  This program accepts 8 bit commands with the following
  format:
  R R R H F F F F

  R - Reserved bit
  H - Hold bit: 0 to release finger, 1 to curl finger. Only applies
      to fingers that have been selected.
  F - Finger bit: 1 to select finger for an action, 0 to not select.
      These can be stacked, so 0 0 0 1 would select the Thumb, and 
      1 0 0 1 would select the Little finger AND Thumb.

  Written by Miles Bardon for the Open Source Robot Arm 
  project found at https://www.instructables.com/id/3D-Printed-Robotic-Arm-2/.
  BSD license, all text above must be included in any redistribution
 ****************************************************/
#include "config.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#ifdef WIFI
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
ESP8266WebServer server(80);
#endif

int finger, hold, reserved;
String inString = "";
const char* ssid = STASSID;
const char* password = STAPSK;
const String host_name = "robothand";

void setup() {
  pwm.begin();  
  pwm.setPWMFreq(50);  // Digital servos run at 50 Hz updates
  pinMode(13, OUTPUT);

  Serial.begin(115200);
  Serial.println("Serial connection ready");

  #ifdef WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println(""); 
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin(host_name)) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/move", handleFingerMovement);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  #endif

  delay(10);
  homeAllServos(); // Start in the un-clenched position.
}

void loop() {
  #ifdef WIFI
  server.handleClient();
  MDNS.update();
  #endif
  
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
        
        Serial.println(constructMessage(finger, hold, reserved));  // Print what the Arduino received to check if they are identical.

        checkData();

        digitalWrite(13, HIGH); // Flashes to show a message was received.
        delay(500);
        digitalWrite(13, LOW);
        
        inString = "";  // Reset the string to accept more data.
      }
    }
  }
}

void parseData(int rec) {
  if (rec <= 255) {
    finger = bitRead(rec, 0) | bitRead(rec, 1) << 1 | bitRead(rec, 2) << 2 | bitRead(rec, 3) << 3;
    hold = bitRead(rec, 4);
    reserved = bitRead(rec, 5) | bitRead(rec, 6) << 1 | bitRead(rec, 7) << 2;
  }
}

void checkData() {
  if ((finger >= 0) && (finger <= 15) && (hold <= 1))  // Makes sure the commands and data received are within specifed bounds.
    carryOutCommand();
}

int constructMessage(int finger, int hold, int reserved) {
  return finger | (hold << 4) | (reserved << 5);
}

void carryOutCommand() {
  for (int i = 0; i < 4; i++) {
    if (bitRead(finger, i) == 1) {
      if (hold == 1)
        pullBackSingleServo(i * 2);
      else
        homeSingleServo(i * 2);
    }
  }
}

void neutral() {
  pwm.setPWM(THUMB, 0, SERVONEUTRAL);
  pwm.setPWM(INDEX, 0, SERVONEUTRAL);
  pwm.setPWM(MIDDLE, 0, SERVONEUTRAL);
  pwm.setPWM(LITTLE, 0, SERVONEUTRAL);
}

// These configurations are based entirely on the physical setup of your arm. 
// Change the order if need be.
void pullBackSingleServo(int port) {
  if ((port == MIDDLE) || (port == INDEX)) {
    pwm.setPWM(port, 0, SERVOMIN);
  } else {
    pwm.setPWM(port, 0, SERVOMAX);
  }
}

void pullBackAllServos() {
  pullBackSingleServo(THUMB);
  pullBackSingleServo(INDEX);
  pullBackSingleServo(MIDDLE);
  pullBackSingleServo(LITTLE);
}

void homeSingleServo(int port) {
  if ((port == MIDDLE) || (port == INDEX)) {
    pwm.setPWM(port, 0, SERVOMAX);
  } else {
    pwm.setPWM(port, 0, SERVOMIN);
  }
}

void homeAllServos() {
  homeSingleServo(THUMB);
  homeSingleServo(INDEX);
  homeSingleServo(MIDDLE);
  homeSingleServo(LITTLE);
}
