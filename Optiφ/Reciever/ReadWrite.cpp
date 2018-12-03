#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "libconfig.h++"
#include <vector>
#include <time.h>
#include <sys/time.h>
//#include "inaryArray.cpp"
#include <string>

using namespace std;
using namespace libconfig;

//----------------------------------------------------------------------------------------------------------
// This gives you the current time
//------------------------------------------------------------------------------------------------------

void getTime(char* buffer)
{
    struct timeval tv;
    
    time_t curtime;
    
    gettimeofday(&tv, NULL);
    
    curtime=tv.tv_sec;
    
    strftime(buffer,30,"%m-%d-%Y  %T.",localtime(&curtime));
}


//------------------------------------------------------------------------------------------------------
// This writes to the log file and also adds the message
//------------------------------------------------------------------------------------------------------
int write(char* msg){
    
    // To read the file and get the log file location
    Config cfgRead;
    
    try{
        cfgRead.readFile("setup.cfg"); // Read the file
    }
    catch(const FileIOException &fioex){
        cerr << "I/O error while reading file. " << endl;
        
    }catch(const ParseException &pex){
        cerr << "Parse Error at " << pex.getFile() << ": " << pex.getLine() << " - " << pex.getError() << endl;
    }
    
    int n = 0;  // This keeps track of how long the log file address is (Just the length)
    
    // Try catch to see if the LOGFILE setting actually exists
    try{
        string logFilename = cfgRead.lookup("LOGFILE");
        n = logFilename.length();
        
    }catch(const SettingNotFoundException &nfex){
        cerr << "WTF! Something went wrong, make sure there is all the listed setting on the file. ";
    }
    
    // Copies the value of the logfile address and saves it to the logFilename
    string logFilename = cfgRead.lookup("LOGFILE");
    
    // Converts the string into the char array and saves the value to the output_file
    static const char *output_file = &logFilename[0u];
    
    Config cfg; // Now, start writing to the log file
    
    // Try catch to see if the outPut file exists, if it doesn't exists make a new one
    try{
        cfg.readFile(output_file);
        
    }catch(const FileIOException &fioex){
        
        cfg.writeFile(output_file);
        cerr << "Making new file " << endl;
        
    }catch(const ParseException &pex){
        
        cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()  // This gives the line # where there is a parse error
        << " - " << pex.getError() << endl;
        return(EXIT_FAILURE);
    }
    
    
    Setting &root = cfg.getRoot();
    
    // Checks if the "adding" group list exists
    if(! root.exists("adding")){
        // If it doesn't exists make a new one
        root.add("adding", Setting::TypeGroup);
    }
    
    Setting &adding = root["adding"];   // Checking inside the adding
    
    // Does times exists? If not add one..
    if(! adding.exists("Characters"))
        adding.add("Characters", Setting::TypeList);
    
    Setting &times = adding["Characters"];   // Checking inside times
    
    char time[30];  // Stores the time
    getTime(time);  // gets the current time
    
    // Create the new charsAdded entry.
    Setting &charsAdded = times.add(Setting::TypeGroup);
    
    
    // The following are the print lines that will be written to the log files
    charsAdded.add("Time", Setting::TypeString) = time;
    charsAdded.add("Name", Setting::TypeString) = output_file;
    charsAdded.add("Msg", Setting::TypeString) = msg;
    
    
    // Write out the updated configuration.
    try{
        cfg.writeFile(output_file);
        cerr << "Updated configuration successfully written to: " << output_file
        << endl;
        
    }catch(const FileIOException &fioex){
        cerr << "I/O error while writing file: " << output_file << endl;
        return(EXIT_FAILURE);
    }
    
    return(EXIT_SUCCESS);
    
}


//------------------------------------------------------------------------------------------------------
// This writes to the log file and writes the time
//------------------------------------------------------------------------------------------------------
int write(){
    
    Config cfgRead;
    try{
        cfgRead.readFile("setup.cfg"); // Read the file
    }
    catch(const FileIOException &fioex){
        cerr << "I/O error while reading file. " << endl;
    }catch(const ParseException &pex){
        cerr << "Parse Error at " << pex.getFile() << ": " << pex.getLine() << " - " << pex.getError() << endl;
    }
    
    int n = 0;
    try{
        string logFilename = cfgRead.lookup("LOGFILE");
        n = logFilename.length();
        
    }catch(const SettingNotFoundException &nfex){
        cerr << "WTF! Something went wrong, make sure there is all the listed setting on the file. ";
    }
    
    string logFilename = cfgRead.lookup("LOGFILE");
    
    static const char *output_file = &logFilename[0u];
    
    
    Config cfg;
    
    try{
        cfg.readFile(output_file);
    }catch(const FileIOException &fioex){
        cfg.writeFile(output_file);
        cerr << "Making new file " << endl;
        
    }catch(const ParseException &pex){
        cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
        << " - " << pex.getError() << endl;
        return(EXIT_FAILURE);
    }
    
    
    Setting &root = cfg.getRoot();
    
    if(! root.exists("adding")){
        root.add("adding", Setting::TypeGroup);
    }
    
    Setting &adding = root["adding"];
    
    if(! adding.exists("times")){
        adding.add("times", Setting::TypeList);
    }
    
    Setting &times = adding["times"];
    
    
    char time[30];
    getTime(time);
    
    
    // Create the new charsAdded entry.
    Setting &charsAdded = times.add(Setting::TypeGroup);
    
    charsAdded.add("Time", Setting::TypeString) = time;
    charsAdded.add("Name", Setting::TypeString) = output_file;
    
    
    // Write out the updated configuration.
    try{
        cfg.writeFile(output_file);
        cerr << "Updated configuration successfully written to: " << output_file
        << endl;
        
    }catch(const FileIOException &fioex){
        cerr << "I/O error while writing file: " << output_file << endl;
        return(EXIT_FAILURE);
    }
    
    return(EXIT_SUCCESS);
    
}


void append(char val){
    static int length = 0;
    static char* valueReceived = new char[length];
    
    if(val == 10){
        write(&valueReceived[0]);
        return;
    }
    length+=1;
    for(int i = length-1; i < length; i++){
        valueReceived[i] = val;
    }
    
}


//------------------------------------------------------------------------------------------------------
// This reads the config file
//------------------------------------------------------------------------------------------------------
int readConfigFile(int &watchDog, int &delay, int &pin1, int &pin2, int* arr, int &length){
    
    Config cfg;
    try{
        cfg.readFile("setup.cfg"); // Read the file
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



