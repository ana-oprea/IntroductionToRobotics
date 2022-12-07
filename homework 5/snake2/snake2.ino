#include <LiquidCrystal.h>
#include "LedControl.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <EEPROM.h>

const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 13; // 6
const byte d6 = 5;
const byte d7 = 4;
const byte contrast = 3;
const byte lcdBrightnessPin = 6;

const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;
const byte matrixSize = 8;

const int pinSW = 2; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected t Y output

byte isButtonPressed = LOW;
byte lastButtonState;
const unsigned int debounceDelay = 50;
unsigned long lastDebounceTime;
unsigned long releaseTime;
unsigned long pressedTime;

int xValue;
int yValue;
int swValue;
bool joyMovedX = false;
bool joyMovedY = false;
bool enterSubMenu = false;

const int minThreshold = 400;
const int maxThreshold = 600;

byte currentFrame = 0;
byte currentFrameSubMenu = 0;
bool updateFrameSubMenu = false;
bool updateFrame = false;
bool mainMenu = true;
byte currentHighScore;
char name[3] = "___";
int letter;
int pos;
bool intro = true;

struct highscore{
  int score = 0;
  char name[4] = "___";
};

int highscores[] = {13, 12, 10, 5, 3};
String highScoreNames[] = {"ANA", "MAE", "REX", "GAB", "TEO"};
int difficulty = 1;
int lcdContrast = 120;
int lcdBrightness = 255;
int matrixBrightness = 3;
bool sounds = false;

char gameName[] = "Snake";
char madeByText[] = "Made By: Oprea Ana-Maria";
char githubText[] = "Github: ana-oprea";
int positionInText = 0;

int row, col;
bool gameStarted;
unsigned long blinkDuration = 500;
unsigned long lastBlinkTime;
bool shouldGenerateFood;
int snakeRow = 0;
int snakeCol = 4;
int lives = 1;
int score = 0;

highscore highScores[5];
int highScoresAddress[] = {0, 16, 32, 48, 64};
int lcdContrastAddress = 84;
int lcdBrightnessAddress = 85;
int matrixBrightnessAddress = 86;
int difficultyAdress = 87;
int soundAdress = 88;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
LedControl lc = LedControl(dinPin, clockPin, loadPin,1);//DIN, CLK, LOAD, No.DRIVER;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(pinSW, INPUT_PULLUP);
  Serial.begin(9600);
  // Print a message to the LCD.
  lcd.setCursor(0,0);
  lcd.print(">");
  lcd.setCursor(1, 0);
  lcd.print("Start Game");
  lcd.setCursor(1, 1);
  lcd.print("Settings");
  analogWrite(contrast, lcdContrast);
  analogWrite(lcdBrightnessPin, lcdBrightness);

  lc.shutdown(0, false);// turn off power saving,enables display
  lc.setIntensity(0, matrixBrightness);// sets brightness(0~15 possible values)
  lc.clearDisplay(0);// clear screen

  srand(time(0));
}

void loop() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  swValue = digitalRead(pinSW);

  // initializeEeprom();

  if (intro == true){
    if(swValue == LOW && lastButtonState == HIGH){
      lastDebounceTime = millis();
    }
    else if(swValue == HIGH && lastButtonState == LOW){
      releaseTime = millis();
      pressedTime = releaseTime - lastDebounceTime;

      if (pressedTime > debounceDelay){
        bool startMenu = true;
        for (int i = 0; i < 3; i++){
          if (name[i] == '_')
            startMenu = false;
        }
        if (startMenu == true){
          intro = false;
          introSequence();
        }
        }  
      }
    lastButtonState = swValue;

  lcd.clear();
  lcd.print("ENTER NAME");
  lcd.setCursor(2, 1);
  for (int i = 0; i < 3; i++){
    lcd.print(name[i]);
  }
  delay(25);
  enterName();
  } 
  else{
    buttonListener();

    if (mainMenu == true){
      snakeRow = 0;
      snakeCol = 4;
      lives = 1;
      score = 0;
      moveThroughFrames();
      if(updateFrame == true)
        showFrame();
    }else{
      moveThroughFrames();
      showSubDisplay();
    }
  }
}

