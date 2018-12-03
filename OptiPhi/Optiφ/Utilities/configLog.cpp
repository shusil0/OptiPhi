#include <stdio.h>
#include <iostream>
#include <time.h>
#include <sys/time.h>

using namespace std;

void getTime(char* buffer)
{
    
    struct timeval tv;
    
    //Create a time_t variable named curtime
    time_t curtime;
    
    
    //Get the current time and store it in the tv struct
    gettimeofday(&tv, NULL);
    
    //Set curtime to be equal to the number of seconds in tv
    curtime=tv.tv_sec;
    
    strftime(buffer,30,"%m-%d-%Y  %T.",localtime(&curtime));
    
}

void readFile(FILE* configFile, int* timeout, char* logFileName, int* numBlinks, int* delay, int* pin1, int* pin2){
    
    int i = 0;
    char buffer[255];
    *timeout = 0;
    *numBlinks = 0;
    *delay = 0;
    *pin1 = 0;
    *pin2 = 0;
    
    int input = 0;
    
    while(fgets(buffer, 255, configFile) != NULL)
    {
        i = 0;
        
        if(buffer[i] != '#')
        {
            while(buffer[i] != 0)
            {
                
                if(buffer[i] == '=' && input == 0)
                {
                    
                    while(buffer[i] != 0)
                    {
                        if(buffer[i] >= '0' && buffer[i] <= '9') // This is watchDog timer
                        {
                            
                            *timeout = (*timeout *10) + (buffer[i] - '0');
                        }
                        i++;
                    }
                    input++;
                }else if(buffer[i] == '=' && input == 1) //This will find the name of the log file
                {
                    
                    int j = 0;
                    while(buffer[i] != 0  && buffer[i] != '\n')
                    {
                        if(buffer[i] != ' ' && buffer[i] != '=')
                        {
                            logFileName[j] = buffer[i];
                            j++;
                        }
                        i++;
                    }
                    logFileName[j] = 0;
                    input++;
                }else if(buffer[i] == '=' && input == 2) // Blink Count
                {
                    
                    while(buffer[i] != 0)
                    {
                        
                        if(buffer[i] >= '0' && buffer[i] <= '9')
                        {
                            
                            *numBlinks = (*numBlinks *10) + (buffer[i] - '0');
                        }
                        i++;
                    }
                    input++;
                }else if(buffer[i] == '=' && input == 3) // Delay
                {
                    
                    while(buffer[i] != 0)
                    {
                        
                        if(buffer[i] >= '0' && buffer[i] <= '9')
                        {
                            
                            *delay = (*delay *10) + (buffer[i] - '0');
                        }
                        i++;
                    }
                    input++;
                }else if(buffer[i] == '=' && input == 4) // PIN 1
                {
                    
                    while(buffer[i] != 0)
                    {
                        
                        if(buffer[i] >= '0' && buffer[i] <= '9')
                        {
                            
                            *pin1 = (*pin1 *10) + (buffer[i] - '0');
                        }
                        i++;
                    }
                    input++;
                }else if(buffer[i] == '=' && input == 5) // PIN 1
                {
                    
                    while(buffer[i] != 0)
                    {
                        
                        if(buffer[i] >= '0' && buffer[i] <= '9')
                        {
                            
                            *pin2 = (*pin2 *10) + (buffer[i] - '0');
                        }
                        i++;
                    }
                    input++;
                }
                else
                {
                    i++;
                }
            }
        }
    }
}


char* getFileName(){
    FILE* configFile;
    configFile = fopen("text.cfg", "r");
    
    if(!configFile){
        perror("The config file could not be opened");
        return -1;
    }
    
    int timeout = 0;
    char logFileName[50];
    int numBlinks;
    int delay = 0;
    int pin1 = 0;
    int pin2 = 0;
    
    readFile(configFile, &timeout, logFileName, &numBlinks, &delay, &pin1, &pin2);
    fclose(configFile);
    
    return logFileName;
}


void printMsg(FILE* logFile, char* time, char* programName, char* comment){
    
    fprintf(logFile, "%s : %s : %s", time, programName, comment);
    
}

bool doesFileExist(FILE* logFile){
    if(!logFile)
    {
        return false;
    }
    return true;
}


int main(){
    FILE* logFile;
    logFile = fopen(getFileName(), "a");
    
    if(doesFileExist(logFile) == false){
        perror("The log file could not be opened");
    }
    
    char time[30];
    getTime(time);
    
    for(int i = 0; i < 4; i++){
        printMsg(logFile, time, "Hii", "Hello\n\n");
    }
    
    fclose(logFile);
}
