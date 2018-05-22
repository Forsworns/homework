#include <pthread.h> //编译时要用gcc -pthread显式调用链接库
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int sum;
void *runner(void *param);

int main(int argc, char *argv[]){
	pthread_t tid;
	pthread_attr_t attr;

	if(argc != 2){
	fprintf(stderr, "the usage must contain an integer parameter\n");
	}
	if(argv[1]<=0){
	fprintf(stderr, "the upper bound is a positive integer\n");
	}

	pthread_attr_init(&attr);
	pthread_create(&tid,&attr,runner,argv[1]);
	pthread_join(tid,NULL);
	
	printf("the sum is %d\n",sum);
}

void *runner(void *param){
	int upper = atoi(param);
	sum = 0;
	for(int i = 1; i <= upper; ++i){
		sum += i;
	}
	pthread_exit(0);
}
