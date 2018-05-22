#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <string.h>
using namespace std;

const int serveSize = 2;	//支持的服务数量
const int encodeNum = 3;	//凯撒密码参数
const int z = (int)('z'); //用于加密
const int Z = (int)('Z');
const int a = (int)('a') - 1;
const int A = (int)('A') - 1;
const char *replyContent = "wait please...\n";

pthread_mutex_t serveMutex = PTHREAD_MUTEX_INITIALIZER;			 //保护服务队列
pthread_mutex_t waitMutex = PTHREAD_MUTEX_INITIALIZER;			 //保护等待（回复）队列
pthread_mutex_t wait2serveMutex = PTHREAD_MUTEX_INITIALIZER; //等待（回复）状态转服务状态时，必须防止客户端的争抢
pthread_t threadId[serveSize];															 //记录用户编号，实现服务器额外功能，记录用户日志或便于观察、维护
vector<int> waitingQueue, servingQueue;											 //等待队列和服务队列，用vector没有用queue因为较灵活，扩展功能方便，比如上一行的功能
void *nothing;																							 //作为空指针，在必要时传入
struct sockaddr_in serv_addr, cli_addr;
socklen_t clilen;

void *serve(void *nousedvar);
void *reply(void *nousedvar);
void encode(char buffer[], int bufferLength);
void *accept_thread(void *sockfd);
int main(int argc, char *argv[])
{
	printf("有多个客户端处于等待状态时，若服务器空闲，哪个客户端先请求加密服务会优先被服务\n");
	//每当空闲时最新申请的客户端拥有最高的优先级
	//////////////////////////////////////////////////////////
	int sockfd, newsockfd, portno, pid;
	pthread_t acceptThreadId;
	for (int i = 0; i < serveSize; ++i)
	{
		threadId[i] = 0;
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		printf("ERROR opening socket\n");
		exit(1);
	}
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	//assign port number(>2000 generally)
	portno = 2050;
	serv_addr.sin_port = htons(portno);
	//the ip address of server
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("Error on binding\n");
		exit(1);
	}
	////////////////////////////////////////////////////////////

	//concurrent multi-thread service
	listen(sockfd, 2);
	clilen = sizeof(cli_addr);

	pthread_create(&acceptThreadId, NULL, accept_thread, &sockfd);
	while (1)
	{
		if (servingQueue.size() < serveSize && waitingQueue.size() != 0)
		{																		//转入服务状态
			newsockfd = waitingQueue.front(); //waitingQueue在acceptThreadId()线程中被填充
			waitingQueue.erase(waitingQueue.begin());
			servingQueue.push_back(newsockfd);
			if (threadId[0] == 0) //保存了是哪个用户正在使用服务
			{
				pthread_create(&threadId[0], NULL, serve, &newsockfd);
			}
			else
			{
				pthread_create(&threadId[1], NULL, serve, &newsockfd);
			}
		}

		if (servingQueue.size() == serveSize && waitingQueue.size() != 0)
		{ //转入等待（回复）状态
			newsockfd = waitingQueue.front();
			waitingQueue.erase(waitingQueue.begin());
			pthread_create(&acceptThreadId, NULL, reply, &newsockfd);
		}
	}
	//综上有两种方式进入服务：直接转入服务状态，或者进入等待（回复）状态后再进入服务状态

	return 0;
}

void encode(char buffer[], int bufferLength)
{
	//提供加密服务
	for (int i = 0; i < bufferLength; ++i)
	{
		if (buffer[i] <= 'Z' && buffer[i] >= 'A')
		{ //A-Z
			if (buffer[i] == 'W')
			{
				buffer[i] = 'Z';
			}
			else
			{
				buffer[i] = (char)((buffer[i] + encodeNum) % Z + (buffer[i] + encodeNum) / Z * A);
			}
		}
		else if (buffer[i] <= 'z' && buffer[i] >= 'a')
		{ //a-z
			if (buffer[i] == 'w')
			{
				buffer[i] = 'z';
			}
			else
			{
				buffer[i] = (char)((buffer[i] + encodeNum) % z + (buffer[i] + encodeNum) / z * a);
			}
		}
	}
}

