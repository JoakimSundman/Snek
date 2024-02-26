/* mipslabmain.c

   This file written 2015 by Axel Isaksson,
   modified 2015, 2017 by F Lundevall

   Latest update 2017-04-21 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

const uint8_t snake_no_tongue[] = {
		// page 1 
//  	1       2       3       4       5       6       7       8       9       10      11      12      13      14      15      16      17      18      19      20      21      22      23      24      25      26      27      28      29      30      31      32
/*R1*/	255,    255,    255,    15,    	239,    239,    225,    29,    	29,    	157,    157,    157,    157,    157,    29,    	225,    227,    239,    15,     31,     255,    255,    255,    1,    	253,    253,    253,    225,    227,    239,    239,    7,
/*R2*/	31,    	127,    127,    255,    255,    1,    	253,    253,    253,    1,    	3,    	255,    255,    255,    1,    	253,    253,    253,    29,    	29,    	157,    157,    157,    157,    157,    157,    157,    157,    157,    129,    131,    255,
/*R3*/	255,    255,    1,    	253,    253,    253,    1,    	3,    	255,    255,    255,    255,    255,    1,    	253,    253,    253,    1,    	3,    	255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,
/*R4*/	255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    63,    	207,    225,    252,    254,    255,    255,    255,    255,

		// page 2 
/*R1*/	255,    255,    255,    192,    159,    159,    31,    	224,    224,    239,    239,    239,    239,    239,    238,    12,    	28,    	124,    124,    255,    255,    255,    255,    0,    	255,    255,    255,    0,    	0,    	252,    240,    231,
/*R2*/	231,    135,    56,    	56,    	59,    	56,    	255,    255,    255,    0,    	0,    	255,    255,    255,    0,    	255,    255,    224,    224,    239,    239,    239,    239,    239,    255,   	15,    	31,    	255,    255,    255,    255,    255,
/*R3*/  255,    255,    0,    	255,    255,    255,    224,    224,    239,    239,    225,    29,    	157,    156,    131,    131,    243,    240,    240,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,	255,
/*R4*/	255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    127,    127,    63,    	143,    224,    255,    255,    255,    255,    255,    255,    255,    255,
		// page 3 
/*R1*/	255,    255,    255,    31,    	223,    223,    222,    30,    	60,    	252,    252,    252,    252,    252,    0,    	255,    255,    255,    0,    	0,    	255,    255,    255,    0,    	255,    255,    255,    0,    	0,    	255,    255,    255,
/*R2*/	255,    255,    255,    255,    255,    0,    	255,    255,    255,    0,    	0,    	255,    255,    255,    0,    	255,    255,    255,    1,    	249,    249,    249,    249,    249,    249,    248,    248,    255,    255,    255,    255,    255,
/*R3*/	255,    255,    0,    	255,    255,    255,    1,    	1,    	249,    249,    225,    206,    206,    14,    	240,    241,    247,    7,    	15,    	255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,
/*R4*/	15,    	247,    243,    251,    249,    253,    253,    226,    226,    226,    253,    253,    253,    253,    126,    14,    	254,    254,    254,    255,    255,    255,    255,    255,    255,    255,    127,    127,    127,    191,    207,    227,
		// page 4 
/*R1*/	255,    255,    255,    252,    249,    249,    225,    206,    206,    206,    206,    206,    206,    206,    206,	193,    193,    249,    248,    248,    255,    255,    255,    224,    207,    207,    207,    192,    192,    255,    255,    255,
/*R2*/	255,    255,    255,    255,    255,    224,    207,    207,    207,    192,    192,    255,    255,    255,    224,    207,    207,    207,   	206,    206,    206,    206,    206,    206,    206,    206,    206,    206,    206,    192,    193,    255,
/*R3*/	255,    255,    224,    207,    207,    207,    192,    192,    255,    255,    255,    255,    255,    224,    207,    207,    207,    192,    192,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,
/*R4*/	254,    253,    243,    241,    229,    237,    237,    237,    237,    237,    237,    237,    238,    230,    246,    247,    247,    243,    251,    251,    251,    253,    253,    253,    254,    254,    255,    255,    255,    255,    255,    255,
};

