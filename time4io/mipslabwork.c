/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void )
{
  return;
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  volatile unsigned *trisEPointer = (unsigned int *) 0xbf886100;
  *trisEPointer = 0x00; // bits 0 to 7 initilized to 0

  TRISD |= 0x07F0; // bits 11 through 5 initialized to 1
  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  volatile unsigned *portEPointer = (unsigned int *) 0xbf886110;
  delay( 1000 );
  time2string( textstring, mytime );
  display_string( 3, textstring );
  display_update();
  tick( &mytime );
  display_image(96, icon);

  *portEPointer = *portEPointer + 0b1;

  int swPressed = getsw();
  int btnsPressed = getbtns();
  int currentSwConfig;

  if(btnsPressed & 0b1){ //BTN2
    currentSwConfig = swPressed << 4;
    mytime = mytime & 0xFF0F;
    mytime += currentSwConfig;
  }
  if(btnsPressed & 0b10){ //BTN3
    currentSwConfig = swPressed << 8;
    mytime = mytime & 0xF0FF;
    mytime += currentSwConfig;
  }
  if(btnsPressed & 0b100){ //BTN4
    currentSwConfig = swPressed << 12;
    mytime = mytime & 0x0FFF;
    mytime += currentSwConfig;
  }
}
