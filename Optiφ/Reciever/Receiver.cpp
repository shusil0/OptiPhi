#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include "../Utilities/Utilities.cpp"
#include "../Utilities/GPIO++.c"
#include "../Utilities/BinaryArray.cpp"
#include "ReadWrite.cpp"

using namespace std;

const int INPUT_PIN;

enum State { START, NEW_CHAR_ZERO, NEW_CHAR_ONE, COUNT_ZERO, COUNT_ONE, COUNT_DONE, COUNT_TRANSITION, ERROR_CHECK};

int singalReading(uint32_t* gpio)
{
	return (bool)getPinState(gpio, INPUT_PIN);
}

//int test [90] = {0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1};
int main()
{

	// FILE* logFile;
	// logFile = fopen("Log.log", "a");
	// int watchdog;

	// if ((watchdog = open("/dev/watchdog", O_RDWR | O_NOCTTY)) < 0) {
	// 	printf("Error: Couldn't open watchdog device! %d\n", watchdog);
	// 	return -1;
	// } 

	// if(!doesFileExist(logFile)){
	// 	logFile = fopen("Log.log", "w");
	// 	fclose(logFile);
	// 	logFile = fopen("Log.log", "a");
	// }

	// if(!doesFileExist(logFile)){
	// 	cout<<"LOG FILE IS  MISSING"<<endl;
	// }
	GPIO_Handle gpio;
	gpio = gpiolib_init_gpio();
	if(gpio == NULL){
		cout<<"We done MESSED up"<<endl;
	}
	setAsInput(gpio, INPUT_PIN);
	int error_output = 0;
	int data_length = 8;


	int* data = new(std::nothrow) int[data_length]; // array to store the characters in binary
													// also stores error check associated with each character
	char char_output; // stores character to  writen to file
	
	State mystate = START;
	int i = 0;
	int temp = 0; // temp that will store a zero or one that represents a bit
	int reading; // stores the reading from the photodiode
	int new_char_zero_count = 0; // count for number of zero singals in the NEW_CHAR state
	int new_char_one_count = 0; // count for number of one singals in the NEW_CHAR state
	int count_zero_count = 0; // count for number of zero singals in the COUNT state
	int count_one_count = 0; // count for number of one singals in the COUNT state

	int watchdogTime = 0;

	int catch_up_delay = DELAY_MICRO;

	int new_char_zero_termination = 0;
	int new_char_one_termination = 0;

	int* new_char_array = {0};
	int length = 0;

	readConfigFile(&watchdogTime, &catch_up_delay, &0, &INPUT_PIN, &new_char_array, &length); // reading from config to adjust variables
	if (watchdogTime < 0 || watchdogTime > 15){
		//Log invalid time
		watchdogTime = WATCHDOG_TIME;
	}
	BinaryArray binaryArray = BinaryArray(new_char_array, length);


	if(catch_up_delay == -99) // -99 is error case, so data is read from another file and set to a default state
	{
		catch_up_delay = DELAY_MICRO;
	}
	if(INPUT_PIN < 0)
	{
		INPUT_PIN = INPUT_PIN_CONFIG;
	}
	if (binaryArray.isValid() == false)
	{
		new_char_zero_termination = 0;
		new_char_one_termination = 0;

		for(int i = 0; i < NEW_CHAR_SEQUENCE_LENGTH; i++)
		{
			if(NEW_CHAR_SEQUENCE[i] == 0)
			{
				++new_char_zero_termination;
			}
			else
			{
				++new_char_one_termination;
			}
		}
	}
	if(binaryArray.isValid() == true)
	{
		new_char_zero_termination = binaryArray.numZeros();
		new_char_one_termination = binaryArray.numOnes();

	}


	//This print statement will confirm to us if the time limit has been properly
	//changed. The \n will create a newline character similar to what endl does.
	//printf("The watchdog timeout is %d seconds.\n\n", timeout);

	while(error_output == 0)
	{
		// //cout<<reading<<", "<<mystate<<endl;

		// if (timer.getElapsedTime() >= sf::seconds((float)watchdogTime)) {
		// 	//writeToLogFile
		// 	timer.restart();
		// }
		

		switch(mystate)
		{
			case START:
				sleep(catch_up_delay/2); // wait for x/2 micrsosecods so the reviever system is in sync with sender system
										   // assuming signals are coming at x microsecond intervals				
				reading = singalReading(gpio); // test signal values
				if(reading == 0)
				{
					mystate = NEW_CHAR_ZERO;
					++new_char_zero_count;
				}
				else if(reading == 1)
				{
					mystate = NEW_CHAR_ONE;
					++new_char_one_count;
				}
				sleep(catch_up_delay/2);
				break;

			case NEW_CHAR_ZERO:			
				sleep(catch_up_delay/2); 
				reading = singalReading(gpio);
				if(reading == 0)
				{
					++new_char_zero_count;
				}
				else if(reading == 1)
				{
					if(new_char_zero_count >= new_char_zero_termination)
					{
						++new_char_one_count;
						mystate = NEW_CHAR_ONE;
						new_char_zero_count = 0;
					}
					else
					{
						mystate = NEW_CHAR_ZERO; // assume hyteresis
					}
				}
				sleep(catch_up_delay/2); // wait for x/2 microseconds again
				break;

			case NEW_CHAR_ONE:				 
				sleep(catch_up_delay/2);
				reading = singalReading(gpio);
				if(reading == 1)
				{
					++new_char_one_count;
				}
				else if(reading == 0)
				{
					if(new_char_one_count >= new_char_one_termination)
					{
						mystate = COUNT_ZERO;
						++count_zero_count;
						new_char_one_count = 0;

					}
					else
					{
						mystate = NEW_CHAR_ZERO; // assume hyteresis
						--new_char_one_count;
					}
				}
				sleep(catch_up_delay/2);
				break;

			case COUNT_ZERO:			
				sleep(catch_up_delay/2); 
				reading = singalReading(gpio);
				if(reading == 0)
				{
					++count_zero_count;
					if(count_zero_count > 2)
					{
						mystate = NEW_CHAR_ZERO;
						count_zero_count = 0;
						count_one_count = 0;
						new_char_one_count = 0;
					}
				}
				else if(reading == 1)
				{
					++count_one_count;
					if(count_zero_count == 2) // if the sequence is {0,0,1}
					{
						mystate = COUNT_DONE;
						temp = 0;
					}
					else if(count_zero_count == 1) // if the sequence is {0,1}
					{
						mystate = COUNT_ONE;
					}
					else
					{
						mystate = NEW_CHAR_ZERO;
						count_zero_count = 0;
						count_one_count = 0;
						new_char_one_count = 0;
					}
				}
				sleep(catch_up_delay/2);
				break;

			case COUNT_ONE:				
				sleep(catch_up_delay/2); 
				reading = singalReading(gpio);
				if(reading == 1 || reading == 0) // assume the zero reading is an error
				{
					++count_one_count;
					if(count_one_count == 2) // if the sequence is {0,1,1}
					{
						mystate = COUNT_DONE;
						temp = 1;
					}
					else // if the sequence is {0,1}
					{
						mystate = COUNT_ONE;
					}
				}
				sleep(catch_up_delay/2);
				break;


			case COUNT_DONE:
				data[i] = temp;
				++i;
				if(i > 7)
				{
					mystate = ERROR_CHECK;
				}
				else
				{
					mystate = COUNT_TRANSITION;
				}
				count_zero_count = 0;
				count_one_count = 0;
				new_char_one_count = 0;
				new_char_zero_count = 0;
				break;

			case COUNT_TRANSITION:
				sleep(catch_up_delay/2);	
				reading = singalReading(gpio);
				if(reading == 0)
				{
					++count_zero_count;
					mystate = COUNT_ZERO;
				}
				else if(reading == 1)
				{
					++count_one_count;
					mystate = COUNT_ONE;
				}
				sleep(catch_up_delay/2);
				break;

			case ERROR_CHECK:
				sleep(catch_up_delay/2);
				i = 0;
				new_char_one_count = 0;
				new_char_zero_count = 0;

				char_output = convertToChar(data);
				reading = singalReading(gpio);
				if(reading == 0){
					++new_char_zero_count;
				}
				else{
					++new_char_one_count;
				}
				cout << char_output;
				append(char_output);
				char time[30];
				getTime(time);
				printMsg(logFile, time, char_output, "\n \n");

				delete[] data;
				data = nullptr;
				data = new(std::nothrow) int[data_length]; // clear data array

  				mystate = START; // treat next sequence as a new start to reset
				sleep(catch_up_delay/2);
				break;
		
		}
		cout<<reading<<", "<<mystate<<endl;
	}
	
	return error_output;
}
