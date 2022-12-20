#include "LedControl.h" // need the library
#include <LiquidCrystal.h>
#include <Arduino.h>
#include <EEPROM.h>

// Initializing main menu
int rows = 7;
int cols = 16;
int currRow = 1;
bool movedCursor = false; 
char **mainMenu;

// Initializing the subcategories
char **settingsMenu;
int currRowSettings = 1;
int pBari = 0;
int auxValue = -1;

// Game variables
int diff = 8;
bool win = false;
long int score = 0;
int snakeSpeed = 0;
int snakeLenght = 1;

// EEPROM variables
int currPosEEPROM = 0;
int MatrixBrightnessAddressEEPROM = 0;


int stateOfTheMenu = -1;
// -1 - welcome message
//  0 - main menu
//  1 - game
//  2 - highscore
//  3 - settings
//  4 - about
//  5 - how to play

int buttonState; 
int lastButtonState = LOW;
int lcdBrightValue = 255;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50; 

const int lcdBright = A2;
const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
const int xPin = A0;
const int yPin = A1;
const int buttonPin = 2;

volatile int playerPos = 0;
volatile int playerScore = 0;
volatile int currFoodPosX = 0;
volatile int currFoodPosY = 0;
volatile int lastFoodPosX = 0;
volatile int lastFoodPosY = 0;
volatile int newFoodPosX = 0;
volatile int newFoodPosY = 0;

volatile byte readingButton = LOW;
int xValue = 0;
int yValue = 0;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); // DIN, CLK, LOAD, No. DRIVER
byte matrixBrightness = 2;
byte xPlayerPos = 0;

byte yPlayerPos = 0;
byte xLastPos = 0;
byte yLastPos = 0;
const int minThreshold = 200;
const int maxThreshold = 600;
const byte moveInterval = 100;
unsigned long long lastMoved = 0;
const byte matrixSize = 8;
bool matrixChanged = true;

const byte RS = 9;
const byte enable = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

bool isAtGreeting = 1;
volatile bool startGame = 0;
byte matrix[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};
byte matrixByte[matrixSize] = {
  B00000000,
  B01000100,
  B00101000,
  B00010000,
  B00010000,
  B00010000,
  B00000000,
  B00000000
};

struct coordonates {
  int posY;
  int posX;
};

coordonates playerCoord, foodCoord, enemyCoord;

void setup() {
  playerCoord.posY = 3;
  playerCoord.posX = 3;

  mainMenu = new char*[rows];
  for (int i = 0; i < rows; i++) {
    mainMenu[i] = new char[cols];
  }

  settingsMenu = new char*[rows];
  for (int i = 0; i < rows; i++) {
    settingsMenu[i] = new char[cols];
  }

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  Serial.begin(9600);

  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(lcdBright, OUTPUT);

  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen

  matrix[xPlayerPos][yPlayerPos] = 1;
}

void loop() {
  analogWrite(lcdBright, lcdBrightValue); 

  // read the input
  readInput();
  
  // choose an option from menu
  stateOfMenu();
}

void readInput() {
  readingButton = !digitalRead(buttonPin);
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);
}

void stateOfMenu() {
  switch (stateOfTheMenu) {
    case -1: // welcome message - displayed for 2s
      startLCD();

      changeState(0);

      break;
    case 0: // the main menu
      moveCursorMainMenu();
      
      changeState(currRow);
      currRowSettings = 1;

      break;

    case 1: // start the game
      startTheGame();

      changeState(0);

      break;

    case 2: // highscore
      showHighscore();

      changeState(0);

      break;

    case 3: // settings
      settings();

      if (currRowSettings < 6 and currRowSettings > 1) {
        changeState(currRowSettings + 4);
      }

      if (currRowSettings == 1) {
        changeState(0);
      }
      
      break;

    case 4: // about
      about();

      changeState(0);

      break;

    case 5: // how to play
      howToPlay();
      
      changeState(0);

      break;

    case 6: // difficulty
      difficulty();

      changeState(3);

      break;

    case 7: // lcd brightness
      LCDBrightness();

      changeState(3);

      break;

    case 8: // mat brightness
      matBrightness();

      if (matrixBrightness != auxValue) { // if the values are not equal
        EEPROM.put(MatrixBrightnessAddressEEPROM, matrixBrightness);
      }

      EEPROM.get(MatrixBrightnessAddressEEPROM, auxValue);

      changeState(3);

      break;

    case 9: // sound
      soundVolume();

      changeState(3);

      break;

    case 10: // back to menu
      //returnToMainMenu();

      // changeState(3);

      break;

    default:
      // statements
      break;
  }
}

