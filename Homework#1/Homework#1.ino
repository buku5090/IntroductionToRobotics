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