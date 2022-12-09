#include <LiquidCrystal.h>
#include "LedControl.h"
#include <EEPROM.h>
 
struct Point {
  byte x;
  byte y;
};
const int pinSW = 2;
const int pinX = A0;
const int pinY = A1;
const int pinBrightness = 3;
 
const byte RS = 9;
const byte enable = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);
 
const int minThreshold = 400;
const int maxThreshold = 600;
 
//temporary 
String name="Default";
 
int xValue;
int yValue;
byte swState;
byte lastSwState;
 
const int matrixSize = 8;
bool matrix[matrixSize][matrixSize]={
  {0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0, 0,}
};
int joyMovedX;
int joyMovedY;
 
int menuOption = 1;
 
int lcdBrightness;
int ledBrightness;
 
const int lcdBrightnessLocation = 0;
const int ledBrightnessLocation = 1;
 
int indicator;
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
 
 
const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
 
LedControl led = LedControl(dinPin, clockPin, loadPin, 1);  //DIN, CLK, LOAD,No. DRIVER
 
Point currentPos, previousPos, foodPos;
 
void setup() {
 
  ledBrightness = EEPROM.read(ledBrightnessLocation);
  lcdBrightness = EEPROM.read(lcdBrightnessLocation);
  Serial.begin(9600);
  led.shutdown(0, false);  
  led.clearDisplay(0);  // clear screen
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
    case 11:
      lcd.setCursor(0, 0);
      lcd.print("     Score:     ");
      lcd.setCursor(0, 1);
      lcd.print(score);
      lcd.setCursor(2, 1);
      lcd.print("            ");
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
      lcd.print("> LCD Brightness");
      break;
    case 23:
      lcd.setCursor(0, 0);
      lcd.print("    Settings    ");
      lcd.setCursor(0, 1);
      lcd.print("> LED Brightness");
      break;
    case 24:
      lcd.setCursor(0, 0);
      lcd.print("    Settings    ");
      lcd.setCursor(0, 1);
      lcd.print("> Sounds        ");
      break;
    case 31:
      lcd.setCursor(0, 0);
      lcd.print("   High Score   ");
      lcd.setCursor(0, 1);
      lcd.print("Coming soon     ");
      break;
    case 41:
      lcd.setCursor(0, 0);
      lcd.print("  How to play   ");
      lcd.setCursor(0, 1);
      lcd.print("Coming soon     ");
      break;
    case 51:
      lcd.setCursor(0, 0);
      lcd.print("     About      ");
      lcd.setCursor(0, 1);
      lcd.print("Git:Angel1Ionita");
      break;
    case 211:
      lcd.setCursor(0, 0);
      lcd.print("      Name      ");
      lcd.setCursor(0, 1);
      //lcd.print("Coming soon     ");
      if (Serial.available()>0)
        {
        name=Serial.readString();
     }
      else
        {
        lcd.print(name);}      
      break;      
    case 221:
      lcd.setCursor(0, 0);
      lcd.print(" LCD Brightness ");
      lcd.setCursor(0, 1);
      //indicator = map(lcdBrightness, 15, 255, 0, 15);
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
    case 231:
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
      break;
    case 241:
      lcd.setCursor(0, 0);
      lcd.print("    Sounds      ");
      lcd.setCursor(0, 1);
      lcd.print("Coming soon     ");
      break;      
    default:
      if (joyMovedX)
        --menuOption;
      else
        menuOption /= 10;
      break;
  }
 
  if (menuOption != 11) {
    //Vertical movement check
    if (xValue < minThreshold && joyMovedX == false) {
      joyMovedX = true;
      if (menuOption == 221 && lcdBrightness < 255) {
        lcdBrightness += 16;
        EEPROM.update(lcdBrightnessLocation, lcdBrightness);
        Serial.println(lcdBrightness);
      } else if (menuOption == 231 && ledBrightness < 15) {
        ++ledBrightness;
        EEPROM.update(ledBrightnessLocation, ledBrightness);
      } else if (menuOption % 10 > 1)
        --menuOption;
    }
    if (xValue > maxThreshold && joyMovedX == false) {
      joyMovedX = true;
      if (menuOption == 221 && lcdBrightness > 15) {
        lcdBrightness -= 16;
        EEPROM.update(lcdBrightnessLocation, lcdBrightness);
        Serial.println(lcdBrightness);
      } else if (menuOption == 231 && ledBrightness > 0) {
        --ledBrightness;
        EEPROM.update(ledBrightnessLocation, ledBrightness);
      } else
        ++menuOption;
    }
    if (xValue > minThreshold && xValue < maxThreshold) {
      joyMovedX = false;
    }
 
    // Horizontal movement check
    if (yValue < minThreshold && joyMovedY == false) {
      joyMovedY = true;
      //if (menuOption % 10 > 1)
      //  --menuOption;
    }
    if (yValue > maxThreshold && joyMovedY == false) {
      joyMovedY = true;
      if (menuOption / 10 != 0)
        menuOption /= 10;
    }
    if (yValue > minThreshold && yValue < maxThreshold) {
      joyMovedY = false;
    }
 
 
    if (swState != lastSwState) {
      if (swState == LOW) {
        menuOption *= 10;
        ++menuOption;
        if (menuOption == 11) {
          initGame();
        }
      }
    }
 
    lastSwState = swState;
  } else {
    updateGame();
  }
}
 
void displayMatrix() {
  led.clearDisplay(0);
  for (int row = 0; row < matrixSize; row++)
    for (int col = 0; col < matrixSize; col++)
      led.setLed(0, row, col, matrix[row][col]);
}
 
void initGame() {
  score = 0;
  foodPos.x = random(matrixSize);
  foodPos.y = random(matrixSize);
  currentPos.x = 0;
  currentPos.y = 0;
  previousPos.x = 0;
  previousPos.y = 0;
  displayMatrix();
}
void updateCoords() {
  if (foodPos.x == currentPos.x && foodPos.y == currentPos.y) {
    ++score;
    do {
      matrix[foodPos.x][foodPos.y] = 0;
      foodPos.x = random(matrixSize);
      foodPos.y = random(matrixSize);
      matrix[foodPos.x][foodPos.y] = 1;
    } while (foodPos.x == currentPos.x && foodPos.y == currentPos.y);
  }
 
  matrix[previousPos.x][previousPos.y] = 0;
  matrix[currentPos.x][currentPos.y] = 1;
  previousPos.x = currentPos.x;
  previousPos.y = currentPos.y;
}
 
void updateGame() {
  //Vertical movement check
  if (xValue < minThreshold && joyMovedX == false) {
    joyMovedX = true;
    if (currentPos.y < 8) {
      ++currentPos.y;
      updateCoords();
      displayMatrix();
    }
  }
  if (xValue > maxThreshold && joyMovedX == false) {
    joyMovedX = true;
    if (currentPos.y > 0) {
      --currentPos.y;
      updateCoords();
      displayMatrix();
    }
  }
  if (xValue > minThreshold && xValue < maxThreshold) {
    joyMovedX = false;
  }
 
  // Horizontal movement check
  if (yValue < minThreshold && joyMovedY == false) {
    joyMovedY = true;
    if (currentPos.x < 8) {
      ++currentPos.x;
      updateCoords();
      displayMatrix();
    }
  }
  if (yValue > maxThreshold && joyMovedY == false) {
    joyMovedY = true;
    if (currentPos.x > 0) {
      --currentPos.x;
      updateCoords();
      displayMatrix();
    }
  }
  if (yValue > minThreshold && yValue < maxThreshold) {
    joyMovedY = false;
  }
}