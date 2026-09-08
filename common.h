#ifndef COMMON_H
#define COMMON_H
#include <string.h>

enum HTTP_Method {
    GET, POST, DELETE, PATCH, UNKNOWN
};

typedef struct {
    enum HTTP_Method method;
    char* path; 
    char* body;
} http_request;

typedef struct {
    int status_code;  
    char* body;  
    char* content_type; 
} http_response;

typedef http_response (*handler_t)(http_request* request);

struct handler {
    enum HTTP_Method method;
    handler_t callback;
};

struct route {
    char* path;
    struct handler *handlers;
    int handler_count;
};

enum HTTP_Method parse_method(char* method_str);  
const char* http_method_to_str(enum HTTP_Method method);  

#endif