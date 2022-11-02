const int buttonPin = 2;
const int buzzerPin = 3;
const int pedestrianRedLedPin = 10;
const int pedestrianGreenLedPin = 9;
const int carsRedLedPin = 13;
const int carsYellowLedPin = 12;
const int carsGreenLedPin = 11;

const int phase1Duration = 8000;
const int phase2Duration = 3000;
const int phase3Duration = 8000;
const int phase4Duration = 4000;

const int blinkInterval = 300;

const int buzzerSound = 466;
const int buzzerInterval = 350;
const int buzzerStopInterval = 200;
const int buzzerIntervalFast = 250;
const int buzzerStopIntervalFast = 100;

int blinkValue;
int pedestrianRedLedState;
int pedestrianGreenLedState;
int carsRedLedState;
int carsYellowLedState;
int carsGreenLedState;
int phase = 1;

unsigned long lastDebounceTime;
unsigned int debounceDelay = 50;
unsigned long timeSinceLastPress;
unsigned long lastBlinkTime;
unsigned long timeSinceLastBuzzer;

byte buttonState;
byte isButtonPressed;
byte lastReading;
byte reading;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(pedestrianRedLedPin, OUTPUT);
  pinMode(pedestrianGreenLedPin, OUTPUT);
  pinMode(carsRedLedPin, OUTPUT);
  pinMode(carsYellowLedPin, OUTPUT);
  pinMode(carsGreenLedPin, OUTPUT);
}

void loop() {
  // read button status with a debounce of 50 mil
  buttonStatus();
  // the four phases of the traffic lights
  phaseSelector();
}

void phaseSelector() {
  if (phase == 1){
    phase1();
    // if button is pressed, we start counting down phase1Duration (8 seconds)
    if (isButtonPressed == 1 && (millis() - timeSinceLastPress > phase1Duration)){
      phase = 2;
      isButtonPressed = 0;
      timeSinceLastPress = millis();      
    }
    noTone(buzzerPin);
  } else if(phase == 2){
      phase2();
      if (millis() - timeSinceLastPress > phase2Duration) {
        phase = 3;
        timeSinceLastPress = millis();
      }
  } else if(phase == 3){
      phase3();
      if (millis() - timeSinceLastPress > phase3Duration) {
        phase = 4;
        timeSinceLastPress = millis();
      }
  } else if(phase == 4){
    phase4();
    if (millis() - timeSinceLastPress > phase4Duration) {
      phase = 1;
    }
  }
}

void buttonStatus(){
   reading = digitalRead(buttonPin);

   if (reading != lastReading) {
     lastDebounceTime = millis();
   }
   if ((millis() - lastDebounceTime) > debounceDelay) {
     buttonState = reading;
    // if the button hasn't been pressed before, record that it was pressed
     if (buttonState == LOW && phase == 1 && isButtonPressed == LOW) {
       isButtonPressed = 1;
       timeSinceLastPress = millis();
      }
   }
   lastReading = reading;
}

void phase1(){
  digitalWrite(pedestrianRedLedPin, HIGH);
  digitalWrite(pedestrianGreenLedPin, LOW);
  digitalWrite(carsRedLedPin, LOW);
  digitalWrite(carsYellowLedPin, LOW);
  digitalWrite(carsGreenLedPin, HIGH);
}

void phase2(){
    digitalWrite(pedestrianRedLedPin, HIGH);
    digitalWrite(pedestrianGreenLedPin, LOW);
    digitalWrite(carsRedLedPin, LOW);
    digitalWrite(carsYellowLedPin, HIGH);
    digitalWrite(carsGreenLedPin, LOW);
}

void phase3(){
    digitalWrite(pedestrianRedLedPin, LOW);
    digitalWrite(pedestrianGreenLedPin, HIGH);
    digitalWrite(carsRedLedPin, HIGH);
    digitalWrite(carsYellowLedPin, LOW);
    digitalWrite(carsGreenLedPin, LOW);

  // if time since last buzzer buzzed is greater than its frequency, then make it buzz
  // buzzerInterval is greater than buzzerStopInterval because, otherwise, the buzzer would never stop, it would continue to buzz for the rest of the phase 3
  if (millis() - timeSinceLastBuzzer > buzzerInterval){
    tone(buzzerPin, buzzerSound);
    timeSinceLastBuzzer = millis();
  }
  // if time since last buzzer buzzed is greater than its frequency to stop, stop it
  if (millis() - timeSinceLastBuzzer > buzzerStopInterval){
    noTone(buzzerPin);
  }
}

void phase4(){
    digitalWrite(pedestrianRedLedPin, LOW);
    digitalWrite(carsRedLedPin, HIGH);
    digitalWrite(carsYellowLedPin, LOW);
    digitalWrite(carsGreenLedPin, LOW);

  // if time since last blink is greater than its frequency, then make it blink
  if (millis() - lastBlinkTime > blinkInterval){
    blinkValue = !blinkValue;
    lastBlinkTime = millis();
  }
  digitalWrite(pedestrianGreenLedPin, blinkValue);

  if (millis() - timeSinceLastBuzzer > buzzerIntervalFast){
    tone(buzzerPin, buzzerSound);
    timeSinceLastBuzzer = millis();
  }

  if (millis() - timeSinceLastBuzzer > buzzerStopIntervalFast){
    noTone(buzzerPin);
  }
}