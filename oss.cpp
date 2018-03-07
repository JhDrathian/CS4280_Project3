//oss program
//header
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/msg.h>
#include <signal.h>
#define PERM (S_IRUSR | S_IWUSR)
using namespace std;

int seconds = 0;
int nanoseconds = 0;
int max_child_process = 5; //default value
int max_time_elapsed = 20; //default value
string output_file_name = "log.out"; //default value
int option = 0; //for use with getop()
pid_t childpid = 0; //for forking()

//shared memory variables
int id1;
int id2;
int * num_seconds;
int * num_nanoseconds;

struct mymsg
{
	long mytype;
	char mtext[100];
};

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
	if (max_child_process < 1 || max_time_elapsed < 1)
	{
		cout << "Please input positive integers." << endl;
		return 0;
	}
	cout << "Max number of child processes: " << max_child_process << "." << endl;
	cout << "Max time for processes to live: " << max_time_elapsed << "." << endl;
	cout << "Output log file name: " << output_file_name << "." << endl;
//SHARED MEMORY CREATION START
	if ((id1 = shmget(1000, sizeof(int), PERM)) == -1)
	{
		perror("Failed to create shared memory segment.");
		return 1;
	}
	if ((num_seconds = (int *)shmat(id1, NULL, 0)) == (void*)-1)
	{
		perror("Failed to attach shared memory segment");
		if (shmctl(id1, IPC_RMID, NULL) == -1)
			perror("Failed to remove memory segment");
		return 1;
	}
	*num_seconds = 1;
	cout << "Seconds = " << *num_seconds << endl;

	if ((id2 = shmget(2000, sizeof(int), PERM | IPC_CREAT | 0666)) == -1)
        {                
		perror("Failed to create shared memory segment.");
                return 1;       
	}        
	if ((num_nanoseconds = (int *)shmat(id2, NULL, 0)) == (void*)-1)
        {       
		perror("Failed to attach shared memory segment");
                if (shmctl(id1, IPC_RMID, NULL) == -1)
                        perror("Failed to remove memory segment");
                return 1;
        }
        cout << "Nanoseconds = " << *num_nanoseconds << endl;

//create mailbox
	int msqid;
	if ((msqid = msgget(1000, PERM | IPC_CREAT)) == -1)
		perror("Failed to create new private message queue");
	
	char* my_string = "MAILBOX TEST";
	struct msgbuf* msg = (struct msgbuf*)malloc(sizeof(100000)/* + strlen(my_string)*/);
	msg->mtype = 1;
	strcpy(msg->mtext, my_string);
	cout << msg->mtext << endl;	
	msgsnd(1000, msg, strlen(my_string)+1, 0);	


	int num_current_processes = 0;
	int num_total_processes = 0;
	int num_max_total_processes = 100;


	while (num_total_processes < num_max_total_processes)
	{
		if ( num_current_processes == max_child_process)
		{
			wait(NULL);
			num_current_processes--;
			cout << "Number of processes has been reduced to: " << num_current_processes << endl;
		}		
		childpid = fork();
		if (childpid < 0)
			perror("Failed to Fork");
		if (childpid == 0)
		{
			execl("./user", NULL);
			perror ("Exec call was unsuccessful.");
		}
		num_current_processes++;
		num_total_processes++;
		cout << "Number of child processes currently active: " << num_current_processes << endl;
		cout << "Total Processes Spawn so far: " << num_total_processes << endl;
	}
	wait(NULL);

}

