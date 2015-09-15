#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "http.h"
#include "version.h"

int parse(char* request, int len);
int respond(int code, char* request, char* response);

#endif // SERVER_H_