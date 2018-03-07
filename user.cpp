//user program
//header

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/msg.h>
#define PERM (S_IRUSR | S_IWUSR)

using namespace std;

struct mymsg
{
	long mytype;
	char mtext[100];
};

int main (int argc, char *argv[])
{
	int id1;
	int id2;
	int * num_seconds;
	int * num_nanoseconds;
	cout << "PID | " << (long)getpid() << " has been created." << endl;
	
	id1 = shmget(1000, sizeof(int), PERM);
	num_seconds = (int *) shmat(id1, NULL, 0);
	cout << "PID | " << (long)getpid() << " | reports: " << * num_seconds << " seconds!" << endl;
	
	id2 = shmget(2000, sizeof(int), PERM);
        num_nanoseconds = (int *) shmat(id2, NULL, 0);
        cout << "PID | " << (long)getpid() << " | reports: " << * num_nanoseconds << " nanoseconds!" << endl;

	int msgid;
	if ((msgid = msgget(1000, PERM)) == -1)
		perror("Unable to open mailbox");
	
	struct mymsg* received_message = (struct mymsg*)malloc(sizeof(100000));
	msgrcv(1000, &received_message, 1000000, 0, 0);
	cout << received_message->mtext << endl;
}
