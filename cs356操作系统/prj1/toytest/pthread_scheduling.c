#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#define NUM_THREADS 5

int times = 0;
void* runner();

int main(){
	int scope;
	pthread_t tid[NUM_THREADS];
	pthread_attr_t attr;
	
	if(pthread_attr_getscope(&attr,&scope)){
		fprintf(stderr,"can't get the scope\n");
	}
	else{
		if(scope==PTHREAD_SCOPE_PROCESS){
			printf("PTHREAD_SCOPE_PROGRESS\n");
		} else if(scope==PTHREAD_SCOPE_SYSTEM){
			printf("PTHREAD_SCOPE_SYSTEM\n");
		}
		else {
			fprintf(stderr,"Illegal scope value.\n");
		}
	}

	pthread_attr_setscope(&attr,PTHREAD_SCOPE_SYSTEM);
	pthread_attr_init(&attr);
	for(int i=0;i<NUM_THREADS;++i){
		pthread_create(&tid[i],&attr,runner,NULL);
	}
	for(int i=0;i<NUM_THREADS;++i){
		pthread_join(tid[i],NULL);
	}
}

void* runner(){
	times++;
	printf("the running order is No.%d,and it's of the No.%d process\n", times, getpid());
	pthread_exit(0);
}
