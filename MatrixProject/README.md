# MatrixProject

This project represents a snake game on arduino.

As hardware it will use:

-> 8x8 LED matrix (for the game itself)
-> LCD (for the menu)
-> Joystick (To control the snake)
-> MAX7219 Driver (To be able to connect everything to arduino)

As game specifications there will be:

-> Levels (By adding levels, the movement of the snake will increase even more, the score will increase as well)
-> Time pressure (By increasing the movement of the snake each time it eats food, you also have 20 seconds to eat a meal, else you lose a life)
-> Available highscore saved in the EEPROM

Video of gameplay:
https://www.youtube.com/watch?v=A-cri46x-wc&feature=youtu.be

How to play:

-> The goal of the game is to finish the game or at least try to make the biggest highscore
-> Use the joystick to move the snake in the 4 possible directions and take the food to increase the score and size as well
-> (The code is there, couldn't fix the bug) Try to have a life line by adding a laser that comes out of the snakes head and destroy a part of your body that the laser touches
-> The game finishes when you run out of lives or when there is no available tile for the snake to go to



Documentation of the program:

As listed above, the project uses a few pieces of hardware that combined, allows to be created a menu of the game and the display of the game itself.

As part of the description of the source code, I will now describe each function and how they work:

void printMenuScreen()
This function displays on the LCD the menu of the game, it's only job is to render the words themselves
To be able to show the position of the joystick in the menu, I created a vector of chars called "menuJoystick". It can only contain " " or ">".

void printEndScreen()
This function displays on the LCD the end screen of the game
This is made of two parts, the first part is of the score obtained in the run, and the highscore of the game itself. Here we verify if the player has made a bigger score than the highscore of the game, rechange the highscore and add it in the EEPROM
The last part is a thank you screen where the player has to press the button to go out of.

void joystickMenu()
This function is for scrolling through the main menu of the game with the joystick
Here if the joystick is moved to left or to right, the vector's "menuJoystick" values get changed between them marking where user's joystick is in the menu.

void setup()
This function is the original function of Arduino, it is called once at the start up of the Arduino, it is used for setting pins, the lcd, the matrix and getting the highscore value form the EEPROM.

void resetFlags()
This function resets all flags to false.
The flags represent in which screen of the game the user is
The vector of bool is called "screenFlags"
screenFlags[0] = menuScreenFlag
screenFlags[1] = startScreenFlag
screenFlags[2] = endScreenFlag.

void buttonPress()
This function checks if the button of the joystick is pressed
If so, it then checks in which screen the user is. Depending on the screen the button does different things
If the endScreenFlag is true then the user has to press once to go in the main menu by reseting the flags and setting the menuScreenFlag true
Else, it means he is in the menu screen. It then checks where the ">" is in "menuJoystick". If it is on the "Start" resets all flags and makes the startScreenFlag true
If the ">" is on "End" it then resets all flags, sets the endSreenFlag true and calls printEndScreen().

void printSettingScreen()
This function just displays the setting menu on the LCD.

void joystickSettings()
This function sets the variable "level" using the up-down movement of the joystick, the level can only be between 1 and 9
When the players presses the button it sets a bool called "levelSelected" to true so the program knows to move to the next screen.

int joystickMovementGame()
Before going to what this function does, we must know that the program has an enum called DIRECTION that contains the 4 cardinal points each having a value
NORTH = 0
EAST = 1
SOUTH = 2
WEST = 3
This function checks in which way the user moves the controller and returns a value between 0 and 3 representing the direction the snake should take.

void moveSnake()
This function represents the displaying of the snake on the matrix and it's movement on it with a certain speed
The idea is that the positions of the body of the snake are retained in a vector called "bodyPositions"
bodyPosition[1][i] = The X of the body part "i"
bodyPosition[0][i] = The Y of the body part "i"
Each time the snake moves all we have to do, in terms of swtiching leds on or off, is to turn on a led (the led of the head) and turn of the led of where was the last body part of the snake before moving.
More over, because we try to implement a shooting laser, we have to update the vector "bodyPositions"
The function then checks which is the direction of the snake and increases/decreases the X/Y position of the head.

void printGameLCD()
This function prints the words and numbers that are relevant on the LCD while the game is on and the snake moves.

bool isFoodPlacedCorrect()
This function checks if the food is placed correctly by checking if it spawned on the snake. 

void putFood()
This function checks over and over if the food is placed incorrectly until it is placed correctly
It then turns on the led of the food's position.

void eatFood()
This function checks if the snake's head is on the same tile as the food
If it is then reset the timer of the game
Put another food in the game
Increase score
Increase speed by adjusting it with the score (Bigger score, bigger speed)
Increase the number of the body parts of the snake and set the last part.

void resetMatrix()
This function turns of all the leds in the matrix, repositions the snake almost on the center of it and renders it and the food.

bool hasDied()
This function checks if the snake's head collides with on if it's body parts
If it does then return true;

int laserHitBody()
This function checks of the laser has collided with the snake, if it did then return the position of the body part that has been hit from "bodyPositions"
If not, return -1.

void shootLaser()
While in the game, it checks if the button is pressed
If it is then create the laser where the head is, and move it by deciding the direction of the snake.
While it hasn't hit any body part move it again until it does
When it does, all the body parts of the sanke starting there and all the way to the bottom get destroyed, and sets the number of body parts to the "positionHit".

void printGameMatrix()
This function lets the user play the snake game, it checks if the user ran out of lives or if the timer has reached 0
If the user ran out of lives the game is over and the user is prompted with the end screen.
If the user has died or ran out of the time he/she looses a life.

void loop()
This function is called each tick of the arduino, it checks where in the game you are by checking the flags and calling previouse mentioned functions.
