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


int main(const int argc, const char *const argv[]) {
  GPIO_Handle gpio;
	gpio = gpiolib_init_gpio();
  setAsOutput(gpio, OUTPUT_PIN);
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

void append(char x){
  int length = 1;
  static char* chars = new char[length];
  if(x == '/n'){
    for(int i = 0; i  < length; i++){
      cout<<chars[i]<<flush;
    }
    cout<<x<<flush;
    return;
  }
  else{
    char* temp = new char[length];
    for(int i = 0; i < length; i++){
      temp[i] = chars[i];
    }
    delete[] chars;
    chars = new char[length + 1];
    for(int i = 0; i < length; i++){
      chars[i] = temp[i];
    }
    chars[length] = x;
    length++;

  }
  
}