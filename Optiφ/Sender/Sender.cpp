#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <iostream>     // std::cout
#include <new>          // std::nothrow
#include "../Utilities/BinaryArray.cpp"
// const BinaryArray ON;
// const BinaryArray OFF;

using namespace std;

int main(const int argc,
  const char *
    const argv[]) {
  BinaryArray outputArray;
  outputArray += outputArray.newCharArray;
  for (int k = 1; k < argc; k++) {
    cout << "****" << endl;
    const char * str = argv[k];
    int i = 0;
    int * currentChar = new int[8];

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

}