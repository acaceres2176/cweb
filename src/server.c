#include "server.h"
#include "logging.h"

// Parses HTTP request from client
int parse(char* request, int len)
{
  int type_flags[4] = {0, 0, 0, 0}; // GET, POST, PUT, DELETE
  char typestr[4] = {request[0], request[1], request[2], '\0'};
  
  // categorise request
  if(strcmp("GET\0", typestr) == 0)
  {
    type_flags[0] = 1;
  }
  else
  {
    return EXIT_FAILURE; // malformed
  }
  
  return EXIT_SUCCESS;
}

int respond(int code, char* request, char* response)
{
  if(code == 200) // HTTP 200 OK
  {
    sprintf(response, "HTTP/%s %d %s\n\n<html><head><title>HTTP %d %s</title></head><body><h1>HTTP %d %s</h1><hr><p><b>CWeb %s</b> at <b>localhost</b></p></body></html>", HTTP_VERSION_NUM, code, HTTP_200_NAME, code, HTTP_200_NAME, code, HTTP_200_NAME, CWEB_VERSION);
  }
  else if(code == 400) // HTTP 400 Bad Request
  {
    sprintf(response, "HTTP/%s %d %s\nConnection: close\n\n<html><head><title>HTTP %d %s</title></head><body><h1>HTTP %d %s</h1><hr><p><b>CWeb %s</b> at <b>localhost</b></p></body></html>", HTTP_VERSION_NUM, code, HTTP_400_NAME, code, HTTP_400_NAME, code, HTTP_400_NAME, CWEB_VERSION);
  }
  else if(code == 404)
  {
    sprintf(response, "HTTP/%s %d %s\n\n<html><head><title>HTTP %d %s</title></head><body><h1>HTTP %d %s</h1><hr><p><b>CWeb %s</b> at <b>localhost</b></p></body></html>", HTTP_VERSION_NUM, code, HTTP_404_NAME, code, HTTP_404_NAME, code, HTTP_404_NAME, CWEB_VERSION);
  }
  
  char* logstr = malloc(1028 * sizeof(char));
  
  sprintf(logstr, "[INFO] Replied to request with HTTP %d\n", code);
  
  log_msg(logstr);
  
  free(logstr);
  
  return EXIT_SUCCESS;
}