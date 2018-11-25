#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <chrono>
#include <thread>
#include "../Utilities/Utilities.cpp"
using namespace std;

enum State { START, IDLE_ZERO, IDLE_ONE, NEW_CHAR_ZERO, NEW_CHAR_ONE, COUNT_ZERO, COUNT_ONE, COUNT_DONE, COUNT_TRANSITION, ERROR_CHECK};

void sleep(int delay)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(delay)); // length of time to sleep, in miliseconds
}

int singalReading(int max_time, int num_readings)
{
	int total_time = 0;
	float average = 0;
	int delay = (max_time/num_readings) + 1;
	int i = 0;
	int pin_state;

	while(total_time < max_time)
	{
		pin_state = 1; //Ansar code needed
		average+=pin_state;
		total_time+= delay;

		sleep(delay);
		++i;
	}
	average = average/(num_readings*1.0);
	return round(average);
}

int signalToText()
{
	int error_output = 0;
	int data_lenght;
	data_lenght = 8;

	int* data = new(std::nothrow) int[data_lenght]; // array to store the characters in binary
													// also stores error check associated with each character
	char* char_output; // stores characters to be writen to file
	
	State mystate = START;
	int i = 0;
	int temp = 0; // temp that will store a zero or one that represents a bit
	int reading;
	int new_char_zero_count = 0; // count for number of zero singals in the NEW_CHAR state
	int new_char_one_count = 0; // count for number of one singals in the NEW_CHAR state
	int count_zero_count = 0; // count for number of zero singals in the COUNT state
	int count_one_count = 0; // count for number of one singals in the COUNT state

	int signal_reading_time = 750;
	int catch_up_delay = 250;
	int num_readings = 3;

	int new_char_zero_termination = 3;
	int new_char_one_termination = 3;

	while(1)
	{
		switch(mystate)
		{
			case START:
				reading = singalReading(signal_reading_time, num_readings); // test signal values for 750ms
				if(reading == 0)
				{
					mystate = IDLE_ZERO;
				}
				else if(reading == 1)
				{
					mystate = IDLE_ONE;
				}
				sleep(catch_up_delay); // wait for 250ms so the reviever system is in sync with sender system
									   // assuming singals are coming at 1000ms intervals
				break;

			case IDLE_ZERO:
				reading = singalReading(signal_reading_time, num_readings);
				if(reading == 1)
				{
					mystate = IDLE_ONE;
				}
				else if(reading == 0)
				{
					mystate = NEW_CHAR_ZERO;
					new_char_zero_count = 2; // count is 2 since IDLE got a zero reading before as well
				}
				sleep(catch_up_delay);
				break;

			case IDLE_ONE:
				reading = singalReading(signal_reading_time, num_readings);
				if(reading == 0)
				{
					mystate = IDLE_ZERO;
				}
				else if(reading == 1)
				{
					mystate = IDLE_ZERO; // ignore the error
				}
				sleep(catch_up_delay);
				break;

			case NEW_CHAR_ZERO:
				reading = singalReading(signal_reading_time, num_readings);
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
				sleep(catch_up_delay);
				break;

			case NEW_CHAR_ONE:
				reading = singalReading(signal_reading_time, num_readings);
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
						mystate = NEW_CHAR_ONE; // assume hyteresis
					}
				}
				sleep(catch_up_delay);
				break;

			case COUNT_ZERO:
				reading = singalReading(signal_reading_time, num_readings);
				if(reading == 0)
				{
					++count_zero_count;
				}
				else if(reading == 1)
				{
					++count_one_count;
					if(count_zero_count == 2) // if the sequence is {0,0,1}
					{
						mystate = COUNT_DONE;
						temp = 0;
					}
					else // if the sequence is {0,1}
					{
						mystate = COUNT_ONE;
					}
				}
				sleep(catch_up_delay);
				break;

			case COUNT_ONE:
				reading = singalReading(signal_reading_time, num_readings);
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
				sleep(catch_up_delay);
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
					COUNT_TRANSITION;
				}
				count_zero_count = 0;
				count_one_count = 0;
				break;

			case COUNT_TRANSITION:
				reading = singalReading(signal_reading_time, num_readings);
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
				sleep(catch_up_delay);
				break;
				

			case ERROR_CHECK:
				reading = singalReading(signal_reading_time, num_readings);
				if(reading == 0)
				{
					data[i] = 0;
				}
				else if(reading == 1)
				{
					data[i] = 1;
				}
				i = 0;
				new_char_one_count = 0;
				new_char_zero_count = 0;

				char_output = convertToChar(data);
				// write to file

				delete[] data;
  				data = nullptr;
  				data = new(std::nothrow) int[data_lenght]; // clear data array

  				mystate = START; // treat next sequence as a new start to determine if its IDLE or NEW_CHAR state
				sleep(catch_up_delay);
				break;

		}
	}
	return error_output;
}