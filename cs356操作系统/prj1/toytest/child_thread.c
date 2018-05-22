#include <sys/types.h> //gcc编译时需要加-lphread
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
int value = 0;
void *runner(void *param){
	value = 5;
	pthread_exit(0);
}
int main(int argc, char *argv[]){
	pid_t pid;
	pthread_t tid;
	pthread_attr_t attr;
	pid = fork();
	if(pid==0){
		pthread_attr_init(&attr);
		pthread_create(&tid, &attr,runner, NULL);
		pthread_join(tid, NULL);
		printf("child:value=%d/n",value);	//子进程会拷贝父进程的所有代码，所以这里的全局变量是一个不同的变量	
	}
	else if(pid>0){
		wait(NULL);
		printf("Parent: value%d/n",value);
	}
}
