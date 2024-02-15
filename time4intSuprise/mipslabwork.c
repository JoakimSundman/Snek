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

int prime = 1234567;

char textstring[] = "text, more text, and even more text!";

int timeoutcounter = 0;

/* Interrupt Service Routine */
void user_isr( void )
{
  volatile unsigned *portEPointer = (unsigned int *) 0xbf886110;

  if(IFS(0) & 0x100){ // acknowledge interupts
    IFSCLR(0) = 0x100;
    timeoutcounter++; // incremeant timeoutcounter when user_isr is called
  } 

  if(IFS(0) & 0x8000){
    IFSCLR(0) = 0x8000;
    *portEPointer = *portEPointer + 0b1;
  }
  
  if(timeoutcounter == 10){
    time2string( textstring, mytime );
    display_string( 3, textstring );
    display_update();
    tick( &mytime );
    timeoutcounter = 0;
  }
}

/* Lab-specific initialization goes here */
// TRISx    Output = 0  Input = 1
// TRISxSET tristate functionality 0, 1, impedance 
// TxCON    Control Register BIT | 15 | timer on | 6-4 | prescaling 111 -> 1/256
// PRx      Period Register  What counter stops at
// TMRx     Timer Value      Value of timer that is incremented at oscillations 
void labinit( void )
{
  volatile unsigned *trisEPointer = (unsigned int *) 0xbf886100; // volatile pointer that points to TRISE in memory
  *trisEPointer = 0x00; // volatile pointer sets bits 0 through 7 to outputs                     handles LEDS

  PORTE = 0x0000; // sends 0 to port E 

  TRISD |= 0x07F0; // Sets bits 5 to 11 to inputs, bitwise or |= leaves other bits unchanged, 8-11 SWITCHES, 5-7 BTNS

  T2CON = 0x0000; // Reset configuration of timer 2 and turning it off
  T2CONSET = 0x70; // sets prescaling to 1/256 
  PR2 = 80000000/(256*10); // how many periods the timer counts     divides by 10 due to seconds/10
  TMR2 = 0x0; // clears the counter value of timer 2 
  IFSCLR(0) = 0x0000; // clears the interrupt flag status
  T2CONSET = 0x8000; // sets bit 15 to 1 stating the clock
  IECSET(0) = 0x100;
  IECSET(0) = 0x8000;           // Sets bit 16 to initialize 
  IPCSET(2) = 0x1F;
  IPCSET(3) = 0xF800000;             // same priority as timer 2 interrupt 
  enable_interrupt();
  return;

}

/* This function is called repetitively from the main program */
void labwork( void )
{
  prime = nextprime( prime );
  display_string( 0, itoaconv( prime ) );
  display_update();
}