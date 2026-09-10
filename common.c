// common.c
#include "headers/common.h"

enum HTTP_Method parse_method(char* method_str) {
    if(strcmp(method_str, "GET")==0) return GET; 
    if(strcmp(method_str, "POST")==0) return POST; 
    if(strcmp(method_str, "PATCH")==0) return PATCH; 
    if(strcmp(method_str, "DELETE")==0) return DELETE; 
    return UNKNOWN; 
}

const char* http_method_to_str(enum HTTP_Method method){
    const char* str; 

    switch (method)
    {
    case GET:
        str = "GET"; 
        break;
    case DELETE:
        str = "DELETE"; 
        break;
    case POST:
        str = "POST"; 
        break;
    case PATCH:
        str = "PATCH"; 
        break;
    default:
        str = "UNKNOW"; 
        break;
    }
    return str ; 
}