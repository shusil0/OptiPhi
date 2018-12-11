#include "gpiolib_addr.h"
#include "gpiolib_reg.h"
#include "gpiolib_reg.c"

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

/*
 Shusil Shapkota
 2018-11-30
 */

// Basic uSleep function, that takes seconds as its input
void sleepIn(int seconds){
    
    usleep(seconds * 1000000);
    
}

// This function helps to set different pins as an output
// Parameters are gpio, initialization of GPIO pins, and the pinNumber that we want to set as an output

void setOutputPin(uint32_t* gpio, int pinNumber){
    
    // This line sets the value of select Register that the pin is in to the int variable reg
    int reg = (pinNumber/10);
    
    // In order to set the required pin as an output the bits that corresponds to such pins must be changed, therefore this line returns the bits for the required pin from the respected register
    int bits = (3*(pinNumber%10));
    
    // this line is creating a variable that contains exactly 32 bits named sel_reg. The 32 bits are taken from whatever the 32 bits of select register are.
    uint32_t sel_reg = gpiolib_read_reg(gpio, GPFSEL(reg));
    
    // The line "sel_reg |= 1" below is using a bitwise OR operation to change the respected bit in respected sel_reg to 1. It will not change the values of any other bit in sel_reg.
    sel_reg |= 1 << bits;
    
    // The line "gpiolib_write_reg(gpio, GPFSEL(1), sel_reg)" below is writing sel_reg into respected select register. This means that the select register will be unchanged except that required(sent in) bit will now be a 1.
    gpiolib_write_reg(gpio, GPFSEL(0), sel_reg);
}

// This function sets the sent-in bits in the clear Register to 1
void clearBit(uint32_t* gpio, int bits){
    gpiolib_write_reg(gpio, GPCLR(0), 1 << bits);
}

// This function sets the sent-in bits in the set Register to 1
void setBit(uint32_t* gpio, int pinNum){
    gpiolib_write_reg(gpio, GPSET(0), 1 << pinNum);
}

// Turns on the required LED(PIN)
int turnOn(uint32_t* gpio, int pinNum){
    setBit(gpio, pinNum);
}

// Turns off the required LED(PIN)
int turnOff(uint32_t* gpio, int pinNum){
    clearBit(gpio, pinNum);
}

// This function takes in the numberOfTimes user wants the blinks, pinNumber that corresponds to respected LED, and the initialized gpio
void blink(uint32_t* gpio, int counter, int count, int pinNum){
    //Loop to blink the LED 5 times
    while(counter < count)
    {
        //Turn on the LED by changing the 17th bit in the set register to 1
        turnOn(gpio, pinNum);
        
        //Pause the program for 1000000 microseconds, i.e. 1 second
        usleep(1000000);
        
        //Turn the LED off by setting the 17th bit in the clear register to 1
        turnOff(gpio, pinNum);
        
        //Pause the program for 1000000 microseconds, i.e. 1 second
        usleep(1000000);
        
        counter ++;
    }
}

// This function checks the current pin state, it will return 0 or 1
int checkPinState(uint32_t* gpio, int pinNum){
    
    uint32_t level_reg = gpiolib_read_reg(gpio, GPLEV(0));
    
    // Declaring a variable called pin_state and assigning the value of bit 4 from the level_reg
    int pin_state = level_reg & (1 << pinNum);
    return pin_state;
}
