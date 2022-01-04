#include <Adafruit_CircuitPlayground.h>
#include <Arduino.h>
#include <vector>

using namespace std;

// initialize state
#define IDLE false
#define ACTIVE true

// initialize led pins numbers
#define LED0 0
#define LED1 1
#define LED2 2
#define LED3 3
#define LED4 4
#define LED5 5
#define LED6 6
#define LED7 7
#define LED8 8
#define LED9 9

// initialize led colors
#define red    0x00ff0000
#define orange 0x00ffa500 
#define yellow 0x00ffff00 
#define green  0x0000ff00 
#define blue   0x000000ff 
#define violet 0x00ee82ee

#define r 0 // xPos
#define o 1 // xNeg
#define y 2 // yPos
#define g 3 // yNeg
#define b 4 // zPos
#define v 5 // zNeg

// initialize coordinates
float X, Y, Z;

// initialize led mapping patterns for 5 nodes
int node1[10] = {3, 4, 2, 8, 7, 1, 6, 0, 5, 9};
int node2[10] = {0, 5, 3, 9, 4, 2, 8, 1, 6, 7}; 
int node3[10] = {4, 8, 1, 7, 2, 9, 0, 6, 3 ,5};
int node4[10] = {5, 3, 8, 2, 0, 6, 1, 7, 9, 4};
int node5[10] = {1, 9, 0, 5, 3, 8, 2, 4, 7, 6};
int node6[10] = {7, 2, 1, 9, 4, 6, 3, 5, 8, 0};

// initialize 10 messages 
// each movement is assoisciate with a color 
// messages were given random length between 4 & 7
// movements were randomly chosen within each message
vector<int> message0 = {b, b, y, g, g, r, b};
vector<int> message1 = {o, y, b, o, o, b, o};
vector<int> message2 = {g, o, g, y, b};
vector<int> message3 = {b, y, y, b, b, g, y};
vector<int> message4 = {v, g, r, v, g, r};
vector<int> message5 = {r, y, v, o, o};
vector<int> message6 = {o, r, r, b, r};
vector<int> message7 = {v, o, y, g, b, b, r};
vector<int> message8 = {r, o, r, v, y, y, r};
vector<int> message9 =  {r, r, b, g, r, r};

// initialize current messageVector
vector<int> currMessage;

// initialize current message number variable
int currMessageNum;

// initialize current movement to -1
int currMovement = -1;

// initialize which node you are (1-5)
int node = 1;

// reset: resets board back to initial state
// parameters:
//      none
// returns:
//      nothing
void reset() {
  X = 0;
  Y = 0;
  Z = 0;
  currMessage.clear();
  currMessageNum = -1;
  currMovement = -1;
}

// toggleLED: turns on/off specific led on board
// parameters:
//       ledNum - specifies which led number to turn on (LED0 - LED9)
//       state - (on = true, off = false)
//       color - which color to light up led with (red, green, or blue)
// returns:
//       nothing
void toggleLED(int ledNum, uint8_t state, uint32_t color = 0x00000000) {
  if (state == HIGH)
    CircuitPlayground.setPixelColor(ledNum, ((color & 0x00ff0000) >> 16), ((color & 0x0000ff00) >> 8), (color & 0x000000ff));
  else
    CircuitPlayground.clearPixels();
}

// map: maps messages to led pin
// parameters:
//      node - indicates which node we are mapping
//      message - indicates message we want send
// returns:
//      led value to turn on
int mapMessagetoLED(int node, int message) {
  switch(node) {
    case 1: //Case statement for node that is indicated
      return node1[message];
    case 2:
      return node2[message];
    case 3:
      return node3[message];
    case 4:
      return node4[message];
    case 5:
      return node5[message];
    case 6:
      return node6[message];
    default:
      return -1; // sent node value not between 1 - 5
  }
}

// getState: determines whether board is idle or active
// parameters:
//      none
// returns:
//      active state = true or idle state   = false
bool getState() {
  if (CircuitPlayground.leftButton())
    return ACTIVE;
  else
    return IDLE;
}


