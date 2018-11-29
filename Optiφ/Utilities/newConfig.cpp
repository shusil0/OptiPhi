#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <libconfig.h++>
#include <vector>
#include <time.h>
#include <sys/time.h>
#include "BinaryArray.cpp"

using namespace std;
using namespace libconfig;

int readConfigFile(float &watchDog, int &delay, int &pin1, int &pin2, int* arr, int &length){
    
    Config cfg;
    try{
        cfg.readFile("text.cfg"); // Read the file
    }
    catch(const FileIOException &fioex){
        cerr << "I/O error while reading file. " << endl;
    }catch(const ParseException &pex){
        cerr << "Parse Error at " << pex.getFile() << ": " << pex.getLine() << " - " << pex.getError() << endl;
    }
    
    // This copies the values from the config file
    try{
        string logFilename = cfg.lookup("LOGFILE");
        watchDog = cfg.lookup("WATCHDOG_TIMEOUT");
        delay = cfg.lookup("TIME_BETWEEN_BITS");
        pin1 = cfg.lookup("SENDER_OUTPUT_PIN");
        pin2 = cfg.lookup("RECIEVER_INPUT_PIN");
        
    }catch(const SettingNotFoundException &nfex){
        cerr << "WTF! Something went wrong, make sure there is all the listed setting on the file. ";
    }
    
    const Setting& set = cfg.getRoot();
    
    // copying the array value
    try{
        const Setting &randomPins = cfg.lookup("NEW_CHAR_SEQUENCE");
        int nums = randomPins.getLength();
        
        vector <int> numss;
        int* array = new int[nums];
        for(int k = 0; k < nums; k++){
            numss.push_back(randomPins[k]);
        }
        
        array = &numss[0];
        length = nums;
        
        for(int i = 0; i < nums; i++){
            arr[i] = array[i];
        }
        
    }catch(const SettingNotFoundException &nfex){
        length = -1;    // If no array found in the config file
    }
    
    
    return(EXIT_SUCCESS);
}
