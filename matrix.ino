#include <LiquidCrystal.h>
#include "LedControl.h"
#include <EEPROM.h>

const int pinSW = A0;
const int pinX = A2;
const int pinY = A1;


const byte RS = 9;
const byte enable = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
const byte pinBrightness = 3;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

const int minThreshold = 400;
const int maxThreshold = 600;

const String reserve = "aaa";
String name = reserve;

int xValue;
int yValue;
byte swState;
byte lastSwState;

const int matrixSize = 8;
const byte matrixRightLimit = matrixSize - 1;

int joyMovedX;
int joyMovedY;

int menuOption = 1;

int lcdBrightness;
int ledBrightness;

const int lcdBrightnessLocation = 0;
const int ledBrightnessLocation = 1;
const int nameLocation = 2;
const int leaderboardNameLocation = 5;
const int leaderboardScoreLocation = 8;
bool newHighscore;
int highscore = 0;
String highscoreName = "top";

int indicator;
int lcdPage = 1;

int score = 0;
byte fullBox[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
byte emptyBox[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte displayMatrix[matrixSize] = {
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
};

byte deathScreen[8] = {
  B00000000,
  B01000010,
  B00100100,
  B00011000,
  B00011000,
  B00100100,
  B01000010,
  B00000000,
};

byte speedUp[8] = {
  B00000000,
  B00011000,
  B00100100,
  B01000010,
  B00011000,
  B00100100,
  B01000010,
  B00000000,
};

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;

const int STARTING_LOCATION = 1;
int ballX, previousBallX;

long currentMillis, previousMillisSpeed, previousMillisAnimation, previousAnimationBlink, previousMillisSpeedIncrease;

bool gameOver;
bool abilityActive;

const int animationDuration = 2700;
const int animationBlinkDuration = 300;
const int speedIncreaseInterval = 20000;

int movementSpeed;
int abilityGenerationInterval;
bool animationPlaying;
int obstacleChance;
int abilityDuration;

bool gameStarted = false;

const int EASY = 1;
const int NORMAL = 2;
const int HARD = 3;

int obstacleX;
byte difficulty = EASY;

LedControl led = LedControl(dinPin, clockPin, loadPin, 1);
bool ledStatus = 1;

long startTimestamp;
int letterIndex = 0;

void setup() {
  Serial.begin(9600);
  ledBrightness = EEPROM.read(ledBrightnessLocation);
  lcdBrightness = EEPROM.read(lcdBrightnessLocation);
  readStringFromEEPROM(nameLocation, name);
  readStringFromEEPROM(leaderboardNameLocation, highscoreName);
  EEPROM.get(leaderboardScoreLocation, highscore);


  randomSeed(analogRead(A4) * 1000 + analogRead(A5));
  led.shutdown(0, false);
  led.clearDisplay(0);
  lcd.createChar(0, fullBox);
  lcd.createChar(1, emptyBox);
  lcd.begin(16, 2);
  pinMode(pinSW, INPUT_PULLUP);

  analogWrite(pinBrightness, lcdBrightness);
  led.setIntensity(0, ledBrightness);
  lcd.setCursor(0, 0);
  lcd.print("    Welcome,     ");
  lcd.setCursor(0, 1);
  lcd.print("     player!     ");
  delay(4000);
}
void loop() {
  Serial.println(name);
  analogWrite(pinBrightness, lcdBrightness);
  led.setIntensity(0, ledBrightness);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  swState = digitalRead(pinSW);


  Serial.println(menuOption);
  switch (menuOption) {
    case 1:
      lcd.setCursor(0, 0);
      lcd.print("      Menu      ");
      lcd.setCursor(0, 1);
      lcd.print("> Start         ");
      break;
    case 2:
      lcd.setCursor(0, 0);
      lcd.print("      Menu      ");
      lcd.setCursor(0, 1);
      lcd.print("> Settings      ");
      break;
    case 3:
      lcd.setCursor(0, 0);
      lcd.print("      Menu      ");
      lcd.setCursor(0, 1);
      lcd.print("> High Score    ");
      break;
    case 4:
      lcd.setCursor(0, 0);
      lcd.print("      Menu      ");
      lcd.setCursor(0, 1);
      lcd.print("> How to play   ");
      break;
    case 5:
      lcd.setCursor(0, 0);
      lcd.print("      Menu      ");
      lcd.setCursor(0, 1);
      lcd.print("> About         ");
      break;
    case 6:
      lcd.setCursor(0, 0);
      lcd.print("      Menu      ");
      lcd.setCursor(0, 1);
      lcd.print("> Reset         ");
      break;
    case 11:
      if (!gameOver) {
        lcd.setCursor(0, 0);
        lcd.print("     Score:     ");
        lcd.setCursor(0, 1);
        lcd.print(String(String(score) + "            "));
      } else {
        if (lcdPage == 1) {
          lcd.setCursor(0, 0);
          lcd.print("   Game Over!   ");
          lcd.setCursor(0, 1);
          lcd.print(String("Score: " + String(score) + "            "));
        } else {
          if (difficulty != HARD) {
            lcd.setCursor(0, 0);
            lcd.print("Play on hard mod");
            lcd.setCursor(0, 1);
            lcd.print("e for highscore!");
          } else {
            if (newHighscore) {
              lcd.setCursor(0, 0);
              lcd.print("You got a        ");
              lcd.setCursor(0, 1);
              lcd.print("highscore!      ");
            } else {
              lcd.setCursor(0, 0);
              lcd.print("Try again for a  ");
              lcd.setCursor(0, 1);
              lcd.print("highscore!       ");
            }
          }
        }
      }


      break;
    case 21:
      lcd.setCursor(0, 0);
      lcd.print("    Settings    ");
      lcd.setCursor(0, 1);
      lcd.print("> Name          ");
      break;
    case 22:
      lcd.setCursor(0, 0);
      lcd.print("    Settings    ");
      lcd.setCursor(0, 1);
      lcd.print("> Difficulty    ");
      break;
    case 23:
      lcd.setCursor(0, 0);
      lcd.print("    Settings    ");
      lcd.setCursor(0, 1);
      lcd.print("> LCD Brightness");
      break;
    case 24:
      lcd.setCursor(0, 0);
      lcd.print("    Settings    ");
      lcd.setCursor(0, 1);
      lcd.print("> LED Brightness");
      break;
    case 25:
      lcd.setCursor(0, 0);
      lcd.print("    Settings    ");
      lcd.setCursor(0, 1);
      lcd.print("> Sounds        ");
      break;
    case 31:
      lcd.setCursor(0, 0);
      lcd.print("   High Score   ");
      lcd.setCursor(0, 1);
      lcd.print(String(String(highscoreName) + ": " + String(highscore) + "            "));
      break;
    case 41:
      lcd.setCursor(0, 0);
      lcd.print("Avoid obstacles!");
      lcd.setCursor(0, 1);
      lcd.print("Move left/right.");
      break;
    case 51:
      lcd.setCursor(0, 0);
      lcd.print("     About      ");
      lcd.setCursor(0, 1);
      lcd.print("Git:Angel1Ionita");
      break;
    case 61:
      lcd.setCursor(0, 0);
      lcd.print("     Reset      ");
      lcd.setCursor(0, 1);
      lcd.print("Reset succesful!");
      break;
    case 211:
      lcd.setCursor(0, 0);
      if (letterIndex == 0)
        lcd.print("V     Name       ");
      else if (letterIndex == 1)
        lcd.print(" V    Name       ");
      else
        lcd.print("  V   Name       ");
      lcd.setCursor(0, 1);
      lcd.print(name + "             ");

      break;
    case 221:
      lcd.setCursor(0, 0);
      lcd.print("     Difficulty     ");
      lcd.setCursor(0, 1);
      switch (difficulty) {
        case EASY:
          lcd.print("> Easy         ");
          break;
        case NORMAL:
          lcd.print("> Normal       ");
          break;
        case HARD:
          lcd.print("> Hard         ");
          break;
      }
      break;
    case 231:
      Serial.println(lcdBrightness);
      lcd.setCursor(0, 0);
      lcd.print(" LCD Brightness ");
      lcd.setCursor(0, 1);
      indicator = lcdBrightness / 16;
      for (int i = 0; i <= indicator; i++) {
        lcd.setCursor(i, 1);
        lcd.write(byte(0));
      }
      for (int i = indicator + 1; i <= 15; i++) {
        lcd.setCursor(i, 1);
        lcd.write(' ');
      }
      break;
    case 241:
      lcd.setCursor(0, 0);
      lcd.print(" LED Brightness ");
      lcd.setCursor(0, 1);
      indicator = ledBrightness;
      for (int i = 0; i <= indicator; i++) {
        lcd.setCursor(i, 1);
        lcd.write(byte(0));
      }
      for (int i = indicator + 1; i <= 15; i++) {
        lcd.setCursor(i, 1);
        lcd.write(' ');
      }
      for (int row = 0; row < matrixSize; row++)
        led.setRow(0, row, B01111110);
      break;
    case 251:
      lcd.setCursor(0, 0);
      lcd.print("    Sounds      ");
      lcd.setCursor(0, 1);
      lcd.print("-               ");
      break;
    default:
      if (joyMovedX)
        --menuOption;
      else
        menuOption /= 10;
      break;
  }


  if (menuOption == 11) {
    if (!gameOver)
      game();
    else if (swState != lastSwState) {
      if (swState == LOW) {
        if (lcdPage == 1) {
          ++lcdPage;
          if (difficulty == HARD && score > highscore) {
            newHighscore = true;
            highscore = score;
            highscoreName = name;
            EEPROM.put(leaderboardScoreLocation, highscore);
            writeStringToEEPROM(leaderboardNameLocation, highscoreName);
          }
        }

        else {
          menuOption = 1;
          lcdPage = 1;
          led.clearDisplay(0);
        }
      }
    }
  }
  //if (menuOption != 11) {
  else {
    //Vertical movement check
    if (xValue < minThreshold && joyMovedX == false) {
      joyMovedX = true;
      if (menuOption == 231 && lcdBrightness < 255) {
        lcdBrightness += 16;
        EEPROM.update(lcdBrightnessLocation, lcdBrightness);
        Serial.println(lcdBrightness);
      } else if (menuOption == 241 && ledBrightness < 15) {
        ++ledBrightness;
        EEPROM.update(ledBrightnessLocation, ledBrightness);
      } else if (menuOption == 221 && difficulty != EASY) {
        --difficulty;
      } else if (menuOption == 211) {
        if (name[letterIndex] != 'z')
          name[letterIndex]++;
        else
          name[letterIndex] = 'a';
      } else if (menuOption % 10 > 1)
        --menuOption;
    }
    if (xValue > maxThreshold && joyMovedX == false) {
      joyMovedX = true;
      if (menuOption == 231 && lcdBrightness > 15) {
        lcdBrightness -= 16;
        //EEPROM.update(lcdBrightnessLocation, lcdBrightness);
        Serial.println(lcdBrightness);
      } else if (menuOption == 241 && ledBrightness > 0) {
        --ledBrightness;
        //EEPROM.update(ledBrightnessLocation, ledBrightness);
      } else if (menuOption == 221 && difficulty != HARD) {
        ++difficulty;
      } else if (menuOption == 211) {
        if (name[letterIndex] != 'a')
          name[letterIndex]--;
        else
          name[letterIndex] = 'z';
      } else
        ++menuOption;
    }
    if (xValue > minThreshold && xValue < maxThreshold) {
      joyMovedX = false;
    }

    // Horizontal movement check
    if (yValue < minThreshold && joyMovedY == false) {
      joyMovedY = true;
    }
    if (yValue > maxThreshold && joyMovedY == false) {
      joyMovedY = true;
      led.clearDisplay(0);
      if (menuOption == 211) {
        Serial.println("Done!");
        writeStringToEEPROM(nameLocation, name);
      }
      if (menuOption == 231) {
        EEPROM.update(lcdBrightnessLocation, lcdBrightness);
      }
      if (menuOption == 241) {
        EEPROM.update(ledBrightnessLocation, ledBrightness);
      }

      if (menuOption / 10 != 0)
        menuOption /= 10;
    }
    if (yValue > minThreshold && yValue < maxThreshold) {
      joyMovedY = false;
    }


    if (swState != lastSwState) {
      if (swState == LOW) {

        if (menuOption == 211) {
          if (letterIndex == 2)
            letterIndex = 0;
          else
            ++letterIndex;
        } else {
          menuOption *= 10;
          ++menuOption;
          if (menuOption == 11) {
            setupGame();
          }
          if (menuOption == 211) {
            letterIndex = 0;
          }
          if (menuOption == 61) {
            resetEEPROM();
            ledBrightness = EEPROM.read(ledBrightnessLocation);
            lcdBrightness = EEPROM.read(lcdBrightnessLocation);
            readStringFromEEPROM(nameLocation, name);
            readStringFromEEPROM(leaderboardNameLocation, highscoreName);
            EEPROM.get(leaderboardScoreLocation, highscore);
          }
        }
      }
    }
  }
  lastSwState = swState;
}

void writeStringToEEPROM(int location, String str) {
  for (int i = 0; i < 3; i++)
    EEPROM.update(location + i, str[i]);
}
void readStringFromEEPROM(int location, String &name) {
  for (int i = 0; i < 3; i++)
    name[i] = EEPROM.read(location + i);
}


void printMatrix() {
  led.clearDisplay(0);
  for (int row = 0; row < matrixSize; row++)
    led.setRow(0, row, displayMatrix[row]);
}

byte generateRandomNumber() {
  return random(1, matrixRightLimit);
}

void scrollScreen(int nr) {
  for (int i = matrixRightLimit; i > 0; i--) {
    displayMatrix[i] = displayMatrix[i - 1];
  }
  displayMatrix[0] = B00000000;
  if (random(obstacleChance) == 0)
    bitSet(displayMatrix[0], matrixRightLimit - nr);
  if (wallHit()) {
    gameOver = 1;
  } else {
    bitSet(displayMatrix[matrixRightLimit], matrixRightLimit - ballX);
  }
  onMatrixUpdate();
}

void onPointMove() {

  if (wallHit()) {
    gameOver = 1;
  } else {
    bitSet(displayMatrix[matrixRightLimit], matrixRightLimit - ballX);
    bitClear(displayMatrix[matrixRightLimit], matrixRightLimit - previousBallX);
    previousBallX = ballX;
  }
  onMatrixUpdate();
}

void onMatrixUpdate() {
  if (gameOver)
    copyMatrix(deathScreen);
  printMatrix();
}

bool wallHit() {
  return bitRead(displayMatrix[matrixRightLimit], matrixRightLimit - ballX);
}

void copyMatrix(byte matrix[8]) {
  for (int row = 0; row < matrixSize; row++)
    displayMatrix[row] = matrix[row];
}

void checkMovement() {
  if (yValue < minThreshold && joyMovedY == false) {
    joyMovedY = true;
    if (ballX < matrixRightLimit - 1) {
      ++ballX;
      onPointMove();
    }
  }
  if (yValue > maxThreshold && joyMovedY == false) {
    joyMovedY = true;
    if (ballX > 1) {
      --ballX;
      onPointMove();
    }
  }
  if (yValue > minThreshold && yValue < maxThreshold) {
    joyMovedY = false;
  }
}

void setDifficulty(int difficulty) {
  switch (difficulty) {

    case EASY:
      obstacleChance = 3;
      movementSpeed = 400;
      abilityDuration = 7000;
      break;
    case NORMAL:
      obstacleChance = 2;
      movementSpeed = 400;
      abilityDuration = 10000;
      break;
    case HARD:
      obstacleChance = 1;
      movementSpeed = 400;
      abilityDuration = 13000;
      break;

    default:
      Serial.print("Sensor error");
  }
}

void setupGame() {
  for (int i = 0; i < matrixSize; i++)
    displayMatrix[i] = B00000000;
  newHighscore = false;
  gameStarted = true;
  gameOver = false;
  startTimestamp = millis();
  score = 0;

  previousMillisSpeed = 0;
  previousMillisAnimation = 0;
  previousAnimationBlink = 0;
  previousMillisSpeedIncrease = 0;

  ballX = STARTING_LOCATION;
  previousBallX = STARTING_LOCATION;

  abilityGenerationInterval = random(15, 26) * 1000;
  animationPlaying = false;
  abilityActive = false;
  setDifficulty(difficulty);
}

void game() {
  currentMillis = millis() - startTimestamp;

  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  swState = digitalRead(pinSW);
  if (!gameOver) {
    if (animationPlaying) {
      if (currentMillis - previousAnimationBlink >= animationBlinkDuration) {
        previousAnimationBlink = currentMillis;
        if (ledStatus) {
          copyMatrix(speedUp);
          printMatrix();
        } else
          led.clearDisplay(0);
        ledStatus = !ledStatus;
      }

      if (currentMillis - previousMillisAnimation >= animationDuration) {
        previousMillisAnimation = currentMillis;
        animationPlaying = 0;
        for (int row = 0; row < matrixSize; row++) {
          displayMatrix[row] = B00000000;
          ballX = 1;
          previousBallX = 1;
        }
        abilityActive = 1;
        movementSpeed /= 2;
      }
    }
    if (!animationPlaying)
      if (currentMillis - previousMillisAnimation >= abilityGenerationInterval) {
        previousMillisAnimation = currentMillis;
        abilityGenerationInterval = random(15, 26) * 1000;
        animationPlaying = 1;
      }
    if (!animationPlaying) {
      if (abilityActive == 1)
        if (currentMillis - previousMillisAnimation >= abilityDuration) {
          previousMillisAnimation = currentMillis;
          abilityActive = 0;
          movementSpeed *= 2;
        }
      if (currentMillis - previousMillisSpeedIncrease >= speedIncreaseInterval) {
        previousMillisSpeedIncrease = currentMillis;
        if (movementSpeed > 150)
          movementSpeed -= 20;
      }
      checkMovement();
      if (currentMillis - previousMillisSpeed >= movementSpeed) {
        previousMillisSpeed = currentMillis;
        obstacleX = random(1, matrixRightLimit);
        scrollScreen(obstacleX);
        score++;
      }
    }
  }
}

void resetEEPROM() {
  EEPROM.update(lcdBrightnessLocation, 255);
  EEPROM.update(ledBrightnessLocation, 15);
  writeStringToEEPROM(nameLocation, "aaa");
  writeStringToEEPROM(leaderboardNameLocation, "aaa");
  EEPROM.put(leaderboardScoreLocation, 0);
}