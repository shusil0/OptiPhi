/*
Created by Ansar Khan Nov 2018
*/



#include "gpiolib_addr.h"
#include "gpiolib_reg.h"
#include "gpiolib_reg.c"
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

const int NUM_PINS = 27;//This corresponds to the hghest value GPIO pin available on the raspberry pi, although it is built for 53 inputs, this version has 27
const int NUM_PINS_BCM_2835 = 53;//Number of pins supported BCM 2835


/*
 *Function: isValidPin
 *this Function takes a pin number and returns whether or not it is valid to manipulate
 
 Input: pin number that needs to be tested
 Output:
 -1 if pin is out of range of given pi
 -2 if pin is out of range of BCM 2835
 -3 if pin is negative
 0 representing that the pin is a valid pin on the pi
 Note: Although the distinction between the three error codes is not used in thr project it may be neccecary for other applications
 
 */
int isValidPin(int pin){
    if(pin < 0)
        return -3;
    if(pin > NUM_PINS_BCM_2835)
        return -2;
    if(pin > NUM_PINS)
        return -1;
    return 0;
}


/*
 *Function: getFunctionSelectBit
 *This function takes in a pin number and and returns what place value on its corresponding selection register should be changed to set this bit
 *Should be used in conjunction with getSelectionRegister
 
 Input:
 pin: The pin number to return the function selection bit, must be in range of pins on the pi
 Output:
 -1 if pin is out of range of given pi
 -2 if pin is out of range of BCM 2835
 -3 if pin is negative
 int >= 0 representing place value of selection in selection register
 */
int getFunctionSelectBit(int pin){
    //NOTE: This function would only work if the number of pins in each register is going up by a constant amount -> NUM_PINS_IN_REG
    const int LENGTH_OF_FUNC_SELECT_FIELD = 3;//This corresponds to how long the function select field is
    const int NUM_PINS_IN_REG = 10;//This is the number of pins in each register
    int pinValidity = isValidPin(pin); //Validates input pin
    if(pinValidity)
        return pinValidity;//Returns error code if pin is invalid
    return LENGTH_OF_FUNC_SELECT_FIELD * (pin - (pin/NUM_PINS_IN_REG)* NUM_PINS_IN_REG);
    
}
/*
 *Function: getSelectionRegister
 *This function takes in a pin number and and returns what selection register should be used to change its functionality
 *Should be used in conjunction with getSelectionBit
 
 Input:
 pin: The pin number to return the function selection bit, must be in range of pins on the pi
 Output:
 -1 if pin is out of range of given pi
 -2 if pin is out of range of BCM 2835
 -3 if pin is negative
 int >= 0 representing the index of the selectionRegister
 */
int getSelectionRegister(int pin){
    //NOTE: This function would only work if the number of pins in each register is going up by a constant amount -> NUM_PINS_IN_REG
    
    const int NUM_PINS_IN_REG = 10;//This is the number of pins in each register
    
    int pinValidity = isValidPin(pin); //Validates input pin
    if(pinValidity)
        return pinValidity;//Returns error code if pin is invalid
    
    return pin/NUM_PINS_IN_REG;//Integer divion actually helps here
    
}

/*
 *Function: setAsOutput
 *This function takes in a pin number and sets that pin to an output on the raspberry pi
 
 Input:
 gpio: The pointer to the GPIO unsigned int, this allows for interfacing with the pi
 pin: The pin number to be set to be an output, must be in range of pins on the pi
 Output:
 0 if succesfull
 -1 if invalid pin
 -10 if GPIO is null
 
 */
int setAsOutput(uint32_t* gpio, int pin){
    if(gpio == NULL)
        return -10;
    int selectionRegisterBit = getFunctionSelectBit(pin);//Get selection register bit
    int selectionRegisterNum = getSelectionRegister(pin);//Get selection register bit
    if(selectionRegisterNum < 0 || selectionRegisterBit < 0)//Check if any functions above resolved in errors
        return -1;
    uint32_t selectionRegister = gpiolib_read_reg(gpio, GPFSEL(selectionRegisterNum));//get the current selection status to minipulate
    selectionRegister |= 1  << selectionRegisterBit; //Set the register to be a one at the right place
    gpiolib_write_reg(gpio, GPFSEL(selectionRegisterNum), selectionRegister);//Write the register value to the pi
    return 0;
}


int setAsInput(uint32_t* gpio, int pin){
    if(gpio == NULL)
        return -10;
    int selectionRegisterBit = getFunctionSelectBit(pin);//Get selection register bit
    int selectionRegisterNum = getSelectionRegister(pin);//Get selection register bit
    if(selectionRegisterNum < 0 || selectionRegisterBit < 0)//Check if any functions above resolved in errors
        return -1;
    uint32_t selectionRegister = gpiolib_read_reg(gpio, GPFSEL(selectionRegisterNum));//get the current selection status to minipulate
    printf("Register before %lu times \n", selectionRegister);
    selectionRegister &= ~ (1 << selectionRegisterBit); //Set the register to be a one at the right place
    printf("Register After %lu times \n", selectionRegister);
    gpiolib_write_reg(gpio, GPFSEL(selectionRegisterNum), selectionRegister);//Write the register value to the pi
    
    return 0;
}

/*
 *Function: getRegisterIndex
 *Returns the register that corresponds to the register index where the pin can be minuplated
 *In the case of the BCM 2835 this means that this will return which register index to use for the following registers, GPLEV, GPSET, GPCLR
 
 Input:
 pin: The pin number to return the function selection bit, must be in range of pins on the pi
 Output:
 -1 if pin is out of range of given pi
 -2 if pin is out of range of BCM 2835
 -3 if pin is negative
 0 if pin is between [0,31]
 1 if pin is between [32,53]
 
 */
int getRegisterIndex(int pin){
    int pinValidity = isValidPin(pin); //Validates input pin
    if(pinValidity)
        return pinValidity;//Returns error code if pin is invalid
    const int  NUM_PINS_IN_REG = 32;
    //At this point in code we know that pin is in range [0,53] which means it is safe to hardcode register 0 and 1
    if(pin < NUM_PINS_IN_REG){
        return 0;
    }
    else{
        return 1;
    }
}

/*
 *Function: getPinState
 returns whether or not a pin is on or not(gets pin state)
 It works by reading the levels register and returning the value at the corresponding index
 Input:
 gpio: The pointer to the GPIO unsigned int, this allows for interfacing with the pi
 pin: The pin number to return the function selection bit, must be in range of pins on the pi
 Output:
 -1 if pin is out of range of given pi
 -2 if pin is out of range of BCM 2835
 -3 if pin is negative
 -10 if GPIO is null
 0 OR 1 dependeing on pin state
 
 */
int getPinState(uint32_t* gpio, int pin){
    int pinValidity = isValidPin(pin); //Validates input pin
    if(pinValidity)
        return pinValidity;//Returns error code if pin is invalid
    if(gpio == NULL)
        return -10;
    int registerIndex = getRegisterIndex(pin);
    uint32_t levelsRegister = gpiolib_read_reg(gpio, GPLEV(registerIndex));
    return levelsRegister & (1 << pin);
    
}
/*
 Function: setPinState
 This function writes a value to a given pin
 With the current implementation, if the value to be wrttien is not a zero or one it will get booleanized and written(i.e anything that is not a zero or one will be written as a one)
 
 Input:
 gpio: The pointer to the GPIO unsigned int, this allows for interfacing with the pi
 pin: The pin number to return the function selection bit, must be in range of pins on the pi
 Output:
 -1 if pin is out of range of given pi
 -2 if pin is out of range of BCM 2835
 -3 if pin is negative
 -10 if GPIO is null
 0 if successful with no warnings
 1, warning, given if value to be written was not zero or one
 */
int setPinState(int value, uint32_t* gpio, int pin){
    int pinValidity = isValidPin(pin); //Validates input pin
    if(pinValidity)
        return pinValidity;//Returns error code if pin is invalid
    if(gpio == NULL)
        return -10;
    int registerIndex = getRegisterIndex(pin);
    int writeVal = !!value;
    if(writeVal){
        gpiolib_write_reg(gpio, GPSET(registerIndex), 1 << pin);
    }
    else{
        gpiolib_write_reg(gpio, GPCLR(registerIndex), 1 << pin);
    }
    if(value != writeVal)
        return 1;
    return 0;
}
/*
 Function: turnOn
 This function writes a one to a given pin
 
 Input:
 gpio: The pointer to the GPIO unsigned int, this allows for interfacing with the pi
 pin: The pin number to return the function selection bit, must be in range of pins on the pi
 Output:
 -1 if pin is out of range of given pi
 -2 if pin is out of range of BCM 2835
 -3 if pin is negative
 -10 if GPIO is null
 0 if successful
 */
int turnOn(uint32_t* gpio, int pin){
    return setPinState(1, gpio, pin);
}
/*
 Function: turnOff
 This function writes a zero to a given pin
 
 Input:
 gpio: The pointer to the GPIO unsigned int, this allows for interfacing with the pi
 pin: The pin number to return the function selection bit, must be in range of pins on the pi
 Output:
 -1 if pin is out of range of given pi
 -2 if pin is out of range of BCM 2835
 -3 if pin is negative
 -10 if GPIO is null
 0 if successful
 */
int turnOff(uint32_t* gpio, int pin){
    return setPinState(0, gpio, pin);
}
/*
 Function: flip
 This function flips the current status of the pin.
 If the pin is high, it gets set to low, and vice versa
 
 Input:
 gpio: The pointer to the GPIO unsigned int, this allows for interfacing with the pi
 pin: The pin number to return the function selection bit, must be in range of pins on the pi
 Output:
 -1 if pin is out of range of given pi
 -2 if pin is out of range of BCM 2835
 -3 if pin is negative
 -10 if GPIO is null
 0 if successful
 */
int flip(uint32_t* gpio, int pin){
    int pinState = getPinState(gpio,pin);
    if(pinState < 0)
        return pinState;
    if(pinState)
        return turnOff(gpio, pin);
    return turnOn(gpio, pin);
}
/*
 Function: delay
 This function sleeps the raspberry pi for a given amount of milliseconds
 rewrote usleep to work with milliseconds because that is easier to work with in other parts of code
 
 Input:
 time: the amount of time to sleep in milliseconds, if negative pi does not sleep
 Output:
 none
 */
void delay(float time){
    if(time <= 0)
        return;
    const uint32_t NANOSECONDS_IN_MILISECOND = 1000;
    time *= NANOSECONDS_IN_MILISECOND;
    usleep(time);
}
