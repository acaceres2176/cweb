#ifndef SERVER_H_
#define SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "http.h"
#include "version.h"

#define DEFAULT_ROOT_DIR "/public_html"

/* Core server components go here (e.g. parsing and responding to HTTP
   requests, etc.) */

int parse(char* request, int len, HTTP_Request_Header* http_header);
int respond(HTTP_Request_Header* http_header, char* response);

#endif /* SERVER_H_ */
