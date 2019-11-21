int D1Pin = 2;
int D2Pin = 3;
int D3Pin = 4;
int D4Pin = 5;

int aPin = 6;
int bPin = 7;
int cPin = 8;
int dPin = 9;
int ePin = 10;
int fPin = 11;
int gPin = 12;
int pointPin = 13;

int pinSW = A2;
int pinX = A0;
int pinY = A1;

int xValue = 0;
int yValue = 0;
int digitNumber = 0;

const int segSize = 8;
const int DPinsSize = 4;
int index = 0;
int switchValue = 0;

int minThreshold = 20;
int maxThreshold = 1000;
bool yJoyMoved = false;
bool xJoyMoved = false;
int digit = 0;
int digitsNumber = 0;

int Digits[10][segSize] = {
  //a, b, c, d, e, f, g .
  {1, 1, 1, 1, 1, 1, 0, 0}, //0
  {0, 1, 1, 0, 0, 0, 0, 0}, //1
  {1, 1, 0, 1, 1, 0, 1, 0}, //2
  {1, 1, 1, 1, 0, 0, 1, 0}, //3
  {0, 1, 1, 0, 0, 1, 1, 0}, //4
  {1, 0, 1, 1, 0, 1, 1, 0}, //5
  {1, 0, 1, 1, 1, 1, 1, 0}, //6
  {1, 1, 1, 0, 0, 0, 0, 0}, //7
  {1, 1, 1, 1, 1, 1, 1, 0}, //8
  {1, 1, 1, 0, 0, 1, 1, 0}  //9
};

int numbers[DPinsSize][segSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

int DPins[DPinsSize] = {
  D1Pin, D2Pin, D3Pin, D4Pin
};
int segments[segSize] = {
  aPin, bPin, cPin, dPin, ePin, fPin, gPin, pointPin
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  for (int i = 0; i < DPinsSize; i++) {
    pinMode(DPins[i], OUTPUT);
    digitalWrite(DPins[i], HIGH);
  }
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(pinSW, INPUT_PULLUP);
}

void writeDigit() {
  for (int i = 0; i < DPinsSize; i++) {

    for (int z = 0; z < DPinsSize; z++) {
      digitalWrite(DPins[z], HIGH);
    }
    digitalWrite(DPins[i], LOW);

    for (int j = 0; j < segSize; j++) {
      digitalWrite(segments[j], numbers[i][j]);
    }
    delay(5);
  }
}


void putNumber(int n, int where) {
  for (int i = 0; i < segSize; i++) {
    numbers[where][i] = Digits[n][i];
  }
}


void changeDigit() {
if (xValue < minThreshold && !xJoyMoved) {
    if (digit == 0) {
      digit = 9;
    }
    else {
      digit--;
    }
    putNumber(digit, digitNumber);

    xJoyMoved = true;
  }

  if (xValue > maxThreshold && !xJoyMoved) {
    if (digit == 9) {
      digit = 0;
    }
    else {
      digit++;
    }
    putNumber(digit, digitNumber);

    xJoyMoved = true;
  }

  if (xValue >= minThreshold && xValue <= maxThreshold) {
    xJoyMoved = false;
  }
}

void changeDisplay(){
  if (yValue < minThreshold && !yJoyMoved) {
    if (digitNumber == 3) {
      digitNumber = 0;
    }
    else {
      digitNumber++;
    }
     for (int z = 0; z < DPinsSize; z++) {
      digitalWrite(DPins[z], HIGH);
    }
    digitalWrite(DPins[digitNumber], LOW);

    yJoyMoved = true;
  }

  if (yValue > maxThreshold && !yJoyMoved) {
    if (digitNumber == 0) {
      digitNumber = 3;
    }
    else {
      digitNumber--;
    }
     for (int z = 0; z < DPinsSize; z++) {
      digitalWrite(DPins[z], HIGH);
    }
    digitalWrite(DPins[digitNumber], LOW);

    yJoyMoved = true;
  }

  if (yValue >= minThreshold && yValue <= maxThreshold) {
    yJoyMoved = false;
  }
}

bool buttonPressed = false;
bool lockedOn = false;
int buttonState = 0;
int lastButtonState = 0;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
void loop() {
  // put your main code here, to run repeatedly:

  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  switchValue = digitalRead(pinSW);

  if (lockedOn){
    changeDigit();
    for (int i = 0; i < DPinsSize; i++){
      numbers[i][segSize - 1] = 0;
    }
    numbers[digitNumber][segSize - 1] = 1;
  }
  else{
    changeDisplay();
  }

if (switchValue != lastButtonState){
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay){
    if (switchValue != buttonState){
      buttonState = switchValue;
      if (buttonState == HIGH){
        //Digits[digitNumber][segSize - 1] = !Digits[digitNumber][segSize - 1];
        lockedOn = !lockedOn;
        if (lockedOn){
          digit = 0;
        }
      }
    
    }
  }
  lastButtonState = switchValue;
  /*
    Serial.print(xValue);
    Serial.print(" ");
    Serial.print(yValue);
    Serial.print(" ");
  */


  /*
       for (int z = 0; z < DPinsSize; z++){
         for (int j = 0; j < segSize; j++){
           Serial.print(numbers[z][j]);
           Serial.print(" ");
         }
         Serial.println();
       }
       Serial.println();
  */
  writeDigit();


}
