const int pinSW = 2; // digital pin connected to switch output
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output

// declare all the segments pins
const int pinA = 4;
const int pinB = 5;
const int pinC = 6;
const int pinD = 7;
const int pinE = 8;
const int pinF = 9;
const int pinG = 10;
const int pinDP = 11;

const int segSize = 8;

byte dpState = LOW;
byte swState = LOW;
byte lastReading = LOW;
byte reading = LOW;
byte centered = HIGH;
byte isButtonPressed = LOW;

int xValue;
int yValue;
int minThreshold = 400;
int maxThreshold = 600;
int state = 1;
int currentSelectedValue;

unsigned long lastDebounceTime;
unsigned int debounceDelay = 100;
unsigned int debounceDelayLong = 1000;
unsigned long lastBlinkTime;

const int blinkInterval = 300;
int blinkValue;

int segments[segSize] = { 
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

byte digitMatrix[8][segSize] = {
// a  b  c  d  e  f  g dp
  {1, 0, 0, 0, 0, 0, 0, 0}, // 0
  {0, 1, 0, 0, 0, 0, 0, 0}, // 1
  {0, 0, 1, 0, 0, 0, 0, 0}, // 2
  {0, 0, 0, 1, 0, 0, 0, 0}, // 3
  {0, 0, 0, 0, 1, 0, 0, 0}, // 4
  {0, 0, 0, 0, 0, 1, 0, 0}, // 5
  {0, 0, 0, 0, 0, 0, 1, 0}, // 6
  {0, 0, 0, 0, 0, 0, 0, 1}, // 7
};

byte digitArray[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  buttonStatus();
  stateSelector();
}

void stateSelector(){
  if(state == 1){
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);

    displaySegments();
    displaySelectedCell(currentSelectedValue);
    
    switch(currentSelectedValue){
      case 0:
        if (xValue > maxThreshold){
          currentSelectedValue = 1;
        }
        else if (xValue < minThreshold){
          currentSelectedValue = 5;
        }
        else if (yValue < minThreshold){
          currentSelectedValue = 6;
        }
        break;
      case 1:
        if (xValue < minThreshold){
          currentSelectedValue = 5;
        }
        else if (yValue < minThreshold){
          currentSelectedValue = 6;
        }
        else if (yValue > maxThreshold){
          currentSelectedValue = 0;
        }
        break;    
      case 2:  
        if (xValue < minThreshold){
          currentSelectedValue = 4;
        }
        else if (yValue < minThreshold){
          currentSelectedValue = 3;
        }
        else if (yValue > maxThreshold){
          currentSelectedValue = 6;
        }
        else if (xValue > maxThreshold){
          currentSelectedValue = 7;
        }
        break;
      case 3:  
        if (xValue < minThreshold){
          currentSelectedValue = 4;
        }
        else if (xValue > maxThreshold){
          currentSelectedValue = 2;
        }
        else if (yValue > maxThreshold){
          currentSelectedValue = 6;
        }
        break;
      case 4:  
        if (xValue > maxThreshold){
          currentSelectedValue = 2;
        }
        else if (yValue < minThreshold){
          currentSelectedValue = 3;
        }
        else if (yValue > maxThreshold){
          currentSelectedValue = 6;
        }
        break;
      case 5:  
        if (xValue > maxThreshold){
          currentSelectedValue = 1;
        }
        else if (yValue < minThreshold){
          currentSelectedValue = 6;
        }
        else if (yValue > maxThreshold){
          currentSelectedValue = 0;
        }
        break;
      case 6:  
        if (yValue < minThreshold){
          currentSelectedValue = 3;
        }
        else if (yValue > maxThreshold){
          currentSelectedValue = 0;
        }
        break;
      case 7:  
        if (xValue < minThreshold){
          currentSelectedValue = 2;
        }
        break;
    }
    delay(100);
    Serial.println(isButtonPressed);
    if (isButtonPressed == HIGH){
      state = 2;
      isButtonPressed = LOW;
      digitalWrite(segments[currentSelectedValue], digitArray[currentSelectedValue]);
    }
  }
  else if(state == 2){
    displayPressedCell(currentSelectedValue);
    if (isButtonPressed == HIGH){
      state = 1;
      isButtonPressed = LOW;
    }
  }
}

// display the blinking status for the current segment
void displaySelectedCell(int cell){
  if (millis() - lastBlinkTime > blinkInterval){
    blinkValue = !blinkValue;
    lastBlinkTime = millis();
  }
  digitalWrite(segments[cell], blinkValue);
}

// display the ON/OFF toggle for the pressed segment
void displayPressedCell(int cell){
    if(state == 2){
      xValue = analogRead(pinX);
      Serial.println(xValue);
      if (xValue > maxThreshold && centered){
        centered = LOW;
        digitalWrite(segments[cell], HIGH);
        digitArray[currentSelectedValue] = 1;
      }
      else if(xValue < minThreshold && centered){
        centered = LOW;
        digitalWrite(segments[cell], LOW);
        digitArray[currentSelectedValue] = 0;
      }
      if (xValue < maxThreshold && xValue > minThreshold){
        centered = HIGH;
      }
  }
}

void displaySegments() {
  for(int i = 0; i < segSize; i++)
    if (i != currentSelectedValue)
      digitalWrite(segments[i], digitArray[i]);
    else{
      digitalWrite(segments[i], 0);      
    }
}

void buttonStatus(){
   reading = digitalRead(pinSW);

   if (reading != lastReading) {
     lastDebounceTime = millis();
   }
   Serial.println((millis() - lastDebounceTime));
   if (((millis() - lastDebounceTime) > debounceDelayLong) && state == 1 && reading == LOW){
      for(int i = 0; i < segSize; i++)
        digitArray[i] = 0;
      
   }
   else if ((millis() - lastDebounceTime) > debounceDelay) {
     swState = reading;
     if (reading == LOW){
      isButtonPressed = HIGH;

      if (state == 2){
        state = 1;
        isButtonPressed = LOW;
      }
     }
   }
   lastReading = reading;
}