void introSequence(){
  lcd.clear();
  lcd.print("Welcome to...");
  lcd.setCursor(0, 1);
  lcd.print("SNAKE");
  delay(1000);
  mainMenu = true;
  updateFrame = true;
}

void buttonListener(){
  if(swValue == LOW && lastButtonState == HIGH){
    lastDebounceTime = millis();
  }
  else if(swValue == HIGH && lastButtonState == LOW){
    releaseTime = millis();
    pressedTime = releaseTime - lastDebounceTime;
    if (pressedTime > debounceDelay){
      Serial.println(lives);
       if (lives == 0){
        Serial.println("pressed");
        Serial.println(mainMenu);
        lives = 1;
        updateFrame = true; 
        mainMenu = true; 
        lcd.clear();
        showFrame();
        }
      else{
        enterSubMenu = true;
        updateFrameSubMenu = true;
        mainMenu = !mainMenu;
        lcd.clear();
        if (mainMenu == true){
          updateFrame == true;
          showFrame();
        }
      }  
    }}
  lastButtonState = swValue;
}

void startGame(){
  if (shouldGenerateFood == true){
    generateFood();
    shouldGenerateFood = false;
  }
  blinkFood();
  drawSnake();
  moveSnake();
  checkCollision();
}

void checkCollision(){
  if (snakeRow == row && snakeCol == col){
    lc.setLed(0, row, col, false); // turns on LED at col, row
    score++;
    shouldGenerateFood = true;
  }
  if ((snakeRow == -1 || snakeRow == 8 || snakeCol == -1 || snakeCol == 8) && lives > 0){
    lives--;
    Serial.println(lives);
    Serial.println("---");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game Over");
    lcd.setCursor(0, 1);
    lcd.print("Score: ");
    lcd.print(score);
    delay(1000);
    lc.setLed(0, row, col, false); // turns on LED at col, row
    lc.setLed(0, snakeRow, snakeCol, false); // turns on LED at col, row
    checkHighScore();
    // mainMenu = true;
  }
}

void checkHighScore(){
  bool top = false;
  Serial.println("HIGHSCORES");
  for (int i = 0; i < 5; i++){
    if (score > highscores[i]){
      for (int j = i + 1; j < 5; j++){
        highscores[j] = highscores[i];
      }
      highscores[i] = score;
      highScoreNames[i] = name;
      lcd.setCursor(0, 0);
      lcd.print("YOU'RE IN TOP 5");
      top = true;
      break;
    }
  }
  if (top == false){
    lcd.setCursor(0, 0);
    lcd.print("TRY AGAIN");
  }
}

void enterName(){
  xValue = analogRead(pinX);
  if(xValue < minThreshold && joyMovedX == false) {
    if(pos - 1 < 0)
      pos = 0;
    else{
      pos -= 1;
    }
    joyMovedX = true;
  }else{
    if(xValue > maxThreshold && joyMovedX == false) {
      if(pos + 1 > 3)
        pos = 3;
      else{
        pos += 1;
      }
      joyMovedX = true;
    }
  }

  if(xValue > minThreshold && xValue < maxThreshold){
    joyMovedX = false;
  }

  yValue = analogRead(pinY);
  if(yValue < minThreshold && joyMovedY == false) {
    if(letter - 1 < 0)
      letter = 0;
    else{
      letter -= 1;
    }
    joyMovedY = true;
  }else{
    if(yValue > maxThreshold && joyMovedY == false) {
      if(letter + 1 > 26)
        letter = 26;
      else{
        letter += 1;
      }
      joyMovedY = true;
    }}
  if(yValue > minThreshold && yValue < maxThreshold){
    joyMovedY = false;
  }

  char character;
  if(joyMovedY == true) {
    if(letter == 26) {
      character = ' ';
    }else{
      character = 'A' + letter;
    }
    name[pos] = character;
  }
}

