#include <stdio.h>
#include <iostream>
#include "ReadWrite.cpp"

using namespace std;

// Test to see if the config Reading works

int main(){
    
    int watchDog = 0.0;
    int delay = 0;
    int pin1 = 0;
    int pin2 = 0;
    int length = 0;
    int* newArray = new int[5];
    
    readConfigFile(watchDog, delay, pin1, pin2, newArray, length);
    
    cout << "Watch Dog: " << watchDog << endl;
    cout << "Delay: " << delay << endl;
    cout << "Pin1: " << pin1 << endl;
    cout << "Pin2: " << pin2 << endl;
    cout << "Length: " << length << endl;
    
    for(int i = 0; i < 5; i++){
        cout << "Imported numbers: " << newArray[i] << endl;
    }
    
    return 0;
    
}
