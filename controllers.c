// handlers.c
#include "controllers.h"




http_response get_jokes_handler(http_request* request) {
    char* json_data = getJokes(NULL);
    
    http_response resp = {
        .status_code = 200,
        .body = json_data,
        .content_type = "application/json"
    };
    
    return resp;
}

http_response post_jokes_handler(http_request* request) {
    /* Parse body, save to DB, etc */
    char* json_data = cJSON_Print(cJSON_CreateObject());
    
    http_response resp = {
        .status_code = 201,
        .body = json_data,
        .content_type = "application/json"
    };
    
    return resp;
}

http_response not_found_handler(http_request* request) {
    char* body = malloc(256);
    snprintf(body, 256, "{\"error\": \"Not Found\"}");
    
    http_response resp = {
        .status_code = 404,
        .body = body,
        .content_type = "application/json"
    };
    return resp;
}

http_response method_not_allowed_handler(http_request* request) {
    char* body = malloc(256);
    snprintf(body, 256, "{\"error\": \"Method Not Allowed\"}");
    
    http_response resp = {
        .status_code = 405,
        .body = body,
        .content_type = "application/json"
    };
    return resp;
}