#ifndef REPONSE_H_
#define RESPONSE_H_

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "utils.h"
#include "parser.h" // For Request struct

void sendResponse(int client_fd, struct Request *request);
void sendFile(int client_fd, char *file);
void sendCode(int client_fd, int code);


#endif // RESPONSE_H_
