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

#define MAX_SCREEN_WIDTH 128
#define MAX_SCREEN_LENGTH 32

int mytime = 0x5957;

int x_dir = 1; 
int y_dir;
int x_pos;
int y_pos;
int length_snake = 0;
int snake[1][2];

char textstring[] = "text, more text, and even more text!";

int timeoutcounter = 0;


/* Interrupt Service Routine */
void user_isr( void )
{
  return;
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
  *trisEPointer = *trisEPointer & 0xffffff00; // volatile pointer sets bits 0 through 7 to outputs                     handles LEDS

  TRISD |= 0x07F0; // Sets bits 5 to 11 to inputs, bitwise or |= leaves other bits unchanged, 8-11 SWITCHES, 5-7 BTNS

  T2CON = 0x0000; // Reset configuration of timer 2 and turning it off 
  T2CONSET = 0x70; // sets prescaling to 1/256
  PR2 = 80000000/(256*10); // how many periods the timer counts     divides by 10 due to seconds/10 
  TMR2 = 0x0; // clears the counter value of timer 2 
  IFSCLR(0) = 0x0; // clears the interrupt flag status 
  T2CONSET = 0x8000; // sets bit 15 to 1 stating the clock 

  // clear incase of shit still left 
  display_string(0, "");
	display_string(1, "");
	display_string(2, "");
	display_string(3, "");
	display_update();
}

void snakeinit(void){
  x_pos = 1;//MAX_SCREEN_WIDTH/2;
  y_pos = MAX_SCREEN_LENGTH/2;
  one_pixel_update(x_pos,y_pos,1);
}

void gameinit(void){
  clear_display();
  display_board();
  draw_board();
  snakeinit();
}

void move_snake(void){
  //one_pixel_update(x_pos,y_pos,0);
  x_pos = x_pos + x_dir;
  one_pixel_update(x_pos,y_pos,1);
  length_snake++;
  if(length_snake == 5){

  }

}

/* This function is called repetitively from the main program */
void labwork( void )
{
  volatile unsigned *portEPointer = (unsigned int *) 0xbf886110; // volatile pointer that points to PORTE in memory 

  if(IFS(0) & 0x100){ // if interupt flag status have a 1 on the 9th bit reset 
    IFSCLR(0) = 0x100;
    timeoutcounter++;
  }  

  if(timeoutcounter == 10){
    clear_display();
    timeoutcounter = 0;
    tick( &mytime );
    display_board();
    draw_board();
    //snakeinit();
    move_snake();
    *portEPointer = *portEPointer + 0b1;
  }

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