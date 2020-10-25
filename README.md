# Li-Fi_Pro
Communication method between two STM32L476G boards across light from an LED. 

This project uses C and ASM. 

Execute from main.c in STM32 Eclipse Workbench on each device, TX and RX respectfully. 

On the transmitter side, an interrupt is called once the joystick button is pressed and
causes the LED to transmit values after the conditions of the preamble are met. 

The receiver side begins to accept 1’s and 0’s based on the LED being on and off, respectively. This is made
possible through the use of interrupts and a clock, SysTick, which work concurrently to fill an
array with values that are read from the photo-resistor. Once all array values have been received,
an assembly function is called to store the content of this array into r0 on the receiver side.




**Please see state machines and flow diagrams below:**



<img width="410" alt="Screen Shot 2020-10-25 at 2 29 11 AM" src="https://user-images.githubusercontent.com/63037678/97100479-4979de00-166a-11eb-8a47-60ae312e2f51.png">

<img width="719" alt="Screen Shot 2020-10-25 at 2 29 52 AM" src="https://user-images.githubusercontent.com/63037678/97100484-51d21900-166a-11eb-975d-354f75251d0d.png">


<img width="430" alt="Screen Shot 2020-10-25 at 2 30 19 AM" src="https://user-images.githubusercontent.com/63037678/97100487-5565a000-166a-11eb-86f7-013c7e332506.png">


<img width="876" alt="Screen Shot 2020-10-25 at 2 30 34 AM" src="https://user-images.githubusercontent.com/63037678/97100490-57c7fa00-166a-11eb-8c20-08170bb8f1f9.png">