// getSample: collects sample coordinate data while left button is pressed and returns average
//            coordinates of the sampled data
// parameters:
//      none
// returns:
//      vector array with average coordinates of samples
vector<float> getSample() {
  vector<float> sample(3, 0.0);
  int count = 0;
  while (getState() == ACTIVE) {
    sample[0] += X;
    sample[1] += Y;
    sample[2] += Z;
    ++count;
    delay(50);
  }

  if (count != 0){  
    sample[0] /= count;
    sample[1] /= count;
    sample[2] /= count;
    }
  
  return sample;
}

// getMovement: determines movement and returns which one it is
// parameters:
//      none
// returns:
//      returns movement represented by int value
int getMovement() {
  vector<float> coordinates = getSample();
  float avgX = coordinates[0];
  float avgY = coordinates[1];
  float avgZ = coordinates[2];

  // check X-axis movements
  if (avgX >= 8.0) return r;
  else if (avgX <= -8.0) return o;
  // check Y-axis movements
  else if (avgY >= 8.0) return y;
  else if (avgY <= -8.0) return g;
  // check Z-axis movement
  else if (avgZ >= 8.0) return b;
  else if (avgZ <= -8.0) return v;
  // movement not detected
  else return -1; 
}

// getMessage: determines what message is and returns message number
// parameters:
//      none
// returns:
//      return which message it is (0 - 9) corresponding to sequence of movements
int getMessage() {
  // compare array to 10 predetermine messages
  if (currMessage.empty()) {
    return -1;
  }
  else if (currMessage == message0) {
    currMessage.clear();
    return 0;
  }
  else if (currMessage == message1) {
    currMessage.clear();
    return 1;
  }
  else if (currMessage == message2) {
    currMessage.clear();
    return 2;
  }
  else if (currMessage == message3) {
    currMessage.clear();
    return 3;
  }
  else if (currMessage == message4) {
    currMessage.clear();
    return 4;
  }
  else if (currMessage == message5) {
    currMessage.clear();
    return 5;
  }
  else if (currMessage == message6) {
    currMessage.clear();
    return 6;
  }
  else if (currMessage == message7) {
    currMessage.clear();
    return 7;
  }
  else if (currMessage == message8) {
    currMessage.clear();
    return 8;
  }
  else if (currMessage == message9) {
    currMessage.clear();
    return 9;
  }
  else return -1;
}

// outputMessagetoLED: sends message to output (leds 1 - 10)
// parameters:
//      node - which node to use to output message
//      color - color which to light up led with
// returns:
//      nothing
void outputMessagetoLED(int node, int color) {
  // map message number to which led to turn on
  int ledNum = mapMessagetoLED(node, currMessageNum);
  // map int value to hex color RGB value
  uint32_t colorRGBVal;
  switch(color) {
    case 0:
      colorRGBVal = red;
      break;
    case 1:
      colorRGBVal = orange;
      break;
    case 2:
      colorRGBVal = yellow;
      break;
    case 3:
      colorRGBVal = green;
      break;
    case 4:
      colorRGBVal = blue;
      break;
    case 5:
      colorRGBVal = violet;
      break;
    default:
      colorRGBVal = 0x00000000;
      break;
  }
  // toggle led
  toggleLED(ledNum, HIGH, colorRGBVal);
  delay(250);
  toggleLED(ledNum, LOW);
  delay(250);
}

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
}

void loop() {
  X = CircuitPlayground.motionX();
  Y = CircuitPlayground.motionY();
  Z = CircuitPlayground.motionZ();

  // reset if right button is pressed
  if (CircuitPlayground.rightButton()) {
    reset();
  }

  // capture current movement
  currMovement = getMovement();
  // Serial.println(currMovement);
  // if board not idle add movement to curr message vector
  if (currMovement != -1) {
    currMessage.push_back(currMovement);
  }
  // capture length before message is cleared
  int length = static_cast<int>(currMessage.size());
  // get message number
  currMessageNum = getMessage();
  // if message is valid (not -1) turn on led on board
  if (currMessageNum != -1) {
    for (int i = 0; i < length; i++) {
      outputMessagetoLED(node, currMessage[i]);
    }
  }


  Serial.println(currMessage.size());
  for (int i = 0; i < currMessage.size();i++){
    Serial.print(currMessage[i]);
    Serial.print(" ");
  }
  Serial.print("\n");
  delay(1000);
  
}