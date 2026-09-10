#ifndef CONTROLLERS_H
#define CONTROLLERS_H

#include <stdlib.h>
#include "cJSON.h"
#include "common.h"
#include "db.h"

http_response get_jokes_handler(http_request* request); 
http_response post_jokes_handler(http_request* request); 
http_response not_found_handler(http_request* request); 
http_response method_not_allowed_handler(http_request* request); 
http_response delete_joke_handler(http_request* request);
#endif