void difficulty() {
  lcd.setCursor(0, 0);
  lcd.print("Difficulty");  

  lcd.setCursor(0, 1);
  lcd.print(diff);

  if (xValue < minThreshold && movedCursor == false) {
    movedCursor = true;
    if (diff < cols) {
      diff = diff + 2;  
      lcd.clear();
    } 
  }

  if (xValue > maxThreshold && movedCursor == false) {
    movedCursor = true;
    if (diff > 3) {
      diff = diff - 2; 
      lcd.clear();
    } 
  }

  if (xValue < maxThreshold && xValue > minThreshold) {
    movedCursor = false;
  }
}

void LCDBrightness() {
  lcd.setCursor(0, 0);
  lcd.print("LCD - ");
  lcd.setCursor(6, 0);
  lcd.print(lcdBrightValue); 

  if (xValue < minThreshold && movedCursor == false) {
    movedCursor = true;
    if (lcdBrightValue < 255) {
      lcdBrightValue = lcdBrightValue + 50; 
      lcd.clear();
    } 
  }

  if (xValue > maxThreshold && movedCursor == false) {
    movedCursor = true;
    if (lcdBrightValue > 60) {
      lcdBrightValue = lcdBrightValue - 50; 
      lcd.clear();
    } 
  }

  if (xValue < maxThreshold && xValue > minThreshold) {
    movedCursor = false;
  }

  pBari=map(lcdBrightValue, 0, 255, 0, 17);

  for (int i=0; i < pBari; i++) {
    lcd.setCursor(i, 1);   
    lcd.write(byte(0));  
  }
}

void matBrightness() {
  lcd.setCursor(0, 0);
  lcd.print("Matrix - ");
  lcd.setCursor(9, 0);
  lcd.print(matrixBrightness);

  if (xValue < minThreshold && movedCursor == false) {
    movedCursor = true;
    if (matrixBrightness < 15) {
      matrixBrightness = matrixBrightness + 1; 
      lcd.clear();
    } 
  }

  if (xValue > maxThreshold && movedCursor == false) {
    movedCursor = true;
    if (matrixBrightness > 2) {
      matrixBrightness = matrixBrightness - 1; 
      lcd.clear();
    } 
  }

  if (xValue < maxThreshold && xValue > minThreshold) {
    movedCursor = false;
  }

  pBari = map(matrixBrightness, 1, 15, 0, 17);

  for (int i=0; i < pBari; i++) {
    lcd.setCursor(i, 1);   
    lcd.write(byte(0));  
  }
}

void soundVolume() {
  lcd.setCursor(0, 0);
  lcd.print("sound"); 
}

void startLCD() {
  lcd.setCursor(0, 0);
  lcd.print("Hello! SnakeGame");

  lcd.setCursor(0, 1);
  lcd.print("Enjoy! By Buki");

  if (millis() > 40) {
    stateOfTheMenu = 0;
    lcd.clear();
  }
}

void changeState(int state) {
  if (readingButton != lastButtonState) {
        lastDebounceTime = millis();
      }

      if ((millis() - lastDebounceTime) > debounceDelay) {
        if (readingButton != buttonState) {
          buttonState = readingButton;

          if (buttonState == HIGH) {
            stateOfTheMenu = state;
            lcd.clear();
          }
        }
      }

      lastButtonState = readingButton;
}

void howToPlay() {
  lcd.setCursor(0, 0);
  lcd.print("how to pl");
}

void about() {
  lcd.setCursor(0, 0);
  lcd.print("about");  
}

void settings() {
  settingsMenu[1] = "<-Main Menu";
  settingsMenu[2] = "a)Difficulty";
  settingsMenu[3] = "b)LCD Brightness";
  settingsMenu[4] = "c)MAT Brightness";
  settingsMenu[5] = "d)Sound";
  settingsMenu[6] = " ";

  lcd.setCursor(0, 0);
  lcd.print(settingsMenu[currRowSettings]);

  lcd.setCursor(15, 0);
  lcd.print("<");
  
  lcd.setCursor(0, 1);
  lcd.print(settingsMenu[currRowSettings + 1]);

  int last_value = 2;  
  moveInMenu(currRowSettings, last_value);  
}

