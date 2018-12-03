#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <iostream>     // std::cout
#include "../Utilities/BinaryArray.cpp"
#include "../Utilities/GPIO++.c"
#include "ReadWrite.cpp"

// const BinaryArray ON;
// const BinaryArray OFF;

using namespace std;

void sendBits(BinaryArray dataToSend, uint32_t* gpio);

int main(const int argc, const char *const argv[]) {
  int unneccecaryVariable = 0;
  int delay = 0;
  int outputPin = 0;
  int length = 0;
  int* newArray = new int[5];

  readConfigFile(unneccecaryVariable, delay, outputPin, unneccecaryVariable, newArray, unneccecaryVariable);
  cout<< "Output " << outputPin<<endl;
  cout<< "Delay: " << delay<<endl;

    

  GPIO_Handle gpio;
	gpio = gpiolib_init_gpio();
  setAsOutput(gpio, outputPin);
  DELAY_MICRO = delay;
  BinaryArray outputArray;
  outputArray += outputArray.newCharArray;
  for (int k = 1; k < argc; k++) {
    cout << "****" << endl;
    const char * str = argv[k];
    int i = 0;
    int* currentChar = new(std::nothrow) int[CHARACTER_LENGTH];
    if(currentChar == nullptr){
      cout<<"Current Char Too Big"<<endl;
    }
    while (str[i]) {

      for (int j = 0; j < 8; j++) {
        currentChar[j] = 0;
      }
      currentChar = convertToBinaryArray(str[i]);
      for (int j = 0; j < CHARACTER_LENGTH; j++) {
        outputArray += currentChar[j];
      }
      outputArray += outputArray.newCharArray;
      i++;
    }
    if (k != argc - 1) {
      currentChar = convertToBinaryArray(' ');
      for (int j = 0; j < CHARACTER_LENGTH; j++) {
        outputArray += currentChar[j];
      }
      outputArray += outputArray.newCharArray;
    }
    if (k == argc-1){
        currentChar = convertToBinaryArray('\n');
        for (int j = 0; j < CHARACTER_LENGTH; j++) {
          outputArray += currentChar[j];
        }
    }


  }
  cout << outputArray << endl;
  sendBits(outputArray, gpio);

  


}

void sendBits(BinaryArray dataToSend, uint32_t* gpio){
  cout<<"Starting to Send"<<endl;
  turnOff(gpio, OUTPUT_PIN);
  for(int i = 0; i < dataToSend.getLength(); i++){
    int currentVal = dataToSend(i);
    if(currentVal != !!currentVal){//Makes sure that element in binary array is valid
      cerr<<"Invalid Data in array found at index: " << i <<endl;
      return;
    }
    if(currentVal){
      turnOn(gpio, OUTPUT_PIN);
      sleep(DELAY_MICRO);
      turnOff(gpio, OUTPUT_PIN);
    }
    else{
      turnOff(gpio, OUTPUT_PIN);
      sleep(DELAY_MICRO);
      turnOff(gpio, OUTPUT_PIN);
    }
    //sleep(DELAY_MICRO);
  }
}
