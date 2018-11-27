#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <iostream>     // std::cout
#include "../Utilities/BinaryArray.cpp"
#include "../Utilities/GPIO++.c"

// const BinaryArray ON;
// const BinaryArray OFF;

using namespace std;
const int OUTPUT_PIN = 4;

int main(const int argc, const char *const argv[]) {
 cout<<"0"<<endl;
  GPIO_Handle gpio;
  cout<<"1"<<endl;
	gpio = gpiolib_init_gpio();
  cout<<"2"<<endl;
  setAsOutput(gpio, OUTPUT_PIN);
  BinaryArray outputArray;
  outputArray += outputArray.newCharArray;
  cout<<"3"<<endl;
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
      if (str[i + 1] != 0) {
        outputArray += outputArray.newCharArray;
      }

      i++;
    }
    if (k != argc - 1) {
      currentChar = convertToBinaryArray(' ');
      for (int j = 0; j < CHARACTER_LENGTH; j++) {
        outputArray += currentChar[j];
      }

    }
  }

  cout << outputArray << endl;
  cout<<"Starting to Send"<<endl;
  turnOff(gpio, OUTPUT_PIN);
  for(int i = 0; i < outputArray.getLength(); i++){
    int currentVal = outputArray.getData()[i];
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