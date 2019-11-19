int potentionmeterRed = 0;
int potentionmeterGreen = 0;
int potentionmeterBlue = 0;

int RED_PIN = 3;
int GREEN_PIN = 5;
int BLUE_PIN = 6;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
    potentionmeterRed = analogRead(A0);
    potentionmeterGreen = analogRead(A1);
    potentionmeterBlue = analogRead(A2);
    potentionmeterRed = map(potentionmeterRed, 0, 1023, 0, 255);
    potentionmeterGreen = map(potentionmeterGreen, 0, 1023, 0, 255);
      potentionmeterBlue = map(potentionmeterBlue, 138, 1023, 0, 255);
    analogWrite(RED_PIN, potentionmeterRed);
    analogWrite(GREEN_PIN, potentionmeterGreen);
    analogWrite(BLUE_PIN, potentionmeterBlue);
    
    Serial.print(potentionmeterRed);
    Serial.print(" ");
    Serial.print(potentionmeterGreen);
    Serial.print(" ");
    Serial.print(potentionmeterBlue);
    Serial.print(" ");
    Serial.println();
}
