
// declare all the joystick pins
const int pinSW = 2; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output

// declare all the segments pins
const int pinA = 4;
const int pinB = 5;
const int pinC = 6;
const int pinD = 7;
const int pinE = 8;
const int pinF = 9;
const int pinG = 10;
const int pinDP = 11;

volatile byte stateA = LOW;
volatile byte stateB = LOW;
volatile byte stateC = LOW;
volatile byte stateD = LOW;
volatile byte stateE = LOW;
volatile byte stateF = LOW;
volatile byte stateG = LOW;
volatile byte stateDP = LOW;

const int segSize = 8;
int index = 0;

// modify if you have common anode
bool commonAnode = false; 
const int noOfDigits = 10;
byte state = HIGH;
byte dpState = LOW;
byte swState = LOW;
byte lastSwState = LOW;
byte tempState = HIGH;
int xValue = 0;
int yValue = 0;

// blinking sequence
long timeToBlink = 0;

bool joyMoved = false;
int digit = 0;
int minThreshold = 400;
int maxThreshold = 600;

int segments[segSize] = { 
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

int states[segSize] = {
  stateA, stateB, stateC, stateD, stateE, stateF, stateG, stateDP
};

// phase 1
int currLed = segments[segSize - 1];
int timeToReset = 3000;
long dpStateDebounceTime = 0;

// phase 2
int currPos = 0;
long timeToStateTwo = 500;
volatile byte boolPhaseTwo = 0;
volatile byte wasInPhaseTwo = 0;

void setup() {
  // initialize all the pins
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
    states[i] = LOW;
  }
  pinMode(pinSW, INPUT_PULLUP);
  if (commonAnode == true) {
    state = !state;
  }

  Serial.begin(9600);
}

void loop() {
  // y axys for left and right. x axys for up and down
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  swState = digitalRead(pinSW);

  // blinks
  blink(currLed);

  // start of phase 1
  if (boolPhaseTwo == 0) {
    // PHASE 1 - THE MOVING

    // LEFT AND RIGHT
    // -- LEFT
    if (yValue < minThreshold && joyMoved == false) {
      if(currLed == pinA || currLed == pinB) {
        stopBlinking(currLed);
        currLed = pinF;
      }
      else if (currLed == pinC || currLed == pinD) {
        stopBlinking(currLed);
        currLed = pinE;
      }
      else if (currLed == pinDP) {
        stopBlinking(currLed);
        currLed = pinC;
      }

      joyMoved = true;
    }

    // -- RIGHT
    if (yValue > maxThreshold && joyMoved == false) {
      stopBlinking(currLed);
      if(currLed == pinA || currLed == pinF) {
        stopBlinking(currLed);
        currLed = pinB;
      }
      else if (currLed == pinD || currLed == pinE) {
        stopBlinking(currLed);
        currLed = pinC;
      }
      else if (currLed == pinC) {
        stopBlinking(currLed);
        currLed = pinDP;
      }

      joyMoved = true;
    }

    // UP AND DOWN
    // -- DOWN
    if (xValue < minThreshold && joyMoved == false) {
      if(currLed == pinA || currLed == pinB || currLed == pinF) {
        stopBlinking(currLed);
        currLed = pinG;
      }
      else if (currLed == pinC || currLed == pinE || currLed == pinG) {
        stopBlinking(currLed);
        currLed = pinD;
      }

      joyMoved = true;
    }

    // -- UP
    if (xValue > maxThreshold && joyMoved == false) {
      if(currLed == pinB || currLed == pinF || currLed == pinG) {
        stopBlinking(currLed);
        currLed = pinA;
      }
      else if (currLed == pinC || currLed == pinD || currLed == pinE) {
        stopBlinking(currLed);
        currLed = pinG;
      }

      joyMoved = true;
    } 

    // END MOVE
    if ((xValue >= minThreshold && xValue <= maxThreshold) && (yValue >= minThreshold && yValue <= maxThreshold)) {
      joyMoved = false;
    }
  }

  // PHASE 1 - THE RESET
  if (swState != lastSwState) {
    dpStateDebounceTime = millis();
    if (swState == LOW) {
      dpState = !dpState; // pinDP initially is OFF
    }
  }

  // check if phase 2 starts - .5s press
  if ((millis() - dpStateDebounceTime) > timeToStateTwo && dpState == HIGH) {
    // start phase 2
    boolPhaseTwo = 1; // phase 2
    if (wasInPhaseTwo == 1) {
      boolPhaseTwo = 0; // phase 1
      wasInPhaseTwo = 0;
    }

    // reset - 3s press
    if ((millis() - dpStateDebounceTime) > timeToReset && dpState == HIGH) {
      stopBlinking(currLed);
      currLed = pinDP;
      dpState = !dpState;

      // switch to phase 1
      boolPhaseTwo = 0;
    }
  }

  // PHASE 2 - THE SWITCHING
  if (boolPhaseTwo == 1) {
    wasInPhaseTwo = 1;
    /* for (int i = 0; i < segSize; i++) {
      /* if (currLed == segments[i]) {
        currPos = i;
      } 
      states[i] = HIGH;
    } */

    // assign current position 
    /* states[currPos] = !states[currPos];
    digitalWrite(currLed, currPos); */

    /* for (int i = 0; i < segSize; i++) {
      digitalWrite(segments[i], states[i]);
    } */

      // UP - TURN LED ON OR OFF
      // -- TURN ON LED
      if (xValue > maxThreshold && joyMoved == false) {
        // FIND the position of the current led
        for (int i = 0; i < segSize; i++) {
          if (currLed == segments[i]) {
            currPos = i;
          }
        }

        // change its state
        digitalWrite(segments[currLed], states[currPos]);

        joyMoved = true;
      } 

      // END MOVE
      if (xValue >= minThreshold && xValue <= maxThreshold) {
        joyMoved = false;
      }
      /* stopBlinking(currLed);
      currLed = pinDP;
      dpState = !dpState; */
    }

  
  lastSwState = swState;
  delay(1);
}

// blinks every 250ms
void blink(int led) {
  int blinkPause = 250;

  if (millis() - timeToBlink > blinkPause) {
    digitalWrite(led, tempState);
    tempState = !tempState;
    timeToBlink = millis();
  }
}

// stops blinking
void stopBlinking(int led) {
  digitalWrite(led, LOW);
}