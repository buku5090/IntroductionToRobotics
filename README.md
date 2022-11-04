# IntroductionToRobotics
Introduction to Robotics laboratory homeworks, taken in the 4th year at the Faculty of Mathematics and Computer Science, University of Bucharest. Each homework includes requirements, implementation details, code and image files, depending on the necessity of a more detailed explaination. 

## Homework 0
Installed Arduino IDE and created this repository.<br><br>
Done: 21.10.2022<br>
Deadline: 23.10.2022

## Homework 1
### Hardware Components:
 - 1 RBG LED
 - 3 Potentiometers
 - 1 Arduino UNO board
 - resistors and wires (as needed)
<br>

### Task Requirements:
 - Control each color of the RGB LED (**R**ed, **G**reen and **B**lue) with a different potentiometer.
<br>

### Code:
```
  /* Declaring pot rotary pins -> connecting the potentiometer to arduino UNO */
  const int potRotPinRed = A0;
  const int potRotPinGreen = A1;
  const int potRotPinBlue = A2;

  /* Declaring led pins -> connecting the LEDs to arduino UNO */
  const int ledPinRed = 11;
  const int ledPinGreen = 10;
  const int ledPinBlue = 9;

  /* Declaring variables */
  float potRotValueRed = 0;
  float potRotValueGreen = 0;
  float potRotValueBlue = 0;
  float const maximumVoltage = 5;
  float const maximumPot = 1023;
  float const minPot = 0;
  float const minVoltage = 0;

  void setup() {
    /* Setting up pins */
    pinMode(potRotPinRed, INPUT);
    pinMode(ledPinRed, OUTPUT);

    pinMode(potRotPinGreen, INPUT);
    pinMode(ledPinGreen, OUTPUT);

    pinMode(potRotPinBlue, INPUT);
    pinMode(ledPinBlue, OUTPUT);
    Serial.begin(9600);
  }

  void loop() {
    /* Reading the data from potentiometers -> Rotate to the right to get a stronger color from the LEDs
        The pots are ordered from left to right like that: RED, GREEN, BLUE
    */
    potRotValueRed = analogRead(potRotPinRed);
    potRotValueRed = map(potRotValueRed, minPot, maximumPot, minVoltage, maximumVoltage);

    potRotValueGreen = analogRead(potRotPinGreen);
    potRotValueGreen = map(potRotValueGreen, minPot, maximumPot, minVoltage, maximumVoltage);

    potRotValueBlue = analogRead(potRotPinBlue);
    potRotValueBlue = map(potRotValueBlue, minPot, maximumPot, minVoltage, maximumVoltage);

    /* Lighting the LEDs */
    analogWrite(ledPinRed, potRotValueRed);
    analogWrite(ledPinGreen, potRotValueGreen);
    analogWrite(ledPinBlue, potRotValueBlue);
  }
```
<br>

### Implementation:
 - Firstly, I connected one potentiometer to a one-color LED to understand how it works. 
 - Then I connected 3 potentiometeres and tested each one of them using the Serial.Write() function so that I can see that each potentiometer works properly.
 - I removed the one-color LED and placed the RGB one and I used 1 resistor for each color of the RGB LED. 
 - I placed a non-connected one-color LED next to each of the potentiometers to indicate the color the potentiometer refers to.
 - Finally, I successfully read the input from the potentiometers and redirected it to the LED.
<br>

### Resources:
 - Mostly, I used the data provided in the laboratories.
<br>

### Notes:
 - It was a lot of fun horsing around with Arduino.
<br>

Photo: ![WhatsApp Image 2022-10-26 at 19 17 51](https://user-images.githubusercontent.com/63961737/198081883-00c5223a-eb86-419a-b6d3-baaec2181342.jpeg)
Link video: https://www.youtube.com/shorts/FyiD2DNzpuM

<br><br>
Done: 26.10.2022<br>
Deadline: 28.10.2022

## Homework 2
### Hardware Components:

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
