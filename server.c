#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "parser.h"
#include "response.h"

#define PORT 8081
#define BACKLOG 10

void handleRequest(int client_fd, char *buffer, size_t size);


int main(int argc, char *argv[]){
	struct sockaddr_in server_addr, client_addr;
	int server_fd, client_fd;
	char buffer[2048];
	int file;
	struct stat st;
		
	
	// Create socket
	if(((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)){
		printf("Socket error: %s\n", strerror(errno));
		exit(0);		
	}
	
	// Set server struct
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(server_addr.sin_zero), 0, 8);
	
	// Bind port and socket
	if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0){
		printf("Bind error: %s\n", strerror(errno));
		exit(0);
	}

	// Listen
	if(listen(server_fd, BACKLOG) < 0){
		printf("Listen error: %s\n", strerror(errno));
		exit(0);
	}


	socklen_t sin_size = sizeof(struct sockaddr_in);

	while(1){
		if(((client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &sin_size))) < 0){
			printf("Connect error: %s\n", strerror(errno));
			exit(0);
			continue;
		}

		if(!fork()){
			close(server_fd);
			memset(buffer, 0, 2047);
			recv(client_fd, buffer, 2047, 0);
			//printf("%s", buffer);

			handleRequest(client_fd, buffer, sizeof(buffer));

			close(file);		
			close(client_fd);
		}
		
		close(client_fd);
	}
	

	close(server_fd);
	return 0;
}



void handleRequest(int client_fd, char *buffer, size_t size){
	struct Request request;
	parser(&request, buffer, strlen(buffer));
	printf("Resource: %s\n", request.resource);	
	sendResponse(client_fd, &request);
}



