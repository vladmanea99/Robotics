#include <LiquidCrystal.h>

const int RS = 12;
const int enable = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;

const int trigPin = 9;
const int echoPin = 10;

long duration;
int distance;


LiquidCrystal lcd (RS, enable, d4, d5, d6, d7);

void setup() {
  // put your setup code here, to run once:
lcd.begin(16, 2);

lcd.print ("hello world!");

pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
Serial.begin(9600);
}
int lastDebounceTime = 0;
int debounceDelay = 10;
void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0,1);
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

 if ((millis() - lastDebounceTime) > debounceDelay){
      lastDebounceTime = millis();
   lcd.clear();
  }
  
   lcd.print(distance);
   distance = duration * 0.034/2;
     
//  lcd.millis()
 
  Serial.println(distance);
}
