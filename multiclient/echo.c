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

#define MAX 100																		//defined MAX as 100 which we take as the buffer limit while reading and writing to socket.

ssize_t writen(int ptr, const void *buffer, size_t length);							//function decleration for writen()
ssize_t readline(int ptr, void *buffer, size_t length);								//function decleration for readline()

int main(int argc, char **argv)														//main function taking server IP and Port as an argument from user
{
	int soc;																		// file descriptor decleration for remote server - soc
	struct sockaddr_in remote_server;												// remote server socket structure decleration
	int len = sizeof(struct sockaddr_in);                                           // calculating size of sockaddr_in structure
	char output[MAX];;
	char input[MAX];
	int data_len;
	int sent;

	if((soc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("client socket:");													//printing errro incase socket creation fails
		exit(-1);
	}

	remote_server.sin_family = AF_INET;												//IPaddress family domain
	remote_server.sin_port = htons(atoi(argv[2]));                                  //Input argument (Port Number
	remote_server.sin_addr.s_addr = inet_addr(argv[1]);                             //IP address
	bzero(&remote_server.sin_zero,8);                                               //Padding for remaining flags

	if((connect(soc,(struct sockaddr*)&remote_server,len)) < 0)
	{
		perror("client connect:");													//printing errro for socket connect failure
		exit(-1);
	}

	//Displaying to user IP and port of connected server 
	printf("client connected to %s: %d \n", inet_ntoa(remote_server.sin_addr),ntohs(remote_server.sin_port));
	/*printf("connected.\n");*/
	printf("send a message to the server! \n");

	fgets(input, MAX, stdin);														//user input stream taken
	sent = writen(soc, input, strlen(input));										//writen() function call to write to remote server socket
	printf("client message sent: %s\n", input);	
	data_len = readline(soc, output,MAX);											//readline() function call to read from server side
	if ( data_len <0)	
	{ perror("socket receive error:");												//error while receiving data
		exit(-1);}	
	else if ( data_len == 0)														//no input received
	{ printf("no message received from socket");	
		exit(-1);}	
	else {																			
	printf("server message received: %s \n", output);								//displays the echoed back data to end user
		}	
    
	close(soc);	
	return 0;	
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
	        count = write(ptr, rem, left);                                          //writes to server buffer

		if (count < 0)	
			{
			perror("write error:");													//prints the error incase write fails
			exit(-1);
			}
		left -=count;
		rem +=count;
		}
	return length;	
}

//readline() function definition
ssize_t readline(int ptr, void *buffer, size_t length)
{
	ssize_t curread;
	size_t totalread;
	char *line;
	char word;																		//varaible to read each charcter
	line = buffer;
	totalread = 0;
	
	if (length < 0)													
        {
             perror("read error:");
             exit(-1);
        }

	while (length >= 0)
	{	
		curread = read(ptr,&word,1);												//reads each character into the output buffer
		
		if (errno == EINTR)
		{    
			continue;
		}
		
	
		else if (curread < 0)		
		{
			perror("read error:");													
			exit(-1);
		}
		else if (curread == 0)
		{
			if (totalread == 0)
				return 0;
			else
				break;

		}
		else 
		{
			if (totalread < (length - 1))
			{
				totalread++;				
				*line++ = word;
			}
			
			if ( word == '\n')														//breaks after receving new line character (\n)
				break;
		}
	}

	
	*line++ = '\0';																	//appends nul-terminated string at the end of the stream for displaying to the end user
	return totalread;
}

				
		
