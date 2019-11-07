int BUTTON_PIN = 2;
int LED_PIN = 4;
int buttonState = 0;
bool ledState = false;
int lastButtonState = 0;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState){
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay){
    if (reading != buttonState){
      buttonState = reading;
      if (buttonState == LOW){
        ledState = !ledState;
      }
    }
  }
  lastButtonState = reading;
  digitalWrite(LED_PIN, ledState);
  Serial.println(buttonState);
}
