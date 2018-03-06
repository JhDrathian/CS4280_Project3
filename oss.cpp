//oss program
//header
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

using namespace std;

int max_child_process = 5; //default value
int max_time_elapsed = 20; //default value
string output_file_name = "log.out"; //default value
int option = 0; //for use with getop()


int main (int argc, char *argv[])
{
	cout << "Hello World." << endl;
	cout << "Number of arguments: " << argc << "." << endl;
	if (argc < 1)
		cout << "Too Few Arguments. Use -h for help menu" << endl;
	while ((option = getopt(argc, argv, "hs:l:t:")) != -1)
		{
			switch(option)
			{
				case 'h' : 
				{
					cout << "You have used -h to access the help menu." << endl;
					cout << "Use -s followed by a positive integer to specify the maximum number of child processes. Default = 5." << endl;
					cout << "Use -t followed by a positive integer to specify the maximum time for the main process to live. Default = 20." << endl;
					cout << "Use -l followed by a string to specify the name of the output log file. Default = log.out ." << endl;
					break;
				}	
				case 's' :
				{
					max_child_process = atoi(optarg);
					break;
				}
				case 'l' : 
				{
					output_file_name = optarg;
					break;
				}
				case 't' :
				{
					max_time_elapsed = atoi(optarg);
					break;
				}
			}
		}
	cout << "Max number of child processes: " << max_child_process << "." << endl;
	cout << "Max time for processes to live: " << max_time_elapsed << "." << endl;
	cout << "Output log file name: " << output_file_name << "." << endl;
}
