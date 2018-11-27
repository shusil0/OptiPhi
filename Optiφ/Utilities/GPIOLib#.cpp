#include <stdio.h>
#include "GPIO++.c"
#include <time.h>
#include "BinaryArray.cpp"
#include "gpiolib_addr.h"
#include "gpiolib_reg.h"
#include "gpiolib_reg.c"

// Initilize the GPIO
GPIO_Handle initializeGPIO() {
    GPIO_Handle gpio;
    gpio = gpiolib_init_gpio();
    
    if(gpio == NULL) {
        perror("Could not initialize GPIO");
    }
    return gpio;
}

void setOutputPin(uint32_t* gpio, int pinNumber){
    
    // This line sets the value of select Register that the pin is in to the int variable reg
    int reg = (pinNumber/10);
    
    /* In order to set the required pin as an output the bits that corresponds to such pins must be changed,
     therefore this line returns the bits for the required pin from the respected register
     */
    
    int bits = (3*(pinNumber%10));
    
    // this line is creating a variable that contains exactly 32 bits named sel_reg. The 32 bits are taken from whatever the 32 bits of select register are.
    uint32_t sel_reg = gpiolib_read_reg(gpio, GPFSEL(reg));
    
    // The line "sel_reg |= 1" below is using a bitwise OR operation to change the respected bit in respected sel_reg to 1. It will not change the values of any other bit in sel_reg.
    sel_reg |= 1 << bits;
    
    /* The line "gpiolib_write_reg(gpio, GPFSEL(1), sel_reg)" below is writing sel_reg into respected select register.
     This means that the select register will be unchanged except that required(sent in) bit will now be a 1.
     */
    gpiolib_write_reg(gpio, GPFSEL(0), sel_reg);
}


// This boolean function decides whether to turn on or off based on the data sent in
bool turnOnOrOff(BinaryArray array, uint32_t* gpio, int LASER){
    
    // Goes through all the elements in an array
    for (int i = 0; i < array.getLength; i++){
        
        if(array.getData[i] == 1){
            
            turnOn(gpio, LASER);
            delay(array.getTime);
            
        }else if(array.getData[i] == 0){
            
            turnOff(gpio, LASER);
            delay();
            
        }else{
            cout << "NOT BINARY " << end;
        }
    }
    return true;
}


