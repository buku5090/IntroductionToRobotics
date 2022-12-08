# IntroductionToRobotics
Introduction to Robotics laboratory homeworks, taken in the 4th year at the Faculty of Mathematics and Computer Science, University of Bucharest. Each homework includes requirements, implementation details, code and image files, depending on the necessity of a more detailed explaination. 

## Homework 0
Installed Arduino IDE and created this repository.<br><br>
Done: 21.10.2022<br>
Deadline: 23.10.2022

## Homework 1

### Task Requirements:
 - Control each color of the RGB LED (**R**ed, **G**reen and **B**lue) with a different potentiometer.
<br>

## Homework 2
### Hardware Components:
5 LEDs, 1 button, 1 buzzer, resistors and wires (per logic)

### Code:
```
const int buttonPin = 2;
const int ledPeopleGreenPin = 13;
const int ledPeopleRedPin = 12;

const int ledCarGreenPin = 11;
const int ledCarYellowPin = 10;
const int ledCarRedPin = 9;

byte buttonState = 0;
byte lastButtonState = -1;

byte ledPeopleRedState = 1;
byte ledPeopleGreenState = 0;
byte ledCarGreenState = 1;
byte ledCarYellowState = 0;
byte ledCarRedState = 0;

unsigned int timeToStateTwo = 8000;
unsigned int timeToStateThree = 11000;
unsigned int timeToStateFour = 19000;
unsigned int timeToStateReset = 23000;
unsigned int timeCounter = 0;

unsigned int ledTemp = 0;
unsigned int safeVar = 0;

unsigned int buttonPushCounter = 0;

const int buzzer = 8;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(buzzer, OUTPUT);
  pinMode(ledPeopleGreenPin, OUTPUT);
  pinMode(ledPeopleRedPin, OUTPUT);
  pinMode(ledCarGreenPin, OUTPUT);
  pinMode(ledCarYellowPin, OUTPUT);
  pinMode(ledCarRedPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // read the input pin:
  buttonState = digitalRead(buttonPin);
  ledPeopleRedState = buttonState;
  ledCarGreenState = !ledPeopleRedState;

  if (buttonState != lastButtonState && safeVar == 0) {
    // Remember the time the button was pressed
    if (buttonState == LOW) {
        timeCounter = millis(); // -> 8 seconds
      	buttonPushCounter++;
    }

    // Remember current button state
    lastButtonState = buttonState;
  }

  // Second state - Yellow for cars, red for people, no sounds -> 3 seconds
  if (millis() - timeCounter > timeToStateTwo && timeCounter > 0) {
    // Protecting the application from unwanted button presses
    safeVar = 1;

    // Turn off green light for cars
    digitalWrite(ledCarGreenPin, LOW);

    // Turn on yellow light for cars
    digitalWrite(ledCarYellowPin, HIGH);
    
    // Third State - Red for cars, green for people + a beep of sound -> 8 seconds
    if (millis() - timeCounter > timeToStateThree) {
      // beeping
      ledTemp = millis() - timeCounter; // 11001
      if ((ledTemp / 750) % 2 == 0) {
        tone(buzzer, 1000);
      } 
      else {
        noTone(buzzer);
      }

      // turn off the yellow light for cars
      digitalWrite(ledCarYellowPin, LOW);

      // turn on red light for cars
      digitalWrite(ledCarRedPin, HIGH);

      // turn off red light for people
      digitalWrite(ledPeopleRedPin, LOW);
      
      // turn on green light for people
      digitalWrite(ledPeopleGreenPin, HIGH);

      // Fourth State - Red for cars, blinking green for people + beeping -> 4 seconds
      if (millis() - timeCounter > timeToStateFour) {
        // Make the led blink
        ledTemp = millis() - timeCounter; // 19001
        if ((ledTemp / 250) % 2 == 0) {
          // turn on green light for people
          tone(buzzer, 1000);
          digitalWrite(ledPeopleGreenPin, HIGH);
        } 
        else {
          // turn on green light for people
          noTone(buzzer);
          digitalWrite(ledPeopleGreenPin, LOW);
        }

        // Time to reset
        if(millis() - timeCounter > timeToStateReset) {
          timeCounter = 0;
          safeVar = 0;
          noTone(buzzer);
        }

      }
    }
  }

  // First state - green for cars, red for people, no sound
  if (timeCounter == 0) {
    // resetting the leds
    digitalWrite(ledPeopleGreenPin, LOW);
    digitalWrite(ledPeopleRedPin, HIGH);

    digitalWrite(ledCarGreenPin, HIGH);
    digitalWrite(ledCarYellowPin, LOW);
    digitalWrite(ledCarRedPin, LOW);
  }
}
```
<br>

Photo: ![WhatsApp Image 2022-11-06 at 21 27 41](https://user-images.githubusercontent.com/63961737/200191603-5bf25d3a-510b-422c-b837-cdfc034e5f0d.jpeg)

Link video: https://youtube.com/shorts/YoVPiFFsUM8?feature=share

<br><br>

## homework 3
```

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
```
