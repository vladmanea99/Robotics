Homework 1(13 LED)

I connect the LED to the 13th pin in arduino with anode and with cathode at ground and I put a rezistor to not burn the LED.
I then digitalWrite the 13th pin so the light of the LED would turn on or off

Homework 3 (RGB)

So the potentiometere has a ground pin, an input voltage pin and in input of analog readeable pin.

I attach 3 potentiometers to the arduino board in the A0 A1 and A2 as INPUTS

The RGB LED is cathod so the 3 pins for colors need to get a voltage through the wires and the longes pin is the one that is connected to the ground

I connect the 3 pins of colors through wires to the 3, 5 and 6 digital sockets in arduino and make them As an Analog Output.

I read the values of the 3 potentiometers in arduino, I map them to be between (0, 255) and then I connect each potentiometere to a color for the RGB LED.


Homework 4 (Buzzer)

This is the homework for the lab with the buzzers.

I intake in an AnalogInput pin in arduino a buzzer so it can record vibrations.

If the input taken is greater than 600 than it'll wait 5 seconds and will play on the second buzzer a song hardcoded in the program.

The notes are predefined with the number of Hz of each required freq for each note.


Homework 5 (4 digit 7 LED)

To use the 4 digit 7 LED I had to connect all the 12 pins of it on Arduino

The D1, D2, D3, D4 that allow me to control which of the 4 displays is working at a certain moment were put in the 2-5 digital pins of arudino. The rest of 8 pins I put them in the next 8 available slots in arduino (6-13).

With the first four I would tell the board to let only one display opened while I write a number with the pins A-G + DotPin.
I would do so with the first display, shut it down, open the second one, write a number, shut it down, etc.

I stored the numbers in a bidimensional array as 0s and 1s.

I connected a joystick in A0 for the X movement, A1 for the Y movement and A2 to tell if the button is pressed or not.

I then check if a display is locked or not. 
If it is, I then allow the movement of the X axis to be recorded and used to switch numbers on that display.

If the button is pressed again, I unlock that certain display, making the movement on the X axis uncontrollable.
Moreover I let the control of the movement on the Y axis to switch to a different display.

If the button is pressed again, the new display gets locked one.
