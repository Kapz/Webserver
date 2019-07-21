#ifndef UTILS_H_
#define UTILS_H_

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>

struct MIME_tag{
	char *extention;
	char *MIME;
};


int checkFile(char *file);
void getMIMEtag(char *file, char *returnbuffer);
int getFileSize(char *file);

#endif
