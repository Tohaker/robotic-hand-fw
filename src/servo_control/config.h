#define WIFI

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

// Define your own local Wifi SSID and Password. The board will connect to the Wifi
// if it can find it.
#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK  "your-password"
#endif