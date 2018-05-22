#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
//这个测试是相当于命令行的实现，如果配置了环境变量，在命令行中输入callPtree也能达到相同效果
int main()
{
	pid_t pid;
	setbuf(stdout, NULL); //清除缓冲区否则子进程的输出被阻塞了
	pid = fork();
	if (pid < 0)
	{
		fprintf(stderr, "fork failed");
		return 1;
	}
	else if (pid == 0)
	{
		printf("now call ptree:\n");
		printf("516021910233 Child PID=%d\n", getpid());
		execl("./callPtree", "callPtree", NULL); //第一个参数是路径，第二个是名字，调用后会关闭子进程
	}
	else
	{
		wait(NULL);
		printf("516021910233 Parent PID=%d\n", getpid());
	}
	return 0;
}
