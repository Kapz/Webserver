#ifndef PARSER_H_
#define PARSER_H_

#include <stdlib.h>
#include <string.h>


enum Methods{
	GET,
	POST
};


struct Request{
	enum Methods method;
	char resource[2048];
	char useragent[2048];
};


void parser(struct Request *request, char *requestdata, size_t size);

#endif // PARSER_H_