const uint8_t snake_with_tongue[] = {
		// page 1 
//  	1       2       3       4       5       6       7       8       9       10      11      12      13      14      15      16      17      18      19      20      21      22      23      24      25      26      27      28      29      30      31      32
/*R1*/	255,    255,    255,    15,    	239,    239,    225,    29,    	29,    	157,    157,    157,    157,    157,    29,    	225,    227,    239,    15,     31,     255,    255,    255,    1,    	253,    253,    253,    225,    227,    239,    239,    7,
/*R2*/	31,    	127,    127,    255,    255,    1,    	253,    253,    253,    1,    	3,    	255,    255,    255,    1,    	253,    253,    253,    29,    	29,    	157,    157,    157,    157,    157,    157,    157,    157,    157,    129,    131,    255,
/*R3*/	255,    255,    1,    	253,    253,    253,    1,    	3,    	255,    255,    255,    255,    255,    1,    	253,    253,    253,    1,    	3,    	255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,
/*R4*/	255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    63,    	207,    225,    252,    254,    255,    255,    255,    255,

		// page 2 
/*R1*/	255,    255,    255,    192,    159,    159,    31,    	224,    224,    239,    239,    239,    239,    239,    238,    12,    	28,    	124,    124,    255,    255,    255,    255,    0,    	255,    255,    255,    0,    	0,    	252,    240,    231,
/*R2*/	231,    135,    56,    	56,    	59,    	56,    	255,    255,    255,    0,    	0,    	255,    255,    255,    0,    	255,    255,    224,    224,    239,    239,    239,    239,    239,    255,   	15,    	31,    	255,    255,    255,    255,    255,
/*R3*/  255,    255,    0,    	255,    255,    255,    224,    224,    239,    239,    225,    29,    	157,    156,    131,    131,    243,    240,    240,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,	255,
/*R4*/	255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    127,    127,    63,    	143,    224,    255,    255,    255,    255,    255,    255,    255,    255,
		// page 3 
/*R1*/	255,    255,    255,    31,    	223,    223,    222,    30,    	60,    	252,    252,    252,    252,    252,    0,    	255,    255,    255,    0,    	0,    	255,    255,    255,    0,    	255,    255,    255,    0,    	0,    	255,    255,    255,
/*R2*/	255,    255,    255,    255,    255,    0,    	255,    255,    255,    0,    	0,    	255,    255,    255,    0,    	255,    255,    255,    1,    	249,    249,    249,    249,    249,    249,    248,    248,    255,    255,    255,    255,    255,
/*R3*/	255,    255,    0,    	255,    255,    255,    1,    	1,    	249,    249,    225,    206,    206,    14,    	240,    241,    247,    7,    	15,    	255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,
/*R4*/	15,    	247,    243,    251,    249,    253,    253,    226,    226,    226,    253,    253,    253,    253,    126,    14,    	254,    254,    254,    255,    255,    255,    255,    255,    255,    255,    127,    127,    127,    191,    207,    227,
		// page 4 
/*R1*/	255,    255,    255,    252,    249,    249,    225,    206,    206,    206,    206,    206,    206,    206,    206,	193,    193,    249,    248,    248,    255,    255,    255,    224,    207,    207,    207,    192,    192,    255,    255,    255,
/*R2*/	255,    255,    255,    255,    255,    224,    207,    207,    207,    192,    192,    255,    255,    255,    224,    207,    207,    207,   	206,    206,    206,    206,    206,    206,    206,    206,    206,    206,    206,    192,    193,    255,
/*R3*/	255,    255,    224,    207,    207,    207,    192,    192,    255,    255,    255,    255,    255,    224,    207,    207,    207,    192,    192,    255,    255,    255,    255,    255,    255,    255,    255,    255,    255,    207,    175,    199,
/*R4*/	242,    252,    243,    241,    229,    237,    237,    237,    237,    237,    237,    237,    238,    230,    246,    247,    247,    243,    251,    251,    251,    253,    253,    253,    254,    254,    255,    255,    255,    255,    255,    255,
};


int main(void) {
        /*
	  This will set the peripheral bus clock to the same frequency
	  as the sysclock. That means 80 MHz, when the microcontroller
	  is running at 80 MHz. Changed 2017, as recommended by Axel.
	*/
	SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
	while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
	while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
	SYSKEY = 0x0;  /* Lock OSCCON */
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
    SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;

	
	display_init();
	display_image(0, snake_no_tongue);
	quicksleep(5000000);
	display_update();
	display_image(0, snake_with_tongue);
	quicksleep(5000000);
	display_update();
	display_image(0, snake_no_tongue);
	quicksleep(5000000);
	display_update();
	display_image(0, snake_with_tongue);
	quicksleep(5000000);
	display_update();
	display_image(0, snake_no_tongue);
	quicksleep(5000000);
	display_update();
	
	labinit(); /* Do any lab-specific initialization */
	gameinit();

	int i = 0;

	while( 1 )
	{
	  labwork(); /* Do lab-specific things again and again */
	}
	return 0;
}



