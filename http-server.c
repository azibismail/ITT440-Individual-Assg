#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#include <mcrypt.h>

int encrypt(char* IV, char* key, int key_len)
{
	MCRYPT td = mcrypt_module_open("azibismail-2302", NULL, "xyz", NULL);
	int blocksize = mcrypt_enc_get_block_size(td);

	mcrypt_generic_init(td, key, key_len, IV);
	mcrypt_generic_deinit(td);
	mcrypt_module_close(td);

	return 0;
}

int decrypt(char* IV, char* key, int key_len)
{
	MCRYPT td = mcrypt_module_open("azibismail-2302", NULL, "xyz", NULL);
	int blocksize = mcrypt_enc_get_block_size(td);

	mcrypt_generic_init(td, key, key_len, IV);
        mcrypt_generic_deinit(td);
        mcrypt_module_close(td);

        return 0;
}

void display(char* ciphertext, int len)
{
	int v;
	for (v=0; v<len; v++)
	{
		printf("%d ", ciphertext[v]);
	}
	printf("\n ");
}

int main()
{
	int socket_desc, new_socket, c;
	struct sockaddr_in server, client;
	char *message;
	
	FILE *file;
	file = fopen("index.html", "r");
	char response[2048];
	char header[2048] = "HTTP/1.1 200 OK\r\n\n";
	fgets(response, 1024, file);
	strcat(header, response);

	char* IV = "AAAAAAAAAAAAAAAA";
	char* key = "0123456789abcdef";
	int keysize = 32;

	//Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_desc==-1)
	{
		printf("Could not create socket");
	}

	//Prepare the sockaddr_in structure
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(8080);

	//Bind
	if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("bind failed");
		return 1;
	}
	puts("bind done");

	//Listen
	listen(socket_desc, 5);
	printf("cipher: ");
	printf("decrypt:\n");

	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c=sizeof(struct sockaddr_in);
	while((new_socket=accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)))
	{
		puts("Conection accepted");
		
		//Reply to the client
		message="Hello Client, I have received your connection.\n";
		write(new_socket, message, strlen(message));
	}
	
	if(new_socket<0)
	{
		perror("Accept failed");
		return 1;
	}
	return 0;
}
