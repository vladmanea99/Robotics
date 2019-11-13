int potPin = A0;
int ledPin = 5;
bool ledState = false;
int buzzerTone = 1000;
void setup() {
  // put your setup code here, to run once:
  pinMode(potPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*int valPot = analogRead(potPin);
  //valPot = map(valPot, 0, 1023, 0, 255);
  if (valPot < 300){
    ledState = HIGH;
  }
  if (valPot > 500){
    ledState = LOW;
  }
  digitalWrite(ledPin, ledState);
  Serial.println(valPot);
*/
tone(ledPin, buzzerTone, 500);
delay(100);
noTone(ledPin);
delay(100);
buzzerTone += 50;
}
