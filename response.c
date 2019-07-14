#include "response.h"

void sendResponse(int client_fd, int code, struct Request *request){
	char template[] = "HTTP/1.0 200 OK\r\n"
					  "Connection: close\r\n"
					  "Content-Type: text/html\r\n"
					  "Content-Length: %ld\r\n\r\n";	

	char buffer[2000];
	struct stat st;	

	printf("Type: %s, Path: %s\n", request->type, request->filepath);
	
	if(checkFile(request->filepath) != -1){
		stat(request->filepath, &st);
		sprintf(buffer, template, st.st_size);
		send(client_fd, buffer, strlen(buffer), 0);
		sendFile(client_fd, request->filepath);
	}else{
		sendCode(client_fd, 404);
	}	
}



void sendCode(int client_fd, int code){
	char template[] = "HTTP/1.0 %s\r\n"
					  "Connection: close\r\n"
					  "Content-Type: text/html\r\n\r\n";
	
	char templatefile[] = "HTTP/1.0 %s\r\n"
						  "Connection: close\r\n"
						  "Content-Type: text/html\r\n"
						  "Content-Length: %ld\r\n\r\n";

	char buffer[2000];
	struct stat st;
	

	if(code == 404){
		if(checkFile("404.html")== -1){
			sprintf(buffer, template, "404 Not found");
			send(client_fd, buffer, strlen(buffer), 0);
		}else{
			stat("404.html", &st);
			sprintf(buffer, templatefile, "404 Not found", st.st_size);
			send(client_fd, buffer, strlen(buffer), 0);
			sendFile(client_fd, "404.html");
				
		}	
	} 

}



void sendFile(int client_fd, char *filepath){
	struct stat st;
	int file_fd;
 	
	stat(filepath, &st);
	file_fd = open(filepath, O_RDONLY);
	
	if(file_fd == -1){
		sendCode(client_fd, 404);
	}else{
		sendfile(client_fd, file_fd, NULL, st.st_size);
	}
	close(file_fd);
}


int checkFile(char *filename){
	int file_fd;
	file_fd = open(filename, O_RDONLY);
	if(file_fd == -1){
		return -1;
	}else{
		close(file_fd);
		return 0;
	}
}
