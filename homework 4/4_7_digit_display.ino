const int pinX = A0;
const int pinY = A1;
const int pinSW = 2;

int state = 1;

int xValue;
int yValue;
byte swValue;
bool joyMovedX = false;
bool joyMovedY = false;
byte lastButtonState;

const int minThreshold = 400;
const int maxThreshold = 600;

unsigned long lastDebounceTime;
unsigned long releaseTime;
unsigned long pressedTime;
unsigned int debounceDelay = 50;
unsigned int debounceDelayLong = 2000;

const int latchPin = 11; // STCP to 12 on Shift Register
const int clockPin = 10; // SHCP to 11 on Shift Register
const int dataPin = 12; // DS to 14 on Shift Register

const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

const byte regSize = 8; // 1 byte aka 8 bits

int displayDigits[] = {
  segD1, segD2, segD3, segD4
};
const int displayCount = 4;
const int encodingsNumber = 16;

int currentDisplay = 3;

int displayValue[] = {
  0, 0, 0, 0
};

unsigned long blinkDuration = 500;
unsigned long lastBlinkTime;

int byteEncodings[encodingsNumber] = {
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
  B10001110  // F
};

int byteEncodingsWithDP[encodingsNumber] = {
//A B C D E F G DP 
  B11111101, // 0 
  B01100001, // 1
  B11011011, // 2
  B11110011, // 3
  B01100111, // 4
  B10110111, // 5
  B10111111, // 6
  B11100001, // 7
  B11111111, // 8
  B11110111, // 9
  B11101111, // A
  B00111111, // b
  B10011101, // C
  B01111011, // d
  B10011111, // E
  B10001111  // F
};

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(pinSW, INPUT_PULLUP);

  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }  
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < displayCount; i++){
    writeNumber(3-i, displayValue[i]);
  }

  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  swValue = digitalRead(pinSW);

  buttonListener();

  if (state == 1){
    moveThroughDisplays();
   }
  else{
    moveThroughDigits();
  }
} 

void buttonListener(){
  if(swValue == LOW && lastButtonState == HIGH){
    lastDebounceTime = millis();
  }
  else if(swValue == HIGH && lastButtonState == LOW){
    releaseTime = millis();
    pressedTime = releaseTime - lastDebounceTime;
    if (pressedTime > debounceDelay && pressedTime < debounceDelayLong){
      if (state == 1){
          state = 2;
        }
        else{
          state = 1;
        }
    }
    else if(pressedTime > debounceDelayLong && state == 1){
      for(int i = 0; i < displayCount; i++){
        displayValue[i] = 0;
        writeNumber(3-i, displayValue[i]);
        currentDisplay = 3;
      }
    }
  }

  lastButtonState = swValue;
}

void writeReg(int digit) {
    // ST_CP LOW to keep LEDs from changing while reading serial data
    digitalWrite(latchPin, LOW);
    // Shift out the bits
    shiftOut(dataPin, clockPin, MSBFIRST, digit);
    /* ST_CP on Rising to move the data from shift register
     * to storage register, making the bits available for output.
     */
    digitalWrite(latchPin, HIGH);
}

void activateDisplay(int displayNumber) { 
  // first, disable all the display digits
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  // then enable only the digit you want to use now
  digitalWrite(displayDigits[displayNumber], LOW);
}

void writeNumber(int display, int number) {
    // enable only the display digit for that 
    activateDisplay(display);
    // send the number to the display
    if(display == currentDisplay){
      if (state == 1){
        if (millis() - lastBlinkTime > blinkDuration) {
          lastBlinkTime = millis();
          writeReg(byteEncodingsWithDP[number]);
        }
        else{
          writeReg(byteEncodings[number]);
        }
      }
      else{
          writeReg(byteEncodingsWithDP[number]);
      }
    }
    else writeReg(byteEncodings[number]);
    delay(5);
}

void moveThroughDisplays(){
  if (xValue > maxThreshold && joyMovedX == false){
    if(currentDisplay == 3)
      currentDisplay = 0;
    else
      currentDisplay++;
    joyMovedX = true;
  }
  else if (xValue < minThreshold && joyMovedX == false){
    if(currentDisplay == 0)
      currentDisplay = 3;
    else
      currentDisplay--;
    joyMovedX = true;
  }

  if(xValue > minThreshold && xValue < maxThreshold){
    joyMovedX = false;
  }
}

void moveThroughDigits(){
  if (yValue > maxThreshold && joyMovedY == false){
    if(displayValue[3-currentDisplay] < 15){
      displayValue[3-currentDisplay]++;      
    }
    else{
      displayValue[3-currentDisplay] = 0;
    }
    joyMovedY = true;
  }
  else if (yValue < minThreshold && joyMovedY == false){
    if(displayValue[3-currentDisplay] > 0){
      displayValue[3-currentDisplay]--;
    }
    else{
      displayValue[3-currentDisplay] = 15;
    }
    joyMovedY = true;
  }

  if(yValue > minThreshold && yValue < maxThreshold){
    joyMovedY = false;
  }
}