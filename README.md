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
 - Control each color of the RGB LED (**R**ed, **G**reen and **B**lue)
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
