#ifndef C_API_HEADER
#define C_API_HEADER

#include "common.h"

void capi_create_app(); 

void capi_get(const char * route, handler_t handler) ; 
void capi_post(const char * route, handler_t handler) ; 
void capi_delete(const char * route, handler_t handler) ; 
void capi_patch(const char * route, handler_t handler) ; 



#endif