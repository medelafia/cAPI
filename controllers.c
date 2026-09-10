// handlers.c
#include "headers/controllers.h"
#include "headers/db.h"


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
    char * body= malloc(150); 
    char* error = malloc(100); 
    int* status_code ; 
    bool inserted = createNewJoke(request->body, error , status_code);   

    if(!inserted) { 
        snprintf(body, 255, "{\"error\": \"%s\"}", error) ; 
        http_response resp = {
            .status_code = *status_code,
            .body = body,
            .content_type = "application/json"
        };
        free(status_code);
        return resp; 
    }

    snprintf(body, 100, "{ \"message\" : \"created\"}");
    http_response resp = {
        .status_code = 201,
        .body = body,
        .content_type = "application/json"
    };
    
    return resp;
}

http_response delete_joke_handler(http_request* request) {
    /* Parse body, save to DB, etc */
    char * body= malloc(150); 
    char* error = malloc(100); 
    int* status_code ; 
    bool inserted = deleteJokeById(request->body, error , status_code);   

    if(!inserted) { 
        snprintf(body, 255, "{\"error\": \"%s\"}", error) ; 
        http_response resp = {
            .status_code = *status_code,
            .body = body,
            .content_type = "application/json"
        };
        free(status_code);
        return resp; 
    }

    snprintf(body, 100, "{ \"message\" : \"deleted\"}");
    http_response resp = {
        .status_code = 200,
        .body = body,
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