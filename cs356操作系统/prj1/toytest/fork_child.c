#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main(){
	pid_t pid;
	pid = fork();
	if (pid<0){
		fprintf(stderr,"fork failed");
		return 1;
	}
	else if(pid == 0){
		printf("child is running\n");
	}
	else {
		wait(NULL);
		printf("child has done\n");
	}
	return 0;
}
