//  Created by Shusil on 2018-11-24.
//
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <chrono>
#include <thread>


const int NEW_CHAR_SEQUENCE_LENGTH = 5;
const int NEW_CHAR_SEQUENCE [NEW_CHAR_SEQUENCE_LENGTH] = { 0, 0, 1, 1, 1}; 

const int CHARACTER_LENGTH = 8;

const int BIT_SEQUENCE_LENGTH = 3;

const int ON_SEQUENCE [BIT_SEQUENCE_LENGTH] = { 0, 1, 1}; 
const int OFF_SEQUENCE [BIT_SEQUENCE_LENGTH] = { 0, 0, 1}; 

const int OUTPUT_PIN = 17;
const int INPUT_PIN_CONFIG = 17;

const int DELAY_MICRO = 40000;
//Working  Value 100000
//50000

using namespace std;

void sleep(int delay)
{
	std::this_thread::sleep_for(std::chrono::microseconds(delay)); // length of time to sleep, in microseconds
}

int totalNumberOfOnes(int data[]){
    
    // Check if the data array has length < 8
    
    int totalNumberOfOnes = 0;                              // total number of Ones
    for(int i = 0; i < CHARACTER_LENGTH-1; i++){
        if(data[i] == 1){
            totalNumberOfOnes = totalNumberOfOnes + 1;
        }
    }
    return totalNumberOfOnes;
}

bool isValid(int data[]){
    
    if(totalNumberOfOnes(data) % 2 == 0 && data[CHARACTER_LENGTH-1] == 1){         // If there are even number of ones and last digit is 1
        return true;
    }else if(totalNumberOfOnes(data) % 2 == 1 && data[CHARACTER_LENGTH-1] == 0){  // If there are odd number of ones and last digit is 0
        return true;
    }else{
        return false;
    }
}


int convertToAsciiNum(int data[]){
    int asciiNum = 0;                                       // After converting from binary
    
    if(isValid(data)){                                    // If the added 1s matches with the last digit in the array
        
        int validLength = CHARACTER_LENGTH-1;
        int power[validLength];                       // the value starts at 2^7 and and goes till 2^0
        for(int i = 0; i < validLength; i++)
        {
            power[i] = pow(2, validLength-i);      //power[] = {7, 6, 5, ..... 1, 0}
        }
        
        for(int j = 0; j < validLength; j++)    // find Ascii Value
        {
            int bits = data[j];              //every elements stored in an array
            asciiNum += bits * power[j];   //asciiNum = sum of elements in the array * 2^power where power = [0,7]
        }
        return asciiNum/2;
        
    }else{

        return -1;
    }
    return 0;
}

char convertToChar(int data[]){
    int asciiNum = convertToAsciiNum(data);
    // Check what if asciiNum is less than 0, this means invalid
    if(asciiNum < 0){
        return '#';
    }
    char ascii = (char)asciiNum;
    
    //cout << ascii << endl;
    return ascii;
    
}


// ---------------------------------------------------------------------------------------

bool isValid(char str){
    char input = str;
    int dec_Val = (int)input;
    if(dec_Val < 0 || dec_Val > 127){
        return false;
    }
    return true;
}

int* convertToBinaryArray(char str){
    int input = (int) str;
    int lengthOfArray = CHARACTER_LENGTH;
    int* binaryArray =  new(std::nothrow) int[lengthOfArray];
    if(binaryArray == NULL){
        cout<<"Array too big"<<endl;
        return new int[0];
    }

    
    for(int i = lengthOfArray-2; i >= 0; i--){
        binaryArray[i] = input % 2;
        input = input/2;
    }
    
    if(totalNumberOfOnes(binaryArray) % 2 == 1){
        binaryArray[lengthOfArray-1] = 0;
    }else if(totalNumberOfOnes(binaryArray) % 2 == 0){
        binaryArray[lengthOfArray-1] = 1;
    }
    
    return binaryArray;
    
}


