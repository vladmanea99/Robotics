int aPin = 2;
int bPin = 3;
int cPin = 4;
int dPin = 5;
int ePin = 6;
int fPin = 7;
int gPin = 8;
int pointPin = 9;

int pinX = A0;
int pinY = A1;
int pinSW = 10;

int xValue = 0;
int yValue = 0;

const int segSize = 8;
int index = 0;
int state = HIGH;

int segments[segSize] = {
  aPin, bPin, cPin, dPin, ePin, fPin, gPin, pointPin
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i =0; i < segSize; i++){
    pinMode(segments[i], OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
}
int Digits[10][segSize] = {
 //a, b, c, d, e, f, g
  {1, 1, 1, 1, 1, 1, 0}, //0
  {0, 1, 1, 0, 0, 0, 0}, //1
  {1, 1, 0, 1, 1, 0, 1}, //2
  {1, 1, 1, 1, 0 ,0 ,1}, //3
  {0, 1, 1, 0, 0, 1, 1}, //4
  {1, 0, 1, 1, 0, 1, 1}, //5
  {1, 0, 1, 1, 1, 1, 1}, //6
  {1, 1, 1, 0, 0, 0, 0}, //7
  {1, 1, 1, 1, 1, 1, 1}, //8
  {1, 1, 1, 0, 0, 1, 1}  //9
};
void writeDigit(int n){
  for (int i = 0; i < segSize; i++){
    digitalWrite(segments[i], Digits[n][i]);
    delay(50);
  }
}
int switchValue = 0;

int minThreshold = 20;
int maxThreshold = 1000;
bool joyMoved = false;
int digit = 0;

void loop() {
  // put your main code here, to run repeatedly:
  /*for (int i = 0; i < 10; i++){
      writeDigit(i);
    delay(1000);
  }
*/

  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  switchValue = digitalRead(pinSW);

  if (xValue < minThreshold && !joyMoved){
    if (digit == 0){
      digit = 9;
    }
    else{
      digit--;
    }
    writeDigit(digit);
    joyMoved = true;
  }

    if (xValue > maxThreshold && !joyMoved){
    if (digit == 9){
      digit = 0;
    }
    else{
      digit++;
    }
    writeDigit(digit);
    joyMoved = true;
  }

  if (xValue >= minThreshold && xValue <= maxThreshold){
    joyMoved = false;
  }

  Serial.print(xValue);
  Serial.print(" ");
  Serial.print(yValue);
  Serial.print(" ");
  Serial.println(switchValue);

}
