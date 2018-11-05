#include "address_map.h"
#include <unistd.h>
#include <stdlib.h>

/* The function below takes two arguments: value which is the value in decimal
 * to be displayed by the seven-segment displays and a character array that have the encoding
 * to display the provided value in decimal on the seven-segment displays. The provided value
 * can range from 0 - 999. The seven-segment displays must output the entire range correctly.
 */

//char sevenSegTable [10] = {
//		0xFC, //1 11111100
//		0x60, //2 01100000
//		0xDA, //3 11011010
//		0x66, //4 01100110
//		0xB6, //5 10110110
//		0xBE, //6 10111110
//		0xE0,  //7 11100000
//		0xFE,  //8 11111110
//		0xF6,  //9 11110110
//		0x01  //. 00000001
//};


int HEX3to0_display (int value, unsigned char * table);
int* toDigit(int value);


int main(void)
{
	/* Declare volatile pointers to the necessary I/O registers (volatile means that IO load
	 * and store instructions will be used to access these pointer locations,
	 * instead of regular memory loads and stores)
	 */

	/********** Your declarations should go below! ***********/

	volatile int * green_LED_ptr	= (int *) 0x10000010;	// green LED address
	volatile int * HEX3_HEX0_ptr	= (int *) 0x10000020;	// HEX3_HEX0 address
	volatile int * SW_switch_ptr	= (int *) 0x10000040;	// SW slider switch address
	volatile int * KEY_ptr			= (int *) 0x10000050;

	int HEX_bits = 0x0000000F;					// pattern for HEX displays

	int SW_value, KEY_value;
	volatile int delay_count;

	// An array containing values to convert a single digit decimal to the corresponding
	// seven_segment value. The first three elements (table[0] to table[2]) are correct;
	// the rest are not. You need to complete the array below with all the correct conversion values.

	unsigned char table[10] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
	//0x3F == 0011 1111 => 0
	//0x06 == 0000 0110 => 1
	//0x5B == 0101 1011 => 2
	//0x4F == 0100 1111 => 3
	//0x66 == 0110 0110 => 4
	//0x6D == 0110 1101 => 5
	//0x7D == 0111 1101 => 6
	//0x07 == 0000 0111 => 7
	//0x7F == 0111 1111 => 8
	//0x6F == 0110 1111 => 9

	unsigned char buttonTable[4] = {0x1, 0x2, 0x4, 0x8};
	//Button values
	//KEY0 = 1 = 0x1
	//KEY1 = 2 = 0x2
	//KEY2 = 4 = 0x4
	//KEY3 = 8 = 0x8
	//KEY0 & KEY2 = 5
	//KEY2 & KEY 3 = 12

	unsigned char switchTable[10] = {0x001, 0x002, 0x004, 0x008, 0x010, 0x020, 0x040, 0x080, 0x100, 0x200};
	//SWITCH values
	//SW0 = 0000 0000 0001 = 1 = 0x001
	//SW1 = 0000 0000 0010 = 2 = 0x002
	//SW2 = 0000 0000 0100 = 4 = 0x004
	//SW3 = 0000 0000 1000 = 8 = 0x008
	//SW4 = 0000 0001 0000 = 16 = 0x010
	//SW5 = 0000 0010 0000 = 32 = 0x020
	//SW6 = 0000 0100 0000 = 64 = 0x040
	//SW7 = 0000 1000 0000 = 128 = 0x080
	//SW8 = 0001 0000 0000 = 256 = 0x100
	//SW9 = 0010 0000 0000 = 512 = 0x200

	//LEDG...
	//LEDG0 = 0000 0001 = 0x01
	//LEDG1 = 0000 0010 = 0x02
	//LEDG2 = 0000 0100 = 0x04
	//LEDG3 = 0000 1000 = 0x08
	//LEDG4 = 0001 0000 = 0x10
	//LEDG5 = 0010 0000 = 0x20
	//LEDG7 = 0100 0000 = 0x40


	/* Declare your local variables that you may need. */
	/********** Your declarations should go below! ***********/




	while(1)
	{
		//		HEX_bits = 01101111011011110110111101101111; //999
		HEX_bits = HEX3to0_display(999, table);
		*(HEX3_HEX0_ptr) = HEX_bits;
		SW_value = *(SW_switch_ptr);		 	// read the SW slider switch values
		*(green_LED_ptr) = SW_value; 			// light up the green LEDs

		KEY_value = *(KEY_ptr); 				// read the pushbutton KEY values

		//When you push KEY1, the seven-segment displays should change from 999 to 5 (not 05 or 005) and LEDG5
		//should also light up for as long as you keep pushing the button. Once you release the button, it should
		//continue to display 999 and the green LED would stop illuminating.

		if (KEY_value == 2)						// check if any KEY was pressed
		{
			//HEX_bits = HEX3to0_display(5, table);
			*(green_LED_ptr) = 32;
			while(KEY_value == 2){
				*(HEX3_HEX0_ptr) = HEX3to0_display(5, table);

				if(!*(KEY_ptr)){
					*(green_LED_ptr) = 0;
				}
				if(*(KEY_ptr)){
					KEY_value = *(KEY_ptr);
				}
				//*(HEX3_HEX0_ptr) = HEX_bits;
			}

			//					while (*KEY_ptr){
			//												// wait for pushbutton KEY release
			//					}
			// display pattern on HEX3 ... HEX0
			usleep(150000); //Delays program execution for the given number of microseconds.

		}

		//When you push KEY2, the seven-segment displays should change from 999 to 10 (not 010) and LEDG6
		//should also light up for as long as you keep pushing the button. Once you release the button, it should
		//continue to display 999 and the green LED would stop illuminating.

		else if (KEY_value == 4){
			//					HEX_bits = HEX3to0_display(10, table);					// set pattern using HEX3to0_display
			//
			//					while (*KEY_ptr){
			//						*(green_LED_ptr) = 64;
			//					}
			//					*(HEX3_HEX0_ptr) = HEX_bits;			// display pattern on HEX3 ... HEX0
			//					usleep(150000); //Delays program execution for the given number of microseconds.
			//					KEY_value = *(KEY_ptr);

			*(green_LED_ptr) = 64;
			while(KEY_value == 4){
				*(HEX3_HEX0_ptr) = HEX3to0_display(10, table);

				if(!*(KEY_ptr)){
					*(green_LED_ptr) = 0;
				}
				if(*(KEY_ptr)){
					KEY_value = *(KEY_ptr);
				}
				//*(HEX3_HEX0_ptr) = HEX_bits;
			}

			//					while (*KEY_ptr){
			//												// wait for pushbutton KEY release
			//					}
			// display pattern on HEX3 ... HEX0
			usleep(150000); //Delays program execution for the given number of microseconds.

		}

		//When you push KEY3, the seven-segment displays should change from 999 to 25 (not 025) and LEDG7
		//should also light up for as long as you keep pushing the button. Once you release the button, it should
		//continue to display 999 and the green LED would stop illuminating.
		else if (KEY_value == 8){
//			HEX_bits = HEX3to0_display(25, table);					// set pattern using HEX3to0_display
//			*(green_LED_ptr) = 3; // set pattern using HEX3to0_display
//			while (*KEY_ptr){
//				*(green_LED_ptr) = 128;
//			}
//			//usleep(150000); //Delays program execution for the given number of microseconds.
//			*(HEX3_HEX0_ptr) = HEX_bits;			// display pattern on HEX3 ... HEX0
//			KEY_value = *(KEY_ptr);
			*(green_LED_ptr) = 128;
			while(KEY_value == 8){
				*(HEX3_HEX0_ptr) = HEX3to0_display(25, table);

				if(!*(KEY_ptr)){
					*(green_LED_ptr) = 0;
				}
				if(*(KEY_ptr)){
					KEY_value = *(KEY_ptr);
				}
				//*(HEX3_HEX0_ptr) = HEX_bits;
			}

			//					while (*KEY_ptr){
			//												// wait for pushbutton KEY release
			//					}
			// display pattern on HEX3 ... HEX0
			usleep(150000); //Delays program execution for the given number of microseconds.

		}

		//*(HEX3_HEX0_ptr) = HEX_bits;			// display pattern on HEX3 ... HEX0

		/* rotate the pattern shown on the HEX displays */

	}
}

/* The function below takes two arguments: value which is the value in decimal
 * to be displayed by the seven-segment displays and a character array that have the encoding
 * to display the provided value in decimal on the seven-segment displays. The provided value
 * can range from 0 - 999. The seven-segment displays must output the entire range correctly.
 */
int HEX3to0_display (int value, unsigned char * table)
{
	int displayHex = 0; //0xffffff
	int* digit = toDigit(value);
	displayHex = (((displayHex >> 24) | table[digit[0]] << 16) |((displayHex >> 16) |  table[digit[1]] << 24 >> 16) | (displayHex |  table[digit[2]])) ;
	//0x00FFFFFF
	if (value < 10){
		displayHex = displayHex & 0x000000FF;}
	if (value < 100){
		displayHex = displayHex & 0x0000FFFF;}
	return displayHex;
}


int* toDigit(int value){
	int* digit;
	digit = (int*) calloc(3,sizeof(int));
	int i;
	for(i = 2; i >= 0; i--){
		digit[i] = 0;
	}
	i = 2;
	while (value > 0){
		digit[i] = value % 10;
		value = value /  10;
		i--;
	}

	return digit;

}

