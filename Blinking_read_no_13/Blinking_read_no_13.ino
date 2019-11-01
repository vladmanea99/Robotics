int BLUE_PIN = 11;
int GREEN_PIN = 10;
int RED_PIN = 9;
int potentionReader = 0;
int LED_PIN = 3;
void setup() {
  // put your setup code here, to run once:
  pinMode(A1, INPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  Serial.begin(9600);
}

int ledState = 0;

void loop() {
  // put your main code here, to run repeatedly:
    potentionReader = analogRead(A1);
    
    ledState = map(potentionReader, 0, 1023, 0, 255);

    analogWrite(RED_PIN, random(255));
    analogWrite(GREEN_PIN, random(255));
    analogWrite(BLUE_PIN, random(255));
    delay(500);
    Serial.println(analogRead(LED_PIN));
  
}
