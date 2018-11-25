

//#include "gpiolib++.c"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <iostream>     // std::cout
#include <new>          // std::nothrow
#include "../BinaryArray.cpp"


const int NEW_CHAR_SEQUENCE_LENGTH = 6;
//const int NEW_CHAR_SEQUENCE [NEW_CHAR_SEQUENCE_LENGTH] = { 0, 0, 0, 1, 1, 1}; 

const int BIT_SEQUENCE_LENGTH = 3;

const int ON_SEQUENCE [BIT_SEQUENCE_LENGTH] = { 0, 1, 1}; 
const int OFF_SEQUENCE [BIT_SEQUENCE_LENGTH] = { 0, 0, 1}; 

// const BinaryArray ON;
// const BinaryArray OFF;


using namespace std;


int main(const int argc, const char* const argv[]){
    

    BinaryArray on(ON_SEQUENCE, 3);
    BinaryArray off(OFF_SEQUENCE, 3);
    on += off;

    cout<<on<<endl;



}

//Incomplete TODO:
int* convertToBinaryArray(char x){
    return new int[0];
}

int convertDecimalToBinary(int n){
    int binaryNumber = 0;
    int remainder = 1; 
    int i = 1;
    int step = 1;

    while (n) {
        remainder = n%2;
        n /= 2;
        binaryNumber += remainder*i;
        i *= 10;
    }
    return binaryNumber;
}

int convertBinaryToDecimal(int n){
    int factor = 1;
    int total = 0;
    while (n){
        total += (n%10) * factor;
        n /= 10;
        factor *= 2;
    }
 
    return total;
}