void moveSnake(){
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  swValue = digitalRead(pinSW);

  if (xValue > maxThreshold && joyMovedX == false){
    if(snakeCol + 1 <= 8){
      lc.setLed(0, snakeRow, snakeCol, false);
      snakeCol++;
    }
    joyMovedX = true;
  }
  else if (xValue < minThreshold && joyMovedX == false){
    if(snakeCol - 1 >= -1){
      lc.setLed(0, snakeRow, snakeCol, false);
      snakeCol--;
    }
    joyMovedX = true;
  }

  if(xValue > minThreshold && xValue < maxThreshold){
    joyMovedX = false;
  }

  if (yValue > maxThreshold && joyMovedY == false){
    if(snakeRow + 1 <= 8){
      lc.setLed(0, snakeRow, snakeCol, false);
      snakeRow++;
    }
    joyMovedY = true;
  }
  else if (yValue < minThreshold && joyMovedY == false){
    if(snakeRow - 1 >= -1){
      lc.setLed(0, snakeRow, snakeCol, false);
      snakeRow--;
    }
    joyMovedY = true;
  }

  if(yValue > minThreshold && yValue < maxThreshold){
    joyMovedY = false;
  }
}

void drawSnake(){
  lc.setLed(0, snakeRow, snakeCol, true);  
}

void generateFood(){
  row = rand() % 8;
  col = rand() % 8;

  while (row == snakeRow && col == snakeCol){
    row = rand() % 8;
    col = rand() % 8;
  }
  Serial.println(row);
  Serial.println(col);
  Serial.println("-----");
}

void blinkFood(){
  if (millis() - lastBlinkTime > blinkDuration) {
    lastBlinkTime = millis();
    lc.setLed(0, row, col, true); // turns on LED at col, row
  }
  else{
    lc.setLed(0, row, col, false);
  }  
}

void showSubDisplay(){
  switch(currentFrame){
    case 0:
      if (lives > 0){
        lcd.setCursor(0, 0);
        lcd.print("Lives: ");
        lcd.print(lives);
        lcd.setCursor(0, 1);
        lcd.print("Score: ");
        lcd.print(score);
        startGame();
      }
      break;
    case 1:
      showSettingsSubMenu();    
      break;
    case 2:
      showHighScore();
      break;
    case 3:
      showHowToPlay();
      break;
    case 4:
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("Snake");
      showAboutInfo();
      break;
    
  }
}

void showHighScore(){
  lcd.setCursor(0, 0);
  lcd.print("HIGHSCORES");
  lcd.setCursor(0, 1);
  lcd.print(currentHighScore + 1);
  lcd.print(".");
  lcd.print(highScoreNames[currentHighScore]);
  lcd.print(" ");
  lcd.print(highscores[currentHighScore]);

  yValue = analogRead(pinY);
  if (yValue > maxThreshold && joyMovedY == false){
      Serial.println("HERE");
      if(currentHighScore == 4)
        currentHighScore = 0;
      else
        currentHighScore++;
      joyMovedY = true;
      // updateFrame = true;
      lcd.clear();
    }
    else if (yValue < minThreshold && joyMovedY == false){
      if(currentHighScore == 0)
        currentHighScore = 4;
      else
        currentHighScore--;
      joyMovedY = true;
      // updateFrame = true;
      lcd.clear();
    }

    if(yValue > minThreshold && yValue < maxThreshold){
      joyMovedY = false;
    }
}

void showHowToPlay(){
  lcd.print("You control a snake, which roams around on a bordered plane, picking up food, trying to avoid hitting its own tail or the edges of the playing area. Each time the snake eats a piece of food, its tail grows longer.");
}

