const int latchPin = 11;
const int clockPin = 10;
const int dataPin = 12;

const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;

const int pinSW = 2; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output

const int xAxis = 0;
const int yAxis = 1;

int xValue = 0;
int yValue = 0;

bool joyMoved = false;
int digit = 0;
int minThreshold = 350;
int maxThreshold = 750;

int SWState;
int lastSWState = LOW;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; 
unsigned int resetTimer = 2000;

unsigned int long buttonTime = 0;
unsigned int long lastButtonTime = 0;

const byte regSize = 8;

int currentDigit = 0;

int currentDisplay = 0;

int positionDP = 3;

int valueDigits[] = { 0, 0, 0, 0};

int displayDigits[] = {
  segD1, segD2, segD3, segD4
};

const int displayCount = 4;

byte registers[regSize];

const int encodingsNumber = 17;

int state = 0;

byte byteEncodings[encodingsNumber] = {
//A B C D E F G DP 
  B11111100, // 0 
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A
  B00111110, // b
  B10011100, // C
  B01111010, // d
  B10011110, // E
  B10001110, // F
  B00000000  // reset
};

unsigned long startMillis;
unsigned long currentMillis;

void setup() {
 
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(pinSW, INPUT_PULLUP);
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);

  for (int i = 0; i < displayCount; i++){
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }

  startMillis = millis();

  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  currentMillis = millis();

  if(state == 0){
    writeNumber();
    moveJoystickAxis(yAxis);
    readSW();
  }else{
    writeNumber();
    moveJoystickAxis(xAxis);  
    readSW(); 
  }
}


void readSW(){

  int reading = digitalRead(pinSW);

  if(reading != lastSWState){
    lastDebounceTime = millis();
    buttonTime = millis();
  }

  if((millis() - lastDebounceTime) > debounceDelay){

    if(reading != SWState){
      SWState = reading;
    }

    if(SWState == LOW){
      state = !state;
    }
  } else if(millis() - lastDebounceTime > resetTimer){
    reset();
  }

  lastSWState = reading;
}

void moveJoystickAxis(int axis){
  if(axis == yAxis){
    yValue = analogRead(pinY);
    if(minThreshold <= yValue && yValue <= maxThreshold){
      joyMoved = false;
    }

    if(yValue > maxThreshold && joyMoved == false){
      positionDP++;
      if(positionDP == displayCount){
        positionDP = 0;
      }
      joyMoved = true;
    }

    if(yValue < minThreshold && joyMoved == false){
      positionDP--;
      if(positionDP < 0){
        positionDP = displayCount - 1;
      }
      joyMoved = true;
    }

  }else if( axis == xAxis){
    xValue = analogRead(pinX);
    if(minThreshold <= xValue && xValue <= maxThreshold){
      joyMoved = false;
    }

    if(xValue > maxThreshold && joyMoved == false){
      valueDigits[positionDP]++;
      if(valueDigits[positionDP] > 15){
        valueDigits[positionDP] = 0;
      }
      joyMoved = true;
    }

    if(xValue < minThreshold && joyMoved == false){
      valueDigits[positionDP]--;
      if(valueDigits[positionDP] < 0){
        valueDigits[positionDP] = 15;
      }
      joyMoved = true;
    }
  }
}

void writeNumber()
{
  for(int i = 0; i < displayCount; i++){
    activateDisplay(i);
    writeDigit(valueDigits[i]);
  }
}

void writeDigit(int digit) {
  writeReg(byteEncodings[digit], currentDisplay);

  currentDisplay++;
  if(currentDisplay == 4){
    currentDisplay = 0;
  }

  resetReg();

}

void activateDisplay(int displayNumber){
  for(auto display:displayDigits){
    digitalWrite(display, HIGH);
  }

  digitalWrite(displayDigits[displayNumber], LOW);

}

void writeReg(byte encoding, int displayIndex){

  digitalWrite(latchPin, LOW);

  if(state == 0){
    if((millis()%1000  < 250 || millis()%1000> 500 && millis()%1000 < 750) && displayIndex == positionDP){
      encoding ^= 1UL << 0;
      startMillis = currentMillis;
    }
  } else if(state == 1 && displayIndex == positionDP ){
        encoding ^= 1UL << 0;
  }
  shiftOut(dataPin, clockPin, MSBFIRST, encoding);

  digitalWrite(latchPin, HIGH);
  
}

void reset(){
  for(int i = 0; i < displayCount; i++){
    valueDigits[i] = 0;
  }
}

void resetReg(){

  digitalWrite(latchPin, LOW);
  
  shiftOut(dataPin, clockPin, MSBFIRST, byteEncodings[16]);

  digitalWrite(latchPin, HIGH);

}
