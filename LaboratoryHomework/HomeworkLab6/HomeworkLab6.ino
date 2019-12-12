#include <LiquidCrystal.h>

const int RS = 12;
const int enable = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
int pinSW = A2;
int pinX = A0;
int pinY = A1;

int xValue = 0;
int yValue = 0;
bool switchValue = false;

bool endScreenFlag = false;
bool startScreenFlag = false;
bool menuScreenFlag = true;

bool screenFlags[3] = {
  menuScreenFlag, startScreenFlag, endScreenFlag
};

int minThreshold = 20;
int maxThreshold = 1000;
bool yJoyMoved = false;
bool xJoyMoved = false;

LiquidCrystal lcd (RS, enable, d4, d5, d6, d7);

bool buttonPressed = false;
int buttonState = 1;
int lastButtonState = 1;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

byte joystickPosition = 0;
const byte joystickMaxPosition = 2;

char menuJoystick[joystickMaxPosition] = "> ";
char * bottomMenu[joystickMaxPosition] = {
  "Start",
  "End"
};


byte level = 1;
byte maxLevel = 9;
byte lives = 3;
byte score = 0;
byte highscore = 0;
void printMenuScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Snake Game");
  lcd.setCursor(0, 1);
  lcd.print(menuJoystick[0]);
  lcd.setCursor(1, 1);
  lcd.print(bottomMenu[0]);
  lcd.setCursor(8, 1);
  lcd.print(menuJoystick[1]);
  lcd.setCursor(9, 1);
  lcd.print(bottomMenu[1]);
}
void printEndScreen() {
  if (highscore < score){
    highscore = score;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Highscore=");
  lcd.setCursor(10, 0);
  lcd.print(highscore);
  lcd.setCursor(0, 1);
  lcd.print("Your score=");
  lcd.setCursor(11, 1);
  lcd.print(score);
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Thank you for ");
  lcd.setCursor(0, 1);
  lcd.print("playing game :D");
}
void joystickMenu() {
  if (yValue < minThreshold && !yJoyMoved) {
    if (joystickPosition == 0) {
      joystickPosition = joystickMaxPosition - 1;
    }
    else {
      joystickPosition --;
    }
    menuJoystick[joystickPosition] = '>';
    menuJoystick[joystickMaxPosition - 1 - joystickPosition] = ' ';
    printMenuScreen();
    yJoyMoved = true;
  }

  if (yValue > maxThreshold && !yJoyMoved) {
    if (joystickPosition == joystickMaxPosition - 1) {
      joystickPosition = 0;
    }
    else {
      joystickPosition ++;
    }
    menuJoystick[joystickPosition] = '>';
    menuJoystick[joystickMaxPosition - 1 - joystickPosition] = ' ';
    printMenuScreen();
    yJoyMoved = true;
  }



  if (yValue >= minThreshold && yValue <= maxThreshold) {
    yJoyMoved = false;
  }

}

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);

  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(pinSW, INPUT_PULLUP);

  printMenuScreen();
  Serial.begin(9600);
}

void resetFlags() {
  for (int j = 0; j < 3; j++) {
    screenFlags[j] = false;
  }
}
void buttonPress() {
  if (switchValue != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (switchValue != buttonState) {
      buttonState = switchValue;
      if (buttonState == HIGH) {
        //if is in the end menu check if button pressed
        if (screenFlags[2] == true) {
          resetFlags();
          //set menuFlag = true
          screenFlags[0] = true;
          printMenuScreen();
          return;
        }
        resetFlags();
        //if you are on the menu and the joystick is on start game, activate the gameFlag
        if (menuJoystick[0] == '>') {
          printSettingsScreen();
          screenFlags[1] = true;
        }
        //if you are on the End with the joystick, activate endFlag
        else if (menuJoystick[1] == '>') {
          printEndScreen();
          screenFlags[2] = true;
        }
      }

    }
  }
  lastButtonState = switchValue;
}

void printSettingsScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Setting Level");
  lcd.setCursor(0, 1);
  lcd.print("Level = ");
  lcd.setCursor(8, 1);
  lcd.print(level);

}

bool levelSelected = false;
void joystickSettings() {
  if (xValue < minThreshold && !xJoyMoved) {
    if (level == 1) {
      level = maxLevel;
    }
    else {
      level --;
    }
    printSettingsScreen();
    xJoyMoved = true;
  }

  if (xValue > maxThreshold && !xJoyMoved) {


    if (level == maxLevel) {
      level = 1;
    }
    else {
      level ++;
    }
    printSettingsScreen();
    xJoyMoved = true;
  }

  if (xValue >= minThreshold && xValue <= maxThreshold) {
    xJoyMoved = false;
  }

  if (switchValue != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (switchValue != buttonState) {
      buttonState = switchValue;
      if (buttonState == HIGH) {
        levelSelected = true;
      }
    }

  }
  lastButtonState = switchValue;
}




void printGame() {
  score = level * 3;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Level=");
  lcd.setCursor(6, 0);
  lcd.print(level);
  if (level != maxLevel){
    level ++;
  }
  lcd.setCursor(8, 0);
  lcd.print("lives=");
  lcd.setCursor(14, 0);
  lcd.print(lives);
  lcd.setCursor(0, 1);
  lcd.print("Score=");
  lcd.setCursor(6, 1);

  lcd.print(score);
}
void loop() {
  // put your main code here, to run repeatedly:
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  switchValue = digitalRead(pinSW);

  if (!screenFlags[1]) {
    buttonPress();
  }

  if (screenFlags[0]) {
    joystickMenu();
  }
  else if (screenFlags[1]) {
    joystickSettings();
    if (levelSelected) {
      printGame();
      delay(5000);
      printGame();
      delay(5000);
      printGame();
      delay(1000);
      resetFlags();
      screenFlags[2] = true;
      printEndScreen();
    }

  }
  else if (screenFlags[2]) {
    levelSelected = false;
    /*
      delay(3000);
      resetFlags();
      screenFlags[0] = true;
      printMenuScreen();
    */
  }


}
