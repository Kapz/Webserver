#include "response.h"


// Send respnse based on request struct
void sendResponse(int client_fd, int code, struct Request *request){
	char template[] = "HTTP/1.0 200 OK\r\n"
					  "Connection: close\r\n"
					  "Content-Type: %s\r\n"
					  "Content-Length: %ld\r\n\r\n";	

	char buffer[2000];
	char MIME[50];
	struct stat st;	

	getMIMETag(request->filepath, MIME);
	printf("Type: %s, Path: %s\n MIME: %s", request->type, request->filepath, MIME);
	
	if(checkFile(request->filepath) != -1){
		stat(request->filepath, &st);
		sprintf(buffer, template, MIME, st.st_size);
		send(client_fd, buffer, strlen(buffer), 0);
		sendFile(client_fd, request->filepath);
	}else{
		sendCode(client_fd, 404);
	}	
}


// Send error code
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


// Send file to the socket fd
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


// Check if file exists
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


// Return MIME tag based on file extention
void getMIMETag(char *filepath, char *buffer){
	char *word;
	char filename[25];
	
	strcpy(filename, filepath);
	word = strtok(filename, ".");
	word = strtok(NULL, ".");	
	
	if(strcmp(word, "html") == 0){
		strcpy(buffer, "text/html\0");
	}else if(strcmp(word, "css") == 0){
		strcpy(buffer, "text/css\0";)
	}
}



