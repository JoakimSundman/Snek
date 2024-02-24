/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <stdio.h>
#include <stdlib.h>
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

#define MAX_SCREEN_WIDTH 128
#define MAX_SCREEN_LENGTH 32

int mytime = 0x5957;

// Snake parameters 
int x_dir; 
int y_dir;
int x_pos;
int y_pos;
int length_snake = 4;
int snake_array[512][2];
int speed[2] = {1, 0};
int apple[2] = {54, 12};
int Dead;

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

  TRISD |= 0x07F0; // Sets bits 5 to 11 to inputs, bitwise or |= leaves other bits unchanged, 8-11 SWITCHES, 5-7 BTNS 2-4
  TRISF |= 0x1;   // Sets bit 1 to input for BTN1 

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
  Dead = 0; 
  x_pos = MAX_SCREEN_WIDTH/2;
  y_pos = MAX_SCREEN_LENGTH/2;
  x_dir = 1;
  y_dir = 0;
  int i;
  for(i = 0; i < 512; i++){
    snake_array[i][0] = 0;
    snake_array[i][1] = 0;
  }
  for(i = 0; i < length_snake; i++){
    snake_array[i][0] = x_pos - i;
    snake_array[i][1] = y_pos;
  }
}

void gameinit(void){
  clear_display();
  draw_board();
  snakeinit();
  display_board();
}

/*void random_apple(){
    int condition = 0;
    int random_x;
    int random_y;
    int i;
    while(condition == 0){                      // Makes sure that randomized numbers are not in snake
        condition = 1;
        srand(time(NULL));                      // Randomizes Numbers
        random_x = (rand() % 126) + 1;
        random_y = (rand() % 30) + 1;
        for(i = 0; i < 512; i++){
            if(snake_array[i][0] == random_x & snake_array[i][1] == random_y){
                condition = 0;
            }
        }
    }
    apple[0] = random_x;
    apple[1] = random_y;
}*/

void random_apple(){
  int x = 54;
  int y = 12;

  int x2 = 17;
  int y2 = 12;

  if(apple[0] == x && apple[1] == y){
    apple[0] = x2;
    apple[1] = y2;
  } else if(apple[0] == x2 && apple[1] == y2){
    apple[0] = x;
    apple[1] = y;
  }

}

void move_snake(void){
  int snake_head[2] = {snake_array[0][0] + speed[0], snake_array[0][1] + speed[1]}; // Svaes next position of the head 
  int apple_round = 0; 

  if(snake_head[0] == apple[0] && snake_head[1] == apple[1]){
    apple_round = 1;
  }

  if(snake_head[0] == 0 || snake_head[0] == 127 || snake_head[1] == 0 || snake_head[1] == 31){
    Dead = 1; 
  }

  int i;
  int new_array[512][2];
  for(i = 0; i < 512; i++){
      new_array[i][0] = 0;
      new_array[i][1] = 0;
  }                                                       // Array to hold new snake
  new_array[0][0] = snake_head[0]; new_array[0][1] = snake_head[1];                   // Adds the new position as the head of the new snake
  for(i = 0; i < 512; i++){                                                       // Moves the actual snake
      // Stops the copying of the snake, cheks for apples and handles either deleteion of the tail or brings it along if there is an apple
      if(apple_round == 1 && snake_array[i][0] == 0 && snake_array[i][1] == 0 || apple_round == 0 && snake_array[i+1][0] == 0 && snake_array[i+1][1] == 0){
          break;
      } else {
          new_array[i + 1][0] = snake_array[i][0];
          new_array[i + 1][1] = snake_array[i][1];
      }
  }

  for(i = 1; i < 512; i++){                   // Checks if collition with self
      if(snake_head[0] == new_array[i][0] && snake_head[1] == new_array[i][1]){
          Dead = 1;
      }
  }

  int j;
  for(i = 0; i < 512; i++){                   // Copies new snake to old snake
    for(j = 0; j < 2; j++){
      snake_array[i][j] = new_array[i][j];
    }
  }
  if(apple_round == 1){
    random_apple();
  }
}

void print_snake(void){
  int i;
  for(i = 0; i < 512; i++){
    if(snake_array[i][0] != 0 && snake_array[i][1] != 0){
      one_pixel_update(snake_array[i][0], snake_array[i][1], 1);
    }
  }
  one_pixel_update(apple[0], apple[1], 1);

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
    draw_board();
    move_snake();
    print_snake();
    display_board();
    *portEPointer = *portEPointer + 0b1;
  }

  int swPressed = getsw();
  int btnsPressed = getbtns();
  //int btnPressed = getbtn();
  int currentSwConfig;

  if(btnsPressed & 0b1000){ //BTN1
    speed[0] = -1;
    speed[1] = 0;
  }
  if(btnsPressed & 0b1){ //BTN2
    speed[0] = 1;
    speed[1] = 0;
  }
  if(btnsPressed & 0b10){ //BTN3
    speed[0] = 0;
    speed[1] = -1;
  }
  if(btnsPressed & 0b100){ //BTN4
    speed[0] = 0;
    speed[1] = 1; 
  }
}