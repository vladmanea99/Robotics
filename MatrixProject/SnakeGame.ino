#include <LiquidCrystal.h>
#include "LedControl.h" // need the library
#include <EEPROM.h>

enum DIRECTIONS {NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3};
DIRECTIONS currentDirection = 0;
LedControl lc = LedControl(12, 11, 10, 1); //DIN, CLK, LOAD,
const int RS = 7;
const int enable = 6;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
int pinSW = A2;
int pinX = A0;
int pinY = A1;

int speed = 300;
int EEPROMHighscore = 0;

byte headCurrentPositionX = 5;
byte headCurrentPositionY = 5;

byte bodyPositions[2][64];
byte nOfBodyParts = 3;

bool hasShotLaser = false;

byte lastPositionOfLastBodyPartX = 0;
byte lastPositionOfLastBodyPartY = 0;

byte foodCurrentPositionX = rand() % 8;
byte foodCurrentPositionY = rand() % 8;

byte laserCurrentPositionX = 0;
byte laserCurrentPositionY = 0;

int timeForEating = 20000;
long int lastMealTime = 0;

int xValue = 0;
int yValue = 0;
bool switchValue = false;

bool endScreenFlag = false;
bool startScreenFlag = false;
bool menuScreenFlag = true;

bool screenFlags[3] = {
  menuScreenFlag, startScreenFlag, endScreenFlag
};

int minThreshold = 400;
int maxThreshold = 600;
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
int score = 0;
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
  if (highscore < score) {
    highscore = score;
  }
   EEPROM.get(0, EEPROMHighscore);

  Serial.print(highscore);
  Serial.print(" ");
  Serial.println(EEPROMHighscore);
  if (highscore > EEPROMHighscore) {
      EEPROMHighscore = highscore;
      EEPROM.put(0, EEPROMHighscore);
      Serial.println(EEPROMHighscore);
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

  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 2); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen

  lc.setLed(0, foodCurrentPositionX, foodCurrentPositionY, true);
 EEPROM.get(0, EEPROMHighscore);

  highscore = EEPROMHighscore;

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


int joystickMovementGame() {
  if (xValue < minThreshold) {
    return 0;
  }

  if (xValue > maxThreshold) {
    return 2;
  }

  if (yValue < minThreshold) {
    return 3;
  }

  if (yValue > maxThreshold) {
    return 1;
  }
  return currentDirection;

}

void moveSnake() {

  lc.setLed(0, bodyPositions[1][nOfBodyParts - 1], bodyPositions[0][nOfBodyParts - 1], false);
  lastPositionOfLastBodyPartX = bodyPositions[1][nOfBodyParts - 1];
  lastPositionOfLastBodyPartY = bodyPositions[0][nOfBodyParts - 1];
  bodyPositions[1][0] = headCurrentPositionX;
  bodyPositions[0][0] = headCurrentPositionY;
  for (int i = nOfBodyParts - 1; i > 0; i--) {
    bodyPositions[0][i] = bodyPositions[0][i - 1];
    bodyPositions[1][i] = bodyPositions[1][i - 1];
  }

  switch (currentDirection) {

    case (NORTH): headCurrentPositionX = (headCurrentPositionX == 7) ? 0 : (headCurrentPositionX + 1);
      break;
    case (SOUTH): headCurrentPositionX = (headCurrentPositionX == 0) ? 7 : (headCurrentPositionX - 1);
      break;
    case (EAST): headCurrentPositionY = (headCurrentPositionY == 7) ? 0 : (headCurrentPositionY + 1);
      break;
    case (WEST): headCurrentPositionY = (headCurrentPositionY == 0) ? 7 : (headCurrentPositionY - 1);
      break;
  }


  lc.setLed(0, headCurrentPositionX, headCurrentPositionY, true);
  delay(speed);
}

void printGameLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Level=");
  lcd.setCursor(6, 0);
  lcd.print(level);
  lcd.setCursor(8, 0);
  lcd.print("lives=");
  lcd.setCursor(14, 0);
  lcd.print(lives);
  lcd.setCursor(0, 1);
  lcd.print("Score=");
  lcd.setCursor(6, 1);
  lcd.print(score);
  lcd.setCursor(9, 1);
  lcd.print("Time:");
  lcd.setCursor(14, 1);


}

bool isFoodPlacedCorrect() {

  if (foodCurrentPositionX == headCurrentPositionX && foodCurrentPositionY == headCurrentPositionY) {
    return false;
  }

  for (int i = 0; i < nOfBodyParts; i++) {
    if (foodCurrentPositionX == bodyPositions[1][i] && foodCurrentPositionY == bodyPositions[0][i]) {
      return false;
    }
  }
  return true;
}

void putFood() {
  while (!isFoodPlacedCorrect()) {
    foodCurrentPositionX = rand() % 8;
    foodCurrentPositionY = rand() % 8;
  }
  lc.setLed(0 , foodCurrentPositionX, foodCurrentPositionY, true);
}

void eatFood() {
  if (headCurrentPositionX == foodCurrentPositionX && headCurrentPositionY == foodCurrentPositionY) {
    lastMealTime = millis();
    putFood();
    score = score + level * 1.2;
    speed = 300 - score / 2;
    nOfBodyParts++;
    bodyPositions[1][nOfBodyParts - 1] = lastPositionOfLastBodyPartX;
    bodyPositions[0][nOfBodyParts - 1] = lastPositionOfLastBodyPartY;

    lc.setLed(0, lastPositionOfLastBodyPartX, lastPositionOfLastBodyPartY, true);
    printGameLCD();
  }
}

void resetMatrix() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      lc.setLed(0, i, j, false);
    }
  }
  headCurrentPositionX = 5;
  headCurrentPositionY = 5;
  nOfBodyParts = 3;

  lc.setLed(0, 5, 5, true);
  for (int i = 0; i < nOfBodyParts; i++) {
    bodyPositions[1][i] = 5;
    bodyPositions[0][i] = 4 - i;
    lc.setLed(0, 5, 4 - i, true);
  }
  putFood();
}

bool hasDied() {
  for (int i = 0; i < nOfBodyParts; i++) {
    if (headCurrentPositionX == bodyPositions[1][i] && headCurrentPositionY == bodyPositions[0][i]) {
      return true;
    }
  }
  return false;
}

byte laserHitBody() {
  for (int i = 0; i < nOfBodyParts; i++) {
    if (laserCurrentPositionX == bodyPositions[1][i] && laserCurrentPositionY == bodyPositions[0][i]) {
      return i;
    }
  }
  return -1;
}

void shootLaser() {
  switchValue = digitalRead(pinSW);
  //Serial.println(switchValue);
  if (switchValue != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (switchValue != buttonState) {
      buttonState = switchValue;
      if (buttonState == HIGH) {
        Serial.println("Am apasat");
        laserCurrentPositionX = headCurrentPositionX;
        laserCurrentPositionY = headCurrentPositionY;
        int positionHit = laserHitBody();
        while (positionHit == -1) {
          lc.setLed(0, laserCurrentPositionX, laserCurrentPositionY, false);
          switch (currentDirection) {
            case (NORTH): laserCurrentPositionX = (laserCurrentPositionX == 7) ? 0 : (laserCurrentPositionX + 1);
              break;
            case (SOUTH): laserCurrentPositionX = (laserCurrentPositionX == 0) ? 7 : (laserCurrentPositionX - 1);
              break;
            case (EAST): laserCurrentPositionY = (laserCurrentPositionY == 7) ? 0 : (laserCurrentPositionY + 1);
              break;
            case (WEST): laserCurrentPositionY = (laserCurrentPositionY == 0) ? 7 : (laserCurrentPositionY - 1);
              break;
          }
          lc.setLed(0, laserCurrentPositionX, laserCurrentPositionY, true);
          delay(50);
          positionHit = laserHitBody();
        }

        for (int i = positionHit; i < nOfBodyParts; i++){
          lc.setLed(0, bodyPositions[1][i], bodyPositions[0][i], false); 
        }
        nOfBodyParts = positionHit;
        hasShotLaser = true;
      }
    }
  }
  lastButtonState = switchValue;

  
}

void printGameMatrix() {
  lastMealTime = millis();
  while (lives != 0) {
    resetMatrix();
    delay(2000);
    lastMealTime = millis();
    while ((millis() - lastMealTime) <= timeForEating) {
      if (hasDied()) {
        break;
      }
      lcd.print((timeForEating - (millis() - lastMealTime)) / 1000);
      lcd.setCursor(14, 1);
      xValue = analogRead(pinX);
      yValue = analogRead(pinY);
      
      eatFood();
      if(!hasShotLaser){
        shootLaser();
      }
      currentDirection = joystickMovementGame();
      moveSnake();
    }
    lives --;
    printGameLCD();
  }
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
      lastMealTime = millis();
      lives = 3;
      hasShotLaser = false;
      printGameLCD();
      printGameMatrix();

      resetFlags();
      screenFlags[2] = true;
      printEndScreen();
    }

  }
  else if (screenFlags[2]) {
    
    levelSelected = false;

  }

}
