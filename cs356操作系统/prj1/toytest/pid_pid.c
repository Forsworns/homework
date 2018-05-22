#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
	pid_t pid, pid1;
	pid = fork();
	if(pid == 0){
	pid1 = getpid();
	printf("pid in child is %d\n",pid);
	printf("pid1 in child is %d\n",pid1);
	} else if (pid>0) {
	wait(NULL);
	pid1 = getpid();
	printf("pid in parent is %d\n",pid);
	printf("pid1 in parent is %d\n",pid1);
	} else {
	printf("failed");
	return 1;
	}
}
