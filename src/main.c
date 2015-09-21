#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "server.h"
#include "logging.h"

#define DEFAULT_PORT "80"
#define DEFAULT_BUF_LEN 512
#define DEFAULT_RESPONSE "HTTP/1.1 200 OK\nConnection: close\n\n<html><head><title>Hello, world!</title></head><body><p>Hello, world!</p></body></html>\n\n"
#define DEFAULT_BACKLOG 16

int main(void)
{
  int status = 0;
  int sock, new_sock = 0;
  struct addrinfo hints;
  struct addrinfo *servinfo;
  
  HTTP_Request_Header http_header;
  http_header.method = 0;
  http_header.uri = "";
  http_header.major_version = 0;
  http_header.minor_version = 0;
  
  char* errmsg = malloc(DEFAULT_ERRMSG_LEN * sizeof(char));
  
  struct sockaddr_storage their_addr;
  socklen_t addr_size;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP
  hints.ai_flags = AI_PASSIVE; // automatically find IP
  
  status = getaddrinfo(NULL, DEFAULT_PORT, &hints, &servinfo);
  
  if(status != 0) 
  {
    sprintf(errmsg, "[ERROR] Failed to receive on socket. System said: %s\n", strerror(errno));
    fprintf(stderr, errmsg);
    log_msg(errmsg);
      
    return EXIT_FAILURE;
  }
  // servinfo now points to a linked list of 1 or more struct addrinfos
  // ... do everything until you don't need servinfo anymore ....
  
  sock = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
  
  if(sock == -1)
  {
    sprintf(errmsg, "[ERROR] Failed to receive on socket. System said: %s\n", strerror(errno));
    fprintf(stderr, errmsg);
    log_msg(errmsg);
    
    close(sock);
    freeaddrinfo(servinfo);
    
    return EXIT_FAILURE;
  }
  
  status = bind(sock, servinfo->ai_addr, servinfo->ai_addrlen);

  // error handling
  if(status == -1)
  {
    sprintf(errmsg, "[ERROR] Failed to receive on socket. System said: %s\n", strerror(errno));
    fprintf(stderr, errmsg);
    log_msg(errmsg);
      
    close(sock);
    freeaddrinfo(servinfo);
        
    return EXIT_FAILURE;
  }
  
  printf("Waiting for requests over the wire...\n");
  log_msg("Waiting...");
  
  char* recvbuf = malloc(DEFAULT_BUF_LEN * sizeof(char));
  char* response = malloc(DEFAULT_BUF_LEN * sizeof(char));
  
  int alive = 1;
  
  while(alive == 1)
  {
    status = listen(sock, DEFAULT_BACKLOG);
  
  // error handling
  if(status == -1)
  {
    sprintf(errmsg, "[ERROR] Failed to receive on socket. System said: %s\n", strerror(errno));
    fprintf(stderr, errmsg);
    log_msg(errmsg);
      
    close(sock);
    freeaddrinfo(servinfo);
    
    return EXIT_FAILURE;
  }
    
    addr_size = sizeof their_addr;
    new_sock = accept(sock, (struct sockaddr *)&their_addr, &addr_size);
    
    status = recv(new_sock, recvbuf, DEFAULT_BUF_LEN, 0);
  
    if(status == -1)
    {
      sprintf(errmsg, "[ERROR] Failed to receive on socket. System said: %s\n", strerror(errno));
      fprintf(stderr, errmsg);
      log_msg(errmsg);
      
      free(response);
      free(recvbuf);
      close(new_sock);
      close(sock);
      freeaddrinfo(servinfo);
    
      return EXIT_FAILURE;
    }
  
    //printf("%s", recvbuf);
  
    status = parse(recvbuf, DEFAULT_BUF_LEN, &http_header);
    
    if(status == EXIT_FAILURE)
    {
      // respond with HTTP 400
      //respond(&http_header, response);
      alive = 0;
    }
    else
    {
      respond(&http_header, response);
      //alive = 0;
      //send(new_sock, DEFAULT_RESPONSE, strlen(DEFAULT_RESPONSE), 0);
    }
    
    send(new_sock, response, DEFAULT_BUF_LEN, 0);
    close(new_sock);
  }
  
  free(errmsg);
  free(response);
  free(recvbuf);
  close(new_sock);
  close(sock);
  freeaddrinfo(servinfo); // free the linked-list  

  return EXIT_SUCCESS;
}