//根据system call返回的DFS顺序的数据进行打印，注意tab数量
#include <stdio.h>
#include <linux/types.h>
#include <string.h>
#define MAXBUFFER 2048
struct prinfo
{
	pid_t parent_pid;				//process id of parent
	pid_t pid;							//process id
	pid_t first_child_pid;	//pid of youngest child
	pid_t next_sibling_pid; //pid of older sibling
	long state;							//current state of process
	long uid;								//user id of process owner
	char comm[64];					//name of program executed
	int depth;							//进程在树中的深度
};
//利用之前返回的深度优先搜索的顺序打印进程
void printptree(struct prinfo *buf, int *nr)
{
	int i, j;
	for (i = 0; i < *nr; ++i)
	{
		for (j = 0; j < buf[i].depth; ++j) //根据深度确定tab的数量
		{
			printf("\t");
		}
		printf("%s,%d,%ld,%d,%d,%d,%ld\n", buf[i].comm, buf[i].pid, buf[i].state, buf[i].parent_pid, buf[i].first_child_pid, buf[i].next_sibling_pid, buf[i].uid);
	}
}
//这个测试文件是使用的syscall直接调用之前加入的模块
int main(int argc, char *argv[])
{
	struct prinfo *buf = (struct prinfo *)malloc(sizeof(struct prinfo) * MAXBUFFER);
	int nr;
	printf("This is a test for ptree:\n");
	syscall(287, buf, &nr);
	printf("syscall succeed!:\n");
	printptree(buf, &nr);
	free(buf);
	return 0;
}