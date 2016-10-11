#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "logging.h"

#define DEFAULT_LOG_FILE "log/master.log"

int log_msg(char* msg)
{
    FILE* file = fopen(DEFAULT_LOG_FILE, "a");
  
    if(file == NULL)
    {
        fprintf(stderr, "[ERROR] Failed to open log file. System said: %s\n", strerror(errno));
  
        return EXIT_FAILURE;
    }
  
    time_t current_time = 0;
    time(&current_time);
  
    struct tm* timeinfo = localtime(&current_time);
  
    char timestr[40];
    strftime(timestr, 40, "%Y-%m-%dT%H:%M:%S %Z", timeinfo);
  
    if(ferror(file) != 0)
    {
        fprintf(stderr, "[ERROR] Failed to write to log file. System said: %s\n", strerror(errno));
    }
    
    fprintf(file, "%s %s\n", timestr, msg);
  
    fclose(file);
  
    return EXIT_SUCCESS;
}
