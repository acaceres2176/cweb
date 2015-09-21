#ifndef HTTP_H_
#define HTTP_H_

/*
  All HTTP constants, status codes, version numbers, etc. go here
*/

#define HTTP_VERSION_NUM "1.1"


#define HTTP_200_NAME "OK"
#define HTTP_400_NAME "Bad Request"
#define HTTP_404_NAME "Not Found"
#define HTTP_500_NAME "Internal Server Error"
#define HTTP_501_NAME "Not Implemented"

enum HTTP_Method
{
  OPTIONS = 1,
  GET = 2,
  HEAD = 3,
  POST = 4,
  PUT = 5,
  DELETE = 6,
  TRACE = 7,
  CONNECT = 8
};

typedef struct
{
  // first line of request
  enum HTTP_Method method;
  char* uri;
  int major_version;
  int minor_version;
} HTTP_Request_Header;

#endif