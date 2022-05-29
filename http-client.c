#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
	int socket_desc;
	struct sockaddr_in server;
	char *message, server_reply[2000];

	//Create socket
	socket_desc=socket(AF_INET, SOCK_STREAM, 0);
	if(socket_desc==-1)
	{
		printf("Could not create socket");
	}

	server.sin_addr.s_addr=inet_addr("10.1.1.86");
	server.sin_family=AF_INET;
	server.sin_port=htons(8080);

	//Connect
	if(connect(socket_desc, (struct sockaddr *)&server, sizeof(server))<0)
	{
		puts("connect error");
		return 1;
	}
	puts("Connected\n");

	//Send some data
	message="connect";
	if(send(socket_desc, message, strlen(message), 0)<0)
	{
		puts("Send failed");
		return 1;
	}
	puts("Data Sent\n");
	
	//Receive a reply from the server
	if(send(socket_desc, server_reply, 2000, 0)<0)
	{
		puts("Receive failed");
		return 1;
	}
	puts("Reply received\n");
	puts(server_reply);

	return 0;
}
