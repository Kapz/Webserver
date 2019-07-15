#ifndef RESPONSE_H_
#define RESPONSE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>

#include "parser.h"


void sendResponse(int client_fd, int code, struct Request *request);
void sendCode(int client_fd, int code);
void sendFile(int client_fd, char *filepath);
int checkFile(char *filename);
void getMIMETag(char *filename, char *buffer);

#endif
