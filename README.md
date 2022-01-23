# RelayRippleCarryAdder
Arduino code for use on the Mega2560 or Due microprocessors

Capable of 5-bit addition, 4 relays create the needed AND and XOR logic gates for calculating the sum of each bit addition.  Green LEDs display sum bits, blue LEDs display carry bits.  LCD screen used to display user controls and addition logic in a human readable format.  Microprocessor supplys the input bits of the A, B, and Cin parameters to the DPDT relay switches.

# User Inputs
5 buttons on the device contol the system.

Yellow Button: Selects Mode, test mode will cycle through a hard coded list of A and B inputs and adds them together in a loop.  User mode will allow the user to select input A and B and execute addition.

Red Button: Frequency control, system supports 1 Hz, 2 Hz, 4 Hz, 8 Hz, and 16 Hz.

Blue Button: User selection of A and B inputes, 1 press increments the inputs parameter by 1.

Green Button: Executes addition of the two inputs.


# Final Build
https://imgur.com/6XSl6ES
