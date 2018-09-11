#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<strings.h>
#include<errno.h>
#include<unistd.h>
#include<arpa/inet.h>

#define MAX 1024

void main(int argc, char **argv)
{
	int soc;
	struct sockaddr_in remote_server;
	int len = sizeof(struct sockaddr_in);
	char output[MAX];;
	char input[MAX];
	int data_len;

	if((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("client socket:");
		exit(-1);
	}

	remote_server.sin_family = AF_INET;
	remote_server.sin_port = htons(atoi(argv[2]));
	remote_server.sin_addr.s_addr = inet_addr(argv[1]);
	bzero(&remote_server.sin_zero,8);

	if((connect(soc,(struct sockaddr*)&remote_server,len)) < 0)
	{
		perror("client connect:");
		exit(-1);
	}

	printf("connected to %s: %d \n", inet_ntoa(remote_server.sin_addr),ntohs(remote_server.sin_port));

	while(1)
	{
		fgets(input, MAX, stdin);
		send(soc, input,strlen(input),0);
		printf("client message sent: %s\n", input);


		data_len = recv(soc, output,MAX, 0);
		output[data_len] = '\0';
		printf("server message received: %s \n", output);

	}

	close(soc);

}


