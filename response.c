#include "response.h"


// Send response based on Request struct
void sendResponse(int client_fd, struct Request *request){
	char responseheaders[2000];
	char MIME_tag[100];
	char date[1000];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);	
	int filesize;	
			
	if(checkFile(request->resource) != -1){
		getMIMEtag(request->resource, MIME_tag);
		filesize = getFileSize(request->resource);		
			
		sprintf(responseheaders, "HTTP/1.0 200 OK\r\n");
		send(client_fd, responseheaders, strlen(responseheaders), 0);
		strftime(date, sizeof(date), "%a, %d %b %Y %H:%M:%S %Z", &tm);
		send(client_fd, responseheaders, strlen(responseheaders), 0);
		sprintf(responseheaders, "Date: %s\r\n", date);
		send(client_fd, responseheaders, strlen(responseheaders), 0);
		sprintf(responseheaders, "Content-Length: %d\r\n", filesize);
		send(client_fd, responseheaders, strlen(responseheaders), 0);
		sprintf(responseheaders, "Content-Type: %s\r\n", MIME_tag);
		send(client_fd, responseheaders, strlen(responseheaders), 0);
		sprintf(responseheaders, "Connection: close\r\n\r\n");
		send(client_fd, responseheaders, strlen(responseheaders), 0);
		
		sendFile(client_fd, request->resource);
	}else{
		sendCode(client_fd, 404);
	}


}


void sendFile(int client_fd, char *file){
	int file_fd;
	int filesize;

	filesize = getFileSize(file);
	file_fd = open(file, O_RDONLY);

	sendfile(client_fd, file_fd, NULL, filesize);
}



// Send reponse code
void sendCode(int client_fd, int code){
	char response[2000];
	char date[1000];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);

	if(code == 404){
		char resp[] = "<h1>File not found on server</h1>";
			
		sprintf(response, "HTTP/1.0 404 Not Found\r\n");
		send(client_fd, response, strlen(response), 0);
		strftime(date, sizeof(date), "%a, %d %b %Y %H:%M:%S %Z", &tm);
		send(client_fd, response, strlen(response), 0);
		sprintf(response, "Date: %s\r\n", date);
		send(client_fd, response, strlen(response), 0);

		if(checkFile("404.html") == -1){
			sprintf(response, "Content-Length: %d\r\n", strlen(resp));
			send(client_fd, response, strlen(response), 0);
			sprintf(response, "Content-Type: text/html\r\n");
			send(client_fd, response, strlen(response), 0);
			sprintf(response, "Connection: close\r\n\r\n");
			send(client_fd, response, strlen(response), 0);
			sprintf(response, resp);
			send(client_fd, response, strlen(response), 0);
		}else{
			sprintf(response, "Content-Length: %d\r\n", getFileSize("404.html"));
			send(client_fd, response, strlen(response), 0);
			sprintf(response, "Content-Type: text/html\r\n");
			send(client_fd, response, strlen(response), 0);
			sprintf(response, "Connection: close\r\n\r\n");
			send(client_fd, response, strlen(response), 0);
			sprintf(response, resp);
			send(client_fd, response, strlen(response), 0);
			sendFile(client_fd, "404.html");
		}
	}// Add rest of codes
}
