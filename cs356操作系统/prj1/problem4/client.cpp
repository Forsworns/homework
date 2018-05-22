#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
//客户端程序，输入:q结束，同时等待先向服务器请求的会被先处理
int main(int argc, char *argv[])
{
	/////////////////////////////////
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];
	//port number of server
	portno = 2050;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		printf("ERROR opening socket");
		exit(1);
	}
	server = gethostbyname("127.0.0.1");
	if (server == NULL)
	{
		printf("ERROR, no such host");
		exit(0);
	}
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("ERROR connecting");
		exit(1);
	}
	else
	{
		printf("Connected!");
	}
	///////////////////////////////////////////

	while (1)
	{
		printf("Please enter the message:\n");
		bzero(buffer, 256);
		fgets(buffer, 255, stdin);
		if (buffer[0] == ':' && buffer[1] == 'q')
		{
			break;
		}
		n = write(sockfd, buffer, strlen(buffer));
		if (n < 0)
		{
			printf("Error!Writing failed.\n");
			exit(1);
		}
		bzero(buffer, 256);
		n = read(sockfd, buffer, 255);
		if (n < 0)
		{
			printf("Error!Reading failed.\n");
		}
		else
		{
			printf("From server: %s", buffer);
		}
	}
	return 0;
}