void startTheGame() {
  lcd.setCursor(0, 0);
  lcd.print("Enjoy the Game!");

  lcd.setCursor(0, 1);
  lcd.print("Score:");

  lcd.setCursor(7, 1);
  lcd.print(score);
  score = millis() / 1000;

  lc.setLed(0, playerCoord.posY, playerCoord.posX, true);
  moveSnake();
  /* for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, true); // turns on LED at col, row
      //delay(25);
    }
  }
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, false); // turns off LED at col, row
      //delay(25);
    }
  } */
}

void runGame() {
  updateByteMatrix();
  if (millis() - lastMoved > moveInterval) {
    // game logic
    updatePositions();

    // increase score
    if (xPlayerPos == currFoodPosX && yPlayerPos == currFoodPosY) {
      playerScore++;

      // generate the next pos for the food
      generateFood();      
    }

    lastMoved = millis();
  }

  if (matrixChanged == true) {
    // matrix display logic
    updateMatrix();
    matrixChanged = false;
  } 
}

void moveSnake() {
  playerCoord.posY += 1;
  if(playerCoord.posY > 8) {
    playerCoord.posY = 0;
  }

  Serial.println(playerCoord.posY);

  updateMatrix();
}

// function to move in the menu
void moveCursorMainMenu() {
  mainMenu[1] = "1. Start Game";
  mainMenu[2] = "2. Highscore";
  mainMenu[3] = "3. Settings";
  mainMenu[4] = "4. About";
  mainMenu[5] = "5. How to play";
  mainMenu[6] = " ";

  lcd.setCursor(0, 0);
  lcd.print(mainMenu[currRow]);

  lcd.setCursor(15, 0);
  lcd.print("<");
  
  lcd.setCursor(0, 1);
  lcd.print(mainMenu[currRow + 1]);

  int lastValue = 2;
  moveInMenu(currRow, lastValue);
}

void showHighscore() {
  lcd.setCursor(0, 0);
  lcd.println("salutare");
}

void generateFood() {
  // lastFoodPos = currentPos;
  // newFoodPos = random(ceva);
  // matrix[lastFoodPos] = 0;
  // matrix[newFoodPos] = 1;
  lastFoodPosX = currFoodPosX;
  lastFoodPosY = currFoodPosY;

  newFoodPosX = random(7);
  newFoodPosY = random(7);

  currFoodPosX = newFoodPosX;
  currFoodPosY = newFoodPosY;

  matrix[lastFoodPosX][lastFoodPosY] = 0;
  matrix[currFoodPosX][currFoodPosY] = 1;

  matrixChanged = true;
}
void updateByteMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    lc.setRow(0, row, matrixByte[row]);
  }
}

void updateMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}

void updatePositions() {
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  xLastPos = xPlayerPos;
  yLastPos = yPlayerPos;

  if (xValue < minThreshold) {
    if (xPlayerPos < matrixSize - 1) {
      xPlayerPos++;
    }
    else {
      xPlayerPos = 0;
    }
  }

  if (xValue > maxThreshold) {
    if (xPlayerPos > 0) {
      xPlayerPos--;
    }
    else {
      xPlayerPos = matrixSize - 1;
    }
  }

  if (yValue > maxThreshold) {
    if (yPlayerPos < matrixSize - 1) {
      yPlayerPos++;
    }
    else {
      yPlayerPos = 0;
    }
  }

  if (yValue < minThreshold) {
    if (yPlayerPos > 0) {
      yPlayerPos--;
    }
    else {
        yPlayerPos = matrixSize - 1;
    }
  }

  if (xPlayerPos != xLastPos || yPlayerPos != yLastPos) {
    matrixChanged = true;
    matrix[xLastPos][yLastPos] = 0;
    matrix[xPlayerPos][yPlayerPos] = 1;
  }
}

void moveInMenu(int &row, int val) {
  if (xValue < minThreshold && movedCursor == false) {
    movedCursor = true;
    Serial.println("up");
    if (row > 1) {
      row = row - 1;  
      lcd.clear();
    } 
  }

  if (xValue > maxThreshold && movedCursor == false) {
    movedCursor = true;
    Serial.println("down");
    if (row < rows - val) {
      row = row + 1;  
      lcd.clear();
    } 
  }

  if (xValue < maxThreshold && xValue > minThreshold) {
    movedCursor = false;
  }
}