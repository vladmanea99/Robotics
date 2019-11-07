int potPin = A0;
int ledPin = 5;

void setup() {
  // put your setup code here, to run once:
  pinMode(potPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int valPot = analogRead(potPin);
  valPot = map(valPot, 0, 1023, 0, 255);
  analogWrite(ledPin, valPot);
  Serial.println(valPot);

}
