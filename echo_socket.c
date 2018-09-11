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
	int cli, soc;
	struct sockaddr_in server, client;
	int len = sizeof(struct sockaddr_in);
	int data_len;
	char data[MAX];

	if((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("server socket:");
		exit(-1);
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(&server.sin_zero,8);

	if((bind(soc, (struct sockaddr *)&server, len)) < 0)
	{
		perror("bind:");
		exit(-1);
	}

	printf("binded successfully to %s:%d \n", inet_ntoa(server.sin_addr),ntohs(server.sin_port));

	if((listen(soc, 5))<0)
	{
		perror("listen:");
		exit(-1);
	}

	while(1)
	{
		if((cli = accept(soc,(struct sockaddr *)&client, &len)) < 0)
		{
			perror("accept:");
			exit(-1);
		}

		printf("connected to %s: %d \n", inet_ntoa(client.sin_addr),ntohs(client.sin_port));

		data_len =1;

		while(data_len)
		{
			data_len =  recv(cli, data, MAX, 0);

			if(data_len)
			{
				printf("server message received: %s \n", data);
				send(cli,data,data_len,0);
				data[data_len] = '\0';
				printf("server message sent: %s \n", data);
			}

		}

		printf("client disconnected \n");

		close(cli);

	}

}

				



