#include "parser.h"


int parser(char *buffer, struct Request *request){
	char *word;	

	if(strncmp(buffer, "GET", 3) == 0){
		strcpy(request->type, "GET\0");
		word = strtok(buffer, " ");
		word = strtok(NULL, " ");
		strcpy(request->filepath, word+1);

	}else if(strncmp(buffer, "POST", 4) == 0){
		strcpy(request->type, "POST\0");
		word = strtok(buffer, " ");
		word = strtok(NULL, " ");
		strcpy(request->filepath, word);

	}else{
		
	}

	return 0;
	
}

