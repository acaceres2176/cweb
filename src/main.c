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

#define DEFAULT_PORT "80"
#define DEFAULT_BUF_LEN 512
#define DEFAULT_RESPONSE "HTTP/1.1 200 OK\nConnection: close\n\n<html><head><title>Hello, world!</title></head><body><p>Hello, world!</p></body></html>\n\n"

int main(void)
{
  int status = 0;
  int sock, new_sock = 0;
  struct addrinfo hints;
  struct addrinfo *servinfo;
  
  struct sockaddr_storage their_addr;
  socklen_t addr_size;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP
  hints.ai_flags = AI_PASSIVE; // automatically find IP
  
  status = getaddrinfo(NULL, DEFAULT_PORT, &hints, &servinfo);
  
  if(status != 0) 
  {
    fprintf(stderr, "[ERROR] Failed to obtain address information: %s\n", gai_strerror(status));
   
    return EXIT_FAILURE;
  }
  // servinfo now points to a linked list of 1 or more struct addrinfos
  // ... do everything until you don't need servinfo anymore ....
  
  sock = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
  
  if(sock == -1)
  {
    fprintf(stderr, "[ERROR] Failed to obtain socket. System said: %s\n", strerror(errno));
    
    close(sock);
    freeaddrinfo(servinfo);
    
    return EXIT_FAILURE;
  }
  
  status = bind(sock, servinfo->ai_addr, servinfo->ai_addrlen);

  // error handling
  if(status == -1)
  {
    fprintf(stderr, "[ERROR] Failed to bind socket. System said: %s\n", strerror(errno));
    
    close(sock);
    freeaddrinfo(servinfo);
        
    return EXIT_FAILURE;
  }
  
  status = listen(sock, 1);
  
  // error handling
  if(status == -1)
  {
    fprintf(stderr, "[ERROR] Failed to listen on socket. System said: %s\n", strerror(errno));
  
    close(sock);
    freeaddrinfo(servinfo);
    
    return EXIT_FAILURE;
  }
  
  printf("Waiting for requests over the wire...\n");
  
  addr_size = sizeof their_addr;
  new_sock = accept(sock, (struct sockaddr *)&their_addr, &addr_size);
  
  char* recvbuf = malloc(DEFAULT_BUF_LEN * sizeof(char));
  
  status = recv(new_sock, recvbuf, DEFAULT_BUF_LEN, 0);
  
  if(status == -1)
  {
    fprintf(stderr, "[ERROR] Failed to receive on socket. System said: %s\n", strerror(errno));
    
    free(recvbuf);
    close(new_sock);
    close(sock);
    freeaddrinfo(servinfo);
    
    return EXIT_FAILURE;
  }
  
  printf("%s", recvbuf);
  
  send(new_sock, DEFAULT_RESPONSE, strlen(DEFAULT_RESPONSE), 0);
  
  free(recvbuf);
  close(new_sock);
  close(sock);
  freeaddrinfo(servinfo); // free the linked-list  

  return EXIT_SUCCESS;
}