/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   Updated 2024-02-27 by Joakim Sundman & Julius Larsson

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

#define MAX_SCREEN_WIDTH 128
#define MAX_SCREEN_LENGTH 32

int mytime = 0x5957;

// testing 

// Snake parameters 
int length_snake;
int snake_array[512][2];
int speed[2] = {1, 0};
int apple[2];
int dead;
int paused;
int seed = 4567;

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

  PORTESET = 0x0000; // sends 0 to port E

  T2CON = 0x0000; // Reset configuration of timer 2 and turning it off 
  T2CONSET = 0x70; // sets prescaling to 1/256
  PR2 = 80000000/(256*10); // how many periods the timer counts     divides by 10 due to seconds/10 
  TMR2 = 0x0; // clears the counter value of timer 2 
  IFSCLR(0) = 0x0; // clears the interrupt flag status 
  T2CONSET = 0x8000; // sets bit 15 to 1 stating the clock 
  IECSET(0) = 0x8000;           // Sets bit 16 to initialize 
}


//
void random_apple (void){
  int random_x;
  int random_y;
  int i; 
  int condition = 0;
  while(condition == 0){                      // Makes sure that randomized numbers are not in snake
    random_x = seed * 123456;
    random_x = random_x%126 + 1;
    random_y = seed * 56342;
    random_y = random_y%30 + 1;
    condition = 1;
    for(i = 0; i < 512; i++){
      if(snake_array[i][0] == random_x && snake_array[i][1] == random_y){
        condition = 0;
      }
      if(random_x < 0 || random_y < 0){
        condition = 0;
      }
      if(random_x > 126 || random_y > 126){
        condition = 0;
      }
    }
  }
  apple[0] = random_x;
  apple[1] = random_y;
}


//
void snakeinit(void){
  volatile unsigned *portEPointer = (unsigned int *) 0xbf886110; // volatile pointer that points to PORTE in memory 
  *portEPointer = 0;
  dead = 0; 
  int x_pos;
  int y_pos;
  x_pos = MAX_SCREEN_WIDTH/2;
  y_pos = MAX_SCREEN_LENGTH/2;
  length_snake = 4;
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


//
void gameinit(void){
  paused = 0; 
  clear_display();
  draw_board();
  snakeinit();
  random_apple();
  display_board();
}


//
void move_snake(void){
  int snake_head[2] = {snake_array[0][0] + speed[0], snake_array[0][1] + speed[1]}; // Svaes next position of the head 
  int apple_round = 0; 
  volatile unsigned *portEPointer = (unsigned int *) 0xbf886110; // volatile pointer that points to PORTE in memory

  if(snake_head[0] == apple[0] && snake_head[1] == apple[1]){
    apple_round = 1;
    *portEPointer = *portEPointer + 0b1;
    length_snake++;
  }

  if(snake_head[0] == 0 || snake_head[0] == 127 || snake_head[1] == 0 || snake_head[1] == 31){
    dead = 1; 
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
          dead = 1;
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
      draw_pixel(snake_array[i][0], snake_array[i][1]);
    }
  }
  draw_pixel(apple[0], apple[1]);
}

/* This function is called repetitively from the main program */
void labwork( void )
{
  volatile unsigned *portEPointer = (unsigned int *) 0xbf886110; // volatile pointer that points to PORTE in memory 
  int score;  

  if(IFS(0) & 0x100){ // if interupt flag status have a 1 on the 9th bit reset 
    IFSCLR(0) = 0x100;
    timeoutcounter++;
    if(seed >= 15000){
      seed = seed % 126;
    }
    seed = seed + 13;
  }  
  if(dead != 1 && paused != 1){
    if(timeoutcounter == 6){
      clear_display();
      timeoutcounter = 0;
      tick( &mytime );
      draw_board();
      move_snake();
      print_snake();
      display_board();
    }
  } else if(dead == 1 && paused != 1){
    if(timeoutcounter == 6){
      timeoutcounter = 0; 
      clear_display(); 
      tick( &mytime );
      score = (length_snake - 4) * 100;
      display_string(0,"Game Over  ");
      display_string(1,"Your Score:  ");
      display_string(2,itoaconv(score));
      display_string(3,"To Restart BTN1  ");
      display_update();
    }
  } else {
    if(timeoutcounter == 6){
      timeoutcounter = 0;
      clear_display(); 
      tick( &mytime );
      display_string(0,itoaconv(apple[0]));
      display_string(1,"  Paused  ");
      display_string(2,itoaconv(apple[1]));
      display_string(3,"  Switch SW2  ");
      display_update();
    }
  }

  int swPressed = getsw();
  int btnsPressed = getbtns();
  int currentSwConfig;

  if((btnsPressed & 0b1) && speed[1] == 0){ //BTN2
    speed[0] = 0;
    speed[1] = 1;
  }
  if((btnsPressed & 0b10) && speed[1] == 0){ //BTN3
    speed[0] = 0;
    speed[1] = -1;
  }
  if((btnsPressed & 0b100) && speed[0] == 0){ //BTN4 
    speed[0] = -1;
    speed[1] = 0;
  }
  if((btnsPressed & 0b1000) && speed[0] == 0 || (btnsPressed & 0b1000) && (dead == 1)){ //BTN1
    speed[0] = 1;
    speed[1] = 0;
    if(dead == 1){
      gameinit(); 
    }
  }

  if(swPressed & 0b1){
    if(dead != 1){
      paused = 1;
    }
  }
  if(swPressed & 0b10){
    paused = 0;
  }
}