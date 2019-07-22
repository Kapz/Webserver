#include "utils.h"


struct MIME_tag MIME_tags[] = {{"html", "text/html"},
							   {"css", "text/css"},
							   {"jpg", "image/jpeg"}, 
							   {"jpeg", "image/jpeg"},
	 						   {"js", "text/javascript"},
							   {"png", "image/png"},
							   {"json", "application/json"},
							   {"gif", "image/gif"}};



// Check if file exists
int checkFile(char *file){
	int file_fd;
	
	file_fd = open(file, O_RDONLY);

	if(file_fd == -1){
		close(file_fd);
		return -1;
	}else{
		close(file_fd);
		return 0;
	}
	
	return -1;
}


// Get MIME tag based on file extention
void getMIMEtag(char *file, char *returnbuffer){
	char *extention;
	char buffer[200];

	// Make copy for strtok
	strcpy(buffer, file);
	extention = strtok(buffer, ".");
	extention = strtok(NULL, ".");
	
	for(int i = 0; i < sizeof(MIME_tags); i++){
		if(strcmp(extention, MIME_tags[i].extention) == 0){
			strcpy(returnbuffer, MIME_tags[i].MIME);
			return;
		}
	}
	
	strcpy(returnbuffer, "text/plain");

}


// Get the filesize of a file
int getFileSize(char *file){
	struct stat st;
	stat(file, &st);
	return st.st_size;
}
