

//#include "gpiolib++.c"
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


int main(const int argc, const char* const argv[]){
    BinaryArray outputArray;
    for(int k = 1; k < argc; k++){
        const char* str = argv[k];
        int i = 0;
        int* currentChar = new int[8];
        while(str[i]){
            
            for(int j = 0; j < 8; j++){
                currentChar[j] = 0;
            }
            currentChar = convertToBinaryArray(str[i]);
            // for(int j = 0; j < 8; j++){
            //     cout <<currentChar[j]<<endl;
            // }
            // cout<<"________"<<str[i]<<"____________"<<endl;
            for(int j = 0; j < CHARACTER_LENGTH; j++){
                outputArray += currentChar[j];
            }
            outputArray += outputArray.newCharArray;
            i++;
        }
            currentChar = convertToBinaryArray(' ');
            for(int j = 0; j < CHARACTER_LENGTH; j++){
                outputArray += currentChar[j];
            }

    }
    




    cout<<outputArray<<endl;



}



