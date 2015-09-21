#ifndef LOGGING_H_
#define LOGGING_H_

/*
  All logging system components go here (should probably replace this with 
  syslog
*/

#define DEFAULT_ERRMSG_LEN 512
#define DEFAULT_LOGSTR_LEN 512

int log_msg(char* msg);

#endif // LOGGING_H_