void *serve(void *sockfd)
{
	//服务状态
	int n;
	char buffer[256];
	int newsockfd = *(int *)sockfd;
	while (1)
	{
		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		pthread_t selfId = pthread_self();
		if (n <= 0)
		{
			pthread_mutex_lock(&serveMutex);
			if (threadId[0] == selfId)
			{
				threadId[0] = 0;
			}
			else
			{
				threadId[1] = 0;
			}
			servingQueue.erase(servingQueue.begin()); //不需要知道是哪个被弹出，只需要保证减少了就可以
			printf("in client%d,size is %lu and front is %d\n", newsockfd, servingQueue.size(), servingQueue.front());
			pthread_mutex_unlock(&serveMutex);

			printf("Client %d log out!\n", newsockfd);
			pthread_exit(nothing);
		}
		else
		{
			printf("Have read from client %d!Encoding...\n", newsockfd);
		}
		encode(buffer, n - 1);
		n = write(newsockfd, buffer, n);
		if (n <= 0)
		{
			printf("Error on write!\n");
			pthread_exit(nothing);
		}
		else
		{
			printf("Write back to client %d!\n", newsockfd);
		}
	}
}
void *reply(void *sockfd)
{
	//reply()线程处理等待中的客户端，并在空闲时将请求的客户端转入服务状态
	int n;
	char buffer[256];
	int newsockfd = *(int *)sockfd;
	pthread_t selfId = pthread_self();
	while (1)
	{
		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		pthread_mutex_lock(&wait2serveMutex); //防止两个处于等待状态的client同时恢复
		if (servingQueue.size() < serveSize)
		{
			servingQueue.push_back(newsockfd); //处理的客户数少于2，可以将客户转为服务状态
			pthread_mutex_unlock(&wait2serveMutex);
			while (1)
			{
				if (n <= 0)
				{
					pthread_mutex_lock(&serveMutex);
					if (threadId[0] == selfId)
					{
						threadId[0] = 0;
					}
					else
					{
						threadId[1] = 0;
					}
					servingQueue.erase(servingQueue.begin()); //不需要知道是哪个被弹出，只需要保证减少了就可以
					pthread_mutex_unlock(&serveMutex);
					printf("Client %d log out!\n", newsockfd);
					pthread_exit(nothing);
				}
				else
				{
					printf("Have read from client %d!Encoding...\n", newsockfd);
				}
				encode(buffer, n - 1);
				printf("%s\n", buffer);
				n = write(newsockfd, buffer, n);
				if (n <= 0) //各种错误情况
				{
					printf("Error on write!\n");
					pthread_exit(nothing);
				}
				else
				{
					printf("Write back to client %d!\n", newsockfd);
				}
				bzero(buffer, 256);
				n = read(newsockfd, buffer, 255);
			}
			pthread_exit(nothing);
		}
		pthread_mutex_unlock(&wait2serveMutex);
		if (n <= 0)
		{
			printf("Client %d log out!\n", newsockfd);
			pthread_exit(nothing);
		}
		else
		{
			printf("Have read from client %d!But he has to wait!\n", newsockfd);
		}
		bzero(buffer, 256);
		strcpy(buffer, replyContent);
		n = write(newsockfd, buffer, strlen(replyContent));
		if (n <= 0)
		{
			printf("Error on write!\n");
			pthread_exit(nothing);
		}
		else
		{
			printf("Reply to client %d!\n", newsockfd);
		}
	}
}
//accept_thread()线程用于处理连接请求，
void *accept_thread(void *sockfd)
{
	while (1)
	{
		int newsockfd;
		newsockfd = accept(*(int *)sockfd, (struct sockaddr *)&cli_addr, &clilen);
		if (newsockfd < 0)
		{
			printf("Error on accept!\n");
			pthread_exit(nothing);
		}
		else
		{
			printf("Client %d connected!\n", newsockfd);
		}
		pthread_mutex_lock(&waitMutex); //防止对waitingQueue的误读写
		waitingQueue.push_back(newsockfd);
		pthread_mutex_unlock(&waitMutex);
	}
}
