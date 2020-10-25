# Li-Fi_Pro
Communication method between two STM32L476G boards across light from an LED. 


On the transmitter side, an interrupt is called once the joystick button is pressed and
causes the LED to transmit values after the conditions of the preamble are met. 

The receiver side begins to accept 1’s and 0’s based on the LED being on and off, respectively. This is made
possible through the use of interrupts and a clock, SysTick, which work concurrently to fill an
array with values that are read from the photo-resistor. Once all array values have been received,
an assembly function is called to store the content of this array into r0 on the receiver side.