void moveThroughFrames(){
  if (mainMenu == true){
    if (yValue > maxThreshold && joyMovedY == false){
      if(currentFrame == 4)
        currentFrame = 0;
      else
        currentFrame++;
      joyMovedY = true;
      updateFrame = true;
      // lcd.clear();
    }
    else if (yValue < minThreshold && joyMovedY == false){
      if(currentFrame == 0)
        currentFrame = 4;
      else
        currentFrame--;
      joyMovedY = true;
      updateFrame = true;
      // lcd.clear();
    }

    if(yValue > minThreshold && yValue < maxThreshold){
      joyMovedY = false;
    }
  }
  else {
    if (yValue > maxThreshold && joyMovedY == false){
      if(currentFrameSubMenu == 4)
        currentFrameSubMenu = 0;
      else
        currentFrameSubMenu++;
      joyMovedY = true;
      updateFrameSubMenu = true;
      // lcd.clear();
    }
    else if (yValue < minThreshold && joyMovedY == false){
      if(currentFrameSubMenu == 0)
        currentFrameSubMenu = 4;
      else
        currentFrameSubMenu--;
      joyMovedY = true;
      updateFrameSubMenu = true;
      // lcd.clear();
    }

    if(yValue > minThreshold && yValue < maxThreshold){
      joyMovedY = false;
    }
  }
}

void showSettingsSubMenu(){
  // moveThroughFrames();
  switch(currentFrameSubMenu){
    case 0:
      int diff;
      if (updateFrameSubMenu == true){
        EEPROM.get(difficultyAdress, diff);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(">");
        lcd.setCursor(1, 0);
        lcd.print("Difficulty");
        lcd.print(diff);
        lcd.setCursor(1, 1);
        lcd.print("LCD Co");
        updateFrameSubMenu = false;
      }

      xValue = analogRead(pinX);
      
      if (xValue > maxThreshold && joyMovedX == false){
        if(diff < 3){
          diff++;
          EEPROM.put(difficultyAdress, diff);
        }
        joyMovedX = true;
        updateFrameSubMenu = true;
        // lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Difficulty");
        lcd.print(diff);
      }
      else if (xValue < minThreshold && joyMovedX == false){
        if(diff > 0){
          diff--;
          EEPROM.put(difficultyAdress, diff);
        }
        joyMovedX = true;
        updateFrameSubMenu = true;
        // lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Difficulty");
        lcd.print(diff);
      }

      if(xValue > minThreshold && xValue < maxThreshold){
        joyMovedX = false;
      }
      break;
    case 1:
      if (updateFrameSubMenu == true){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(">");
        lcd.setCursor(1, 0);
        lcd.print("LCD Co");
        lcd.print(lcdContrast);
        lcd.setCursor(1, 1);
        lcd.print("LCD Bri");
        updateFrameSubMenu = false;
      }
      xValue = analogRead(pinX);
      
      if (xValue > maxThreshold && joyMovedX == false){
        if(lcdContrast < 180)
          lcdContrast += 40;
        joyMovedX = true;
        updateFrameSubMenu = true;
        // lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("LCD Co");
        lcd.print(lcdContrast);
      }
      else if (xValue < minThreshold && joyMovedX == false){
        if(lcdContrast > 0)
          lcdContrast -= 40;
        joyMovedX = true;
        updateFrameSubMenu = true;
        // lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("LCD Co");
        lcd.print(lcdContrast);
      }

      if(xValue > minThreshold && xValue < maxThreshold){
        joyMovedX = false;
      }
      analogWrite(contrast, lcdContrast);
      
      break;
    case 2:
      if (updateFrameSubMenu == true){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(">");
        lcd.setCursor(1, 0);
        lcd.print("LCD Bri");
        lcd.print(lcdBrightness);
        lcd.setCursor(1, 1);
        lcd.print("Matrix Bri");
        updateFrameSubMenu = false;
        }
        xValue = analogRead(pinX);
      
        if (xValue > maxThreshold && joyMovedX == false){
          if(lcdBrightness < 255)
            lcdBrightness += 25;
          joyMovedX = true;
          updateFrameSubMenu = true;
          // lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("LCD Bri");
          lcd.print(lcdBrightness);
        }
        else if (xValue < minThreshold && joyMovedX == false){
          if(lcdBrightness - 25 > 0)
            lcdBrightness -= 25;
          joyMovedX = true;
          updateFrameSubMenu = true;
          // lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("LCD Bri");
          lcd.print(lcdBrightness);
        }

        if(xValue > minThreshold && xValue < maxThreshold){
          joyMovedX = false;
        }
        analogWrite(lcdBrightnessPin, lcdBrightness);
      
      break;
    case 3:
      if (updateFrameSubMenu == true){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(">");
        lcd.setCursor(1, 0);
        lcd.print("Matrix Bri");
        lcd.print(matrixBrightness);
        lcd.setCursor(1, 1);
        lcd.print("Sounds");
        updateFrameSubMenu = false;
      }

      xValue = analogRead(pinX);
      
      if (xValue > maxThreshold && joyMovedX == false){
        if(matrixBrightness + 3 <= 15)
          matrixBrightness += 3;
        joyMovedX = true;
        updateFrameSubMenu = true;
        // lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Matrix Bri");
        lcd.print(matrixBrightness);
      }
      else if (xValue < minThreshold && joyMovedX == false){
        if(matrixBrightness - 3 > 0)
          matrixBrightness -= 3;
        joyMovedX = true;
        updateFrameSubMenu = true;
        // lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Matrix Bri");
        lcd.print(matrixBrightness);
      }

      if(xValue > minThreshold && xValue < maxThreshold){
        joyMovedX = false;
      }
      
      lc.setIntensity(0, matrixBrightness);
      break;
    case 4:
      if (updateFrameSubMenu == true){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(">");
        lcd.setCursor(1, 0);
        lcd.print("Sounds");
        if(sounds == true)
          lcd.print("ON");
        else
          lcd.print("OFF");
        lcd.setCursor(1, 1);
        lcd.print("Difficulty");
        updateFrameSubMenu = false;
      }

      xValue = analogRead(pinX);
      
      if (xValue > maxThreshold && joyMovedX == false){
        if(sounds == false)
          sounds = true;
        joyMovedX = true;
        updateFrameSubMenu = true;
        // lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Sounds");
        if(sounds == true)
          lcd.print("ON");
        else
          lcd.print("OFF");
      }
      else if (xValue < minThreshold && joyMovedX == false){
        if(sounds == true)
          sounds = false;
        joyMovedX = true;
        updateFrameSubMenu = true;
        // lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Sounds");
        if(sounds == true)
          lcd.print("ON");
        else
          lcd.print("OFF");
      }

      if(xValue > minThreshold && xValue < maxThreshold){
        joyMovedX = false;
      }
      
      break;
  }
}

