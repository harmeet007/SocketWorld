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

#define MAX 100														         		//defined MAX as 100 which we take as the buffer limit while reading and writing to socket.

ssize_t writen(int ptr, const void *buffer, size_t length); 						//function decleration for writen()

void main(int argc, char **argv)										        	//main function taking port as an argument from user
{
	int cli, soc;                                									// file descriptor decleration for client - cli and server - soc
	struct sockaddr_in server, client;           									// client and server socket structure decleration
	int len = sizeof(struct sockaddr_in);	     									// calculating size of sockaddr_in structure
	int data_len;				      
	char data[MAX];
	
	//Socket creation
	if((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("server socket:");													//printing errro incase socket creation fails
		exit(-1);
	}

	server.sin_family = AF_INET;													//IPaddress family domain
	server.sin_port = htons(atoi(argv[1]));											//Input argument (Port Number 
	server.sin_addr.s_addr = htonl(INADDR_ANY);										//IP address
	bzero(&server.sin_zero,8);														//Padding for remaining flags
	
	//Socket binding
	if((bind(soc, (struct sockaddr *)&server, len)) < 0)  
	{
		perror("bind:");															//printing error for binding failure
		exit(-1);
	}
	//printing binding confirmation
	printf("binded successfully to port %d waiting for connection...\n", ntohs(server.sin_port)); 

	//listeing to at max 10 client here, can be set as variable
	if(listen(soc, 10)<0)
	{
		perror("listen:");
		exit(-1);
	}

	while(1)
	{
		//Socket accept
		if((cli = accept(soc,(struct sockaddr *)&client, &len)) < 0)		
		{
			perror("accept:");														//printing error if socket accept fails
			exit(-1);
		}

		//fork initiated for accept request recieved
		pid_t fork(void);		
		
		if (!fork()) 																//enters loop with child process only PID zero
		{
			//printing to terminal client IP and Port
			printf("server connected to %s: %d \n", inet_ntoa(client.sin_addr),ntohs(client.sin_port));		
			data_len = read(cli, data, MAX);										//reading received output to data buffer with length MAX
			
			if(data_len)															//enters loop only if data received from client
			{
				//child process created successfully
				printf("child process created for client port %d \n", ntohs(client.sin_port));			
				close(soc);															//closing child server socket
				if(writen(cli,data,data_len) < 0)									//writen function called to send recevied data back to client
				{
				perror("send");														//printing error
				}
				close(cli);															//closing child client socket
				exit(0);
			}	
			printf("client disconnected");
			
		}
		close(cli);																	//parent closing second instance of child process
	}

}

//writen() function definition
ssize_t writen(int ptr, const void *buffer, size_t length)									
{
        size_t left;
        ssize_t count;
        const char *rem;

        rem = buffer;
        left = length;
        while (left)																	//loop continues till data is in buffer
                {
                count = write(ptr, rem, left);											//writes to client buffer

                if (count < 0)
                        {
                        perror("write error:");
                        exit(-1);
                        }
                left -=count;
                rem +=count;
                }
        return length;
}

