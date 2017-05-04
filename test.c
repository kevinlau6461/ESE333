#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <string.h>

int i = 0;
int m_index = 0;
char monitor[10];
sig_atomic_t sigusr1_count = 0;

void handler (int signal_number){
	++sigusr1_count;
}
void doSomeWork(char name)
{	
	const int NUM_TIMES = 5;
	for (i = 0; i < NUM_TIMES; i++){
		sleep(rand()%3);
		printf("%d = %c\n",i,name);
		monitor[m_index] = name;
		m_index++;
	}
}
int main(int argc, char *argv[])
{
	 	
	struct sigaction sa;
	memset (&sa, 0, sizeof (sa));
	sa.sa_handler = &handler;
	sigaction(SIGUSR1, &sa, NULL);
	
	printf("I am: %d\n", (int) getpid());
	pid_t pid = fork();
	srand((int) pid);
	printf("fork returned: %d\n", (int) pid);
	
	if(pid < 0){
		perror("Fork failed");
	}
	if(pid == 0){
		printf("I am the child with pid %d\n", (int) getpid());
		doSomeWork('C');		
		exit(0);	
	}	
	printf("I am the parent with pid %d waiting for child to end\n", (int) getpid());
	doSomeWork('P');
	wait(NULL);
	printf("Parent ending\n");
	printf("SIGUSR1 was raised %d times\n", sigusr1_count);		

	return 0;
	



}