void showAboutInfo(){
  if (mainMenu == false){
  lcd.setCursor(5, 0);
  lcd.print(madeByText);
  lcd.setCursor(5, 1);
  lcd.print(githubText);
  for (int pos = 0; pos < 16 + strlen(madeByText); pos++){
    lcd.scrollDisplayLeft();
    delay(200);
  }
  }
}

void showFrame(){
  switch(currentFrame){
    case 0:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(">");
      lcd.setCursor(1, 0);
      lcd.print("Start Game");
      lcd.setCursor(1, 1);
      lcd.print("Settings");
      updateFrame = false;
      break;
    case 1:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(">");
      lcd.setCursor(1, 0);
      lcd.print("Settings");
      lcd.setCursor(1, 1);
      lcd.print("High Score");
      updateFrame = false;
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(">");
      lcd.setCursor(1, 0);
      lcd.print("High Score");
      lcd.setCursor(1, 1);
      lcd.print("How to Play");
      updateFrame = false;
      break;
    case 3:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(">");
      lcd.setCursor(1, 0);
      lcd.print("How To Play");
      lcd.setCursor(1, 1);
      lcd.print("About");
      updateFrame = false;
      break;
    case 4:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(">");
      lcd.setCursor(1, 0);
      lcd.print("About");
      lcd.setCursor(1, 1);
      lcd.print("Start Game");
      updateFrame = false;
      break;
  }
}

void initializeEeprom() {
  for (int i = 0; i < 5; i++) {
    EEPROM.put(highScoresAddress[i], highScores[i]);
  }
  EEPROM.put(lcdContrastAddress, lcdContrast);
  EEPROM.put(lcdBrightnessAddress, lcdBrightnessPin);
  EEPROM.put(matrixBrightnessAddress, matrixBrightness);
  EEPROM.put(difficultyAdress, difficulty);
  EEPROM.put(soundAdress, sounds);
}