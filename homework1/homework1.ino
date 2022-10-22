const int potPinRed = A0;
const int potPinGreen = A1;
const int potPinBlue = A2;

const int ledPinRed = 11;
const int ledPinGreen = 10;
const int ledPinBlue = 9;

const int maxValuePot = 1023;
const int maxValueLed = 255;

int potValueRed;
int potValueGreen;
int potValueBlue;

int ledValueRed;
int ledValueGreen;
int ledValueBlue;

void setup(){
  pinMode(potPinRed, INPUT);
  pinMode(potPinGreen, INPUT);
  pinMode(potPinBlue, INPUT);

  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);
  pinMode(ledPinBlue, OUTPUT);
}

void loop(){
  potValueRed = analogRead(potPinRed);
  potValueGreen = analogRead(potPinGreen);
  potValueBlue = analogRead(potPinBlue);

  setColorLed(potValueRed, potValueGreen, potValueBlue);
}

void setColorLed(int redValue,int greenValue,int blueValue){
  ledValueRed = map(redValue, 0, maxValuePot, 0, maxValueLed);
  ledValueGreen = map(greenValue, 0, maxValuePot, 0, maxValueLed);
  ledValueBlue = map(blueValue, 0, maxValuePot, 0, maxValueLed);

  analogWrite(ledPinRed, ledValueRed);
  analogWrite(ledPinGreen, ledValueGreen);
  analogWrite(ledPinBlue, ledValueBlue);
}