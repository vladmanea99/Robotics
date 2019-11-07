So the potentiometere has a ground pin, an input voltage pin and in input of analog readeable pin.

I attach 3 potentiometers to the arduino board in the A0 A1 and A2 as INPUTS

The RGB LED is cathod so the 3 pins for colors need to get a voltage through the wires and the longes pin is the one that is connected to the ground

I connect the 3 pins of colors through wires to the 3, 5 and 6 digital sockets in arduino and make them As an Analog Output.

I read the values of the 3 potentiometers in arduino, I map them to be between (0, 255) and then I connect each potentiometere to a color for the RGB LED.
