//  Created by Shusil on 2018-11-24.
//
#include <iostream>
#include <stdio.h>
#include <cmath>

const int length = 8;

using namespace std;

int totalNumberOfOnes(int data[]){
    
    // Check if the data array has length < 8
    
    int totalNumberOfOnes = 0;                              // total number of Ones
    for(int i = 0; i < length-1; i++){
        if(data[i] == 1){
            totalNumberOfOnes = totalNumberOfOnes + 1;
        }
    }
    return totalNumberOfOnes;
}

bool isValid(int data[]){
    
    if(totalNumberOfOnes(data) % 2 == 0 && data[length-1] == 1){         // If there are even number of ones and last digit is 1
        return true;
    }else if(totalNumberOfOnes(data) % 2 == 1 && data[length-1] == 0){  // If there are odd number of ones and last digit is 0
        return true;
    }else{
        return false;
    }
}


int convertToAsciiNum(int data[]){
    int asciiNum = 0;                                       // After converting from binary
    
    if(isValid(data)){                                    // If the added 1s matches with the last digit in the array
        
        int validLength = 7;
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
    char ascii = asciiNum;
    
    cout << "ascii " << ascii << endl;
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

int* convertToBinaryArray(int binaryArray[], char str){
    int input = (int) str;
    int lengthOfArray = 8;
    binaryArray =  new int[lengthOfArray];
    
    for(int i = lengthOfArray-2; i >= 0; i--){
        binaryArray[i] = input % 2;
        input = input/2;
    }
    
    if(totalNumberOfOnes(binaryArray) % 2 == 1){
        binaryArray[lengthOfArray-1] = 0;
    }else if(totalNumberOfOnes(binaryArray) % 2 == 0){
        binaryArray[lengthOfArray-1] = 1;
    }
    
    for(int j = 0; j < 8; j++){
        cout << binaryArray[j] << ", " << flush;
    }
    return binaryArray;
    
}


