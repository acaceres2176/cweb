#include <errno.h>

#include "server.h"
#include "logging.h"
#include "hash.h"

int num_bytes_until(char* str, int len)
{
    int i = 0;
    int bytes = 0;
    char* chunk;

    for(i=0;i<len;i++)
    {
        chunk = &str[i+1];

        if(strcmp(str, chunk) < 0)
        {
            break;
        }
        else
        {
            bytes++;
        }
    }

    return bytes;
}

// Parses HTTP request from client
int parse(char* request, int len, HTTP_Request_Header* http_header)
{
    int i = 0;
    char* line;
    //char* cell;
    char* current_pos;
    char* first_pos = strstr(request, "\r\n\r\n");

    char* errmsg = malloc(512 * sizeof(char));

    // find how many bytes into the request the header ends
    for(i=0;i<len;i++)
    {
        current_pos = &request[i];
  
        if(current_pos == first_pos)
        {
            break;
        }
    }

    char* header = malloc(i * sizeof(char));

    if(header == NULL)
    {
        sprintf(errmsg, "[ERROR] Failed to allocate sufficient memory. System"\
                " said: %s", strerror(errno));
        fprintf(stderr, "%s\n", errmsg);
        log_msg(errmsg);
    }

    strncpy(header, request, i);

    i = 0;

    // count the number of times we encounter CRLFs in header
    int num_crlfs = 0;

    for(i=0;i<len;i++)
    {
        if(header[i] == '\r')
        {
            if(header[i+1] == '\n')
            {
                num_crlfs++;
            }
        }
    }

    i = 0;

    char** lines = malloc(num_crlfs * sizeof(char*));

    if(lines == NULL)
    {
        sprintf(errmsg, "[ERROR] Failed to allocate sufficient memory."\
                "System said: %s", strerror(errno));
        fprintf(stderr, "%s\n", errmsg);
        log_msg(errmsg);
    }

    line = strtok(header, "\r\n"); // get the first line from the header

    // keep getting lines from strtok and adding them to lines array
    while(line != NULL)
    {
        lines[i] = malloc(strlen(line) * sizeof(char));
  
        if(lines[i] == NULL)
        {
            sprintf(errmsg, "[ERROR] Failed to allocate sufficient memory."\
                    "System said: %s", strerror(errno));
            fprintf(stderr, "%s\n", errmsg);
            log_msg(errmsg);
        }
  
        strncpy(lines[i], line, strlen(line));
        line = strtok(NULL, "\r\n");
        i++;
    }

    i = 0;

    /*
     * Parse the first line of the request header and retrieve 
     */

    char* word = strtok(header, " ");

    while(word != NULL)
    {
        if(i == 0)
        {
            if(strcmp(word, "OPTIONS") == 0)
            {
                http_header->method = 1;
            }
            else if(strcmp(word, "GET") == 0)
            {
                http_header->method = 2;
            }
            else if(strcmp(word, "HEAD") == 0)
            {
                http_header->method = 3;
            }
            else if(strcmp(word, "POST") == 0)
            {
                http_header->method = 4;
            }
            else if(strcmp(word, "PUT") == 0)
            {
                http_header->method = 5;
            }
            else if(strcmp(word, "DELETE") == 0)
            {
                http_header->method = 6;
            }
            else if(strcmp(word, "TRACE") == 0)
            {
                http_header->method = 7;
            }
            else if(strcmp(word, "CONNECT") == 0)
            {
                http_header->method = 8;
            }
        }
        else if(i == 1)
        {
            http_header->uri = malloc((strlen(word) + 1) * sizeof(char));
            strncpy(http_header->uri, word, strlen(word));
            http_header->uri[strlen(word)] = '\0';
            break;
        }
  
        word = strtok(NULL, " ");
        i++;
    }

    i = 0;

    printf("Request for %s received\n", http_header->uri);

    free(header);
    free(errmsg);

    return EXIT_SUCCESS;
}

int respond(HTTP_Request_Header* http_header, char* response)
{
    char* errmsg = malloc(DEFAULT_ERRMSG_LEN * sizeof(char));
    char* logstr = malloc(DEFAULT_LOGSTR_LEN * sizeof(char));
  
    if(http_header->method == GET)
    {
        size_t file_size;
        char* file_contents;

        char* file_path = malloc(1028 * sizeof(char));
    
        if(strcmp(http_header->uri, "/") == 0)
        {
            sprintf(file_path, "%s/index.html", DEFAULT_ROOT_DIR);
        }
        else
        {
            sprintf(file_path, "%s/%s", DEFAULT_ROOT_DIR, http_header->uri);
        }
    
        FILE* fp = fopen(file_path, "r");
    
        if(fp == NULL)
        {
            sprintf(errmsg, "[ERROR] Failed to read from file. System "\
                    "said: %s", strerror(errno));
            fprintf(stderr, "%s\n", errmsg);
            log_msg(errmsg);
            sprintf(response, "HTTP/%s %d %s\n\n<html><head><title>HTTP %d %s"\
                                "</title></head><body><h1>HTTP %d %s</h1><hr>"\
                                "<p><b>CWeb %s</b> at <b>localhost</b></p>"\
                                "</body></html>", HTTP_VERSION_NUM, 404, 
                                HTTP_404_NAME, 404, HTTP_404_NAME, 404, 
                                HTTP_404_NAME, CWEB_VERSION);
      
            sprintf(logstr, "[INFO] Replied to request for %s with HTTP 404", 
                            http_header->uri);
            log_msg(logstr);
     
            free(errmsg);
            free(logstr);
      
            return EXIT_SUCCESS;
        }
    
        /* obtain file size */
        fseek(fp, 0, SEEK_END);
        file_size = ftell(fp);
        rewind(fp);
    
        file_contents = malloc(file_size * sizeof(char));
    
        fread(file_contents, sizeof(char), file_size, fp);
    
        fclose(fp);
    
        sprintf(response, "HTTP/%s %d %s\r\nServer: CWeb/%s (Unix)\r\n"\
                            "Content-Type: text/html; charset=utf-8\r\n"\
                            "Content-Length: %zu\r\n\r\n%s", HTTP_VERSION_NUM,
                            200, HTTP_200_NAME, CWEB_VERSION, file_size, 
                            file_contents);
    
        free(file_contents);
        free(file_path);
    
        sprintf(logstr, "[INFO] Replied to request for %s", http_header->uri);
        log_msg(logstr);
    }
  
    free(logstr);
    free(errmsg);
  
    return EXIT_SUCCESS;
}
