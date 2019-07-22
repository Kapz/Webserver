#include "parser.h"

void parser(struct Request *request, char *requestdata, size_t size){
	char buffer[size];
	char linebuffer[2000];
	
	char *line;
	char *field;
		
	// Extra pointers for strtok_r
	char *save_line;
	char *save_field;

	// Make copy of request that strtok doesn't edit
	strcpy(buffer, requestdata);
	line = strtok_r(buffer, "\r\n", &save_line);
	strcpy(linebuffer, line);
	field = strtok_r(linebuffer, " ", &save_field);
	
	// Get request method
	if(strcmp(field, "GET") == 0){
		request->method = GET;
	}else if(strcmp(field, "POST") == 0){
		request->method = POST;
	}else{
		// Send error
	}
	
	// Get requested resource
	field = strtok_r(NULL, " ", &save_field);
	strcpy(request->resource, "htdocs/");
	strcat(request->resource, field+1);

	
	// Parse rest of the request fields
	while((line = strtok_r(NULL, "\r\n", &save_line)) != NULL){
		strcpy(linebuffer, line);
		field = strtok_r(linebuffer, " ", &save_field);

		if(strcmp(field, "User-Agent:") == 0){
			strcpy(request->useragent, (line + strlen("User-Agent:")));
		}// Implement the rest
	}

}
