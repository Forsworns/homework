#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int global_value = 10;

int main(){
	pid_t pid;
	pid = fork();
	if(pid==0){
		global_value += 10;
		printf("the value in child process is %d \n",global_value);
	} else {
		//wait(NULL);考虑如果注释掉wait()函数，父进程不再等待子进程，测试区别确是父进程先运行不等待子进程，子进程仍然可以执行
		printf("the value in parent is %d \n",global_value);
	}
	return 0;
}
