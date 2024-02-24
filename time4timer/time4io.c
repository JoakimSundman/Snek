#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h" 

int getsw(void){
    int swReturn;
    swReturn = (PORTD >> 8) & 0xF; 
    return swReturn;
}

int getbtns(void){
    int btnsReturn;
    btnsReturn = (PORTD >> 5) & 0x7;
    btnsReturn |= ((PORTF >> 1) & 0x1) << 3;
    return btnsReturn;
}