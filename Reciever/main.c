#include "state.h"
#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "LED.h"
#include "EXTI.h"
#include "reg.h"

/*
 * This program reads in a preamble of 111 and then begins to read in the binary array of values from
 * the photo-resistor. After reading in 32 bits, it converts the binary number to a decimal number
 * and finally sends the decimal number to register r0 via a asm function file so that the value can
 * be viewed in the array as a hexadecimal value.
*/

uint32_t data[32];		//Initializing the array of 32 bits to be rcvd.
int n = 0;			//int n is for use in the parsing through the array
int preamble = 0;	//Preamble initialization
int value = 0;		//value of the final number of the binary array rcvd
theSTATE = -1;

extern void reg0(int a);	//register function from reg.s asm file

int main()
{
	//These next four lines are initialization for the necessary states, LEDs for verification,
	//external interrupt, and the SysTick which keep the program running.

	theSTATE = STANDBY;
	LED_Init();
	EXTI_Init();
	SysTick_Init();

while(1)
{
		//System_Clock_Init(); // Switch System Clock = 80 MHz

		switch(theSTATE)		//Switch for the state to progress program
		{

		/*****************************STANDBY************************************
		* Case STANDBY checks if any 1 has been flashed from Tx LED If 1 is rcvd,
		*the state becomes PREAMBLE to check if preamble has passed.
		************************************************************************/
			case STANDBY:
				if ((GPIOA -> IDR & 1UL) == 1)		//If 1 is detected by the photo-resistor,
													//else stay at STANDBY.
				{
					theSTATE = PREAMBLE;

				}

				break;

		/*****************************PREAMBLE************************************
		* Case PREAMBLE checks if 111 has been flashed from Tx LED. If 111 is rcvd,
		*the state becomes STR begin and store the binary number into the array.
		**************************************************************************/
			case PREAMBLE:

				while(preamble < 4)					//Preamble of 3 because 1 was already
				{									//detected in the previous state
					if ((GPIOA -> IDR & 1UL) == 1)	//If photo-resistor detects a one,
					{
						preamble++;					//increase the preamble number, and
						Green_LED_On();				//Turn on LED to indicate rcvd.
						delay(525);
					}
					else
					//if ((GPIOA -> IDR & 1UL) == 0)
						{
						Green_LED_Off();			//If zero rcvd, LED is turned off.
						delay(525);
						}
				}
					if (preamble >= 3)				//If the preamble is 3, rcvd preamble
					{								//of 1111

						theSTATE = STR;				//STATE is STR to store the binary number.
					}

				break;

		/********************************STR*************************************
		* Case STR checks if any 1 has been flashed from Tx LED If 1 is rcvd,
		*1 is inserted into the array and Green LED turns on to indicate such.
		*If nothing is flashed, a zero is inserted into the array and the Green
		*LED is turned off in order to indicate that a zero has been rcvd.
		************************************************************************/
			case STR:
				while(n <= 31)						//While under 32 bits rcvd (max of array)
				{
					if((GPIOA -> IDR & 1UL) == 0 )	//If LED is not flashed, logical 0.
					{
						delay(250);					//Delay in order to keep synchronized to Tx.
						data[n] = 0;				//Input 0 to the current position of array
						Green_LED_Off();			//Acknowledgment of logical 0
						n++; 						//Increase the size/position by 1
					}

					else //if((GPIOA -> IDR & 1UL) == 1 )	//If LED is flashed, logical 1.
					{
						delay(250);					//Delay in order to keep synchronized to Tx.
						data[n] = 1;				//Input 1 to the current position of array
						Green_LED_On();				//Acknowledgment of logical 1
						n++; 						//Increase the size/position by 1
					}
				}
				//delay(100);
				theSTATE = REG;						//Once 32 bits are captured, STATE = REG.

				break;

		/********************************REG*************************************
		* Case REG takes the array of binary numbers and parses through converting
		*binary number in array to a decimal number to send to the register.
		*Once converted and stored in Value variable, it calls the reg0 function
		*from asm file and sends over the number to register r0.
		************************************************************************/
			case REG:
				//For loop parses through array and converts the binary number of
				//32 bits into the decimal number equivalent.

				for(int i = 0; i < 32; i++)
				{
					value <<= 1;
					value += data[i];
				}

				reg0(value);					//Call to asm function in file reg.s
				value = 0; n = 0; 				//Reseting variables for next number.
				theSTATE = STANDBY;				//Returns back to STANDBY for next # to be rcvd.

				break;

			default:
				break;
		}
	}
}
