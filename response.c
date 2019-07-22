#include "response.h"

struct StatusCode status_codes[] = {{404, "404 Not Found", "<a>File not found on server</a>"},
							      {400, "400 Bad Request", "<a>Bad request -  request was invalid</a>"},
								  {500, "500 Internal Server Error", "<a>Internal server error</a>"},
								  {501, "501 Not Implemented", "<a>Request type not implemented</a>"}};



// Send response based on Request struct
void sendResponse(int client_fd, struct Request *request){
	char responseheaders[2000];
	char MIME_tag[100];
	char date[1000];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);	
	int filesize;	
		
	// POST is not implemented yet
	if(request->method == POST){
		sendCode(client_fd, 501);
		return;
	}
	
			
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
	char responseheaders[2000];
	char date[1000];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);

	
	for(int i = 0; i < sizeof(status_codes); i++){
		if(code == status_codes[i].code){
			sprintf(responseheaders, "HTTP/1.0 %s\r\n", status_codes[i].text);
			send(client_fd, responseheaders, strlen(responseheaders), 0);
			strftime(date, sizeof(date), "%a, %d %b %Y %H:%M:%S %Z", &tm);
			send(client_fd, responseheaders, strlen(responseheaders), 0);
			sprintf(responseheaders, "Date: %s\r\n", date);
			send(client_fd, responseheaders, strlen(responseheaders), 0);
			sprintf(responseheaders, "Content-Length: %d\r\n", strlen(status_codes[i].html));
			send(client_fd, responseheaders, strlen(responseheaders), 0);
			sprintf(responseheaders, "Content-Type: text/html\r\n");
			send(client_fd, responseheaders, strlen(responseheaders), 0);
			sprintf(responseheaders, "Connection: close\r\n\r\n");
			send(client_fd, responseheaders, strlen(responseheaders), 0);
			
			send(client_fd, status_codes[i].html, strlen(status_codes[i].html), 0);	
			
		}
	}
}
