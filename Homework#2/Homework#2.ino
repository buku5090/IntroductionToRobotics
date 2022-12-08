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