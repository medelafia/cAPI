// server.c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include "headers/db.h"
#include "headers/common.h"
#include "headers/config.h"
#include "headers/controllers.h"
#include "headers/logger.h"

#define BUFFER_SIZE 4096
#define QUERY_PARAMS_COUNT 10
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"

int socket_fd; 
struct sockaddr_in server_addr; 

struct sockaddr_in client_addr; 
socklen_t addrlen = sizeof(client_addr) ; 

extern struct route routes[];
extern int NUM_ROUTES_VALUE;



void handle_shutdown(int sig) {
    log("Shutting down server...");
    close(socket_fd);
    sqlite3_close(app_ctx->db);
    exit(0);
}

void start_server() {
    socket_fd = socket(AF_INET , SOCK_STREAM, 0) ; 
    log("Starting socket now ..."); 
    if(log < 0) { 
        perror("Cannot create a server socker" ) ; 
        exit(1);
    }
    log("Socket started successfuly"); 
    
    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(app_ctx->port) ; 
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    log("Binding socket to address and port now ..."); 
    if(bind(socket_fd , (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
        perror("Bind socker failed"); 
        exit(1);
    }
    log("Socket binded to ip and port"); 

    if(listen(socket_fd, 5) < 0){
        perror("Listen failed");
        exit(1);
    }
    log("Socket listening now ...");

}
void print_startup_banner() {
    time_t now = time(NULL);
    char* time_str = ctime(&now);
    time_str[strcspn(time_str, "\n")] = '\0';

    printf("\n");
    printf(GREEN "====================================================\n" RESET);
    printf(GREEN "*     C HTTP Server\n" RESET);
    printf(GREEN "====================================================\n" RESET);
    printf(BLUE "*     Status       : running\n" RESET);
    printf(BLUE "*     Port         : %d\n" RESET, app_ctx->port );
    printf(BLUE "*     URL          : http://%s:%d/\n" RESET,app_ctx->host, app_ctx->port);
    printf(BLUE "*     PID          : %d\n" RESET, getpid());
    printf(BLUE "*     Started at   : %s\n" RESET, time_str);
    printf(BLUE "*     Author at   : Mohamed EL AFIA\n" RESET);
    printf(GREEN "====================================================\n" RESET);
    printf(GREEN "*     Press Ctrl+C to stop\n" RESET);
    printf(GREEN "====================================================\n\n" RESET);
    fflush(stdout);
}
bool compare_path_and_parameterized_route(char* path, char* parameterized_route, route_param* extracted_params, int* count) {
    char* path_copy = malloc(strlen(path) + 1);
    char* route_copy = malloc(strlen(parameterized_route) + 1);
    
    if (!path_copy || !route_copy) {
        free(path_copy);
        free(route_copy);
        return false;
    }
    
    strcpy(path_copy, path);
    strcpy(route_copy, parameterized_route);
    
    char* path_save;
    char* route_save;
    
    char* path_token = strtok_r(path_copy, "/", &path_save);
    char* route_token = strtok_r(route_copy, "/", &route_save);
    
    int param_index = 0;
    
    while (path_token && route_token) {
        
        if (route_token[0] == ':') {
            /* Parameter */
            extracted_params[param_index].name = strdup(route_token + 1);
            extracted_params[param_index].value = strdup(path_token);

            param_index++;
        } else {
            /* Static - must match */
            if (strcmp(path_token, route_token) != 0) {
                free(path_copy);
                free(route_copy);
                return false;
            }
        }
        
        path_token = strtok_r(NULL, "/", &path_save);
        route_token = strtok_r(NULL, "/", &route_save);
    }
    
    /* Both must end at same time */
    if (path_token != route_token) {  // Both NULL
        free(path_copy);
        free(route_copy);
        return false;
    }
    
    *count = param_index;
    
    free(path_copy);
    free(route_copy);
    
    return true;
}

struct query_param* parse_query_params(char* path_string, int * count) { 
    query_param* parsed_query_params = malloc(sizeof(query_param) * QUERY_PARAMS_COUNT);

    int i = 0 ; 
    char* save_amp;
    for (char* entry = strtok_r(path_string, "&", &save_amp);
        entry;
        entry = strtok_r(NULL, "&", &save_amp))
    {
        char* save_eq;
        char* key   = strtok_r(entry, "=", &save_eq);
        char* value = strtok_r(NULL, "=", &save_eq);

        parsed_query_params[i].key   = strdup(key);
        parsed_query_params[i].value = value ? strdup(value) : NULL;

        i++; 
    }

    *count = i; 

    return parsed_query_params; 
}

struct http_request* parse_request(char* request_data) {
    /* Allocate memory for request object */
    http_request* parsed_request = malloc(sizeof(http_request));
    if (!parsed_request) return NULL;
    
    char* buffer = malloc(strlen(request_data) + 1);
    strcpy(buffer, request_data);
    
    char* split_point = strstr(buffer, "\r\n\r\n");
    if (!split_point) {
        printf("ERROR: No header/body separator found\n");
        free(parsed_request);
        free(buffer);
        return NULL;
    }
    
    *split_point = '\0';              /* End the header string */
    char* headers = buffer;
    char* body = split_point + 4;     /* Skip past "\r\n\r\n" */
    
    /* Empty body for GET requests */
    if (*body == '\0') body = NULL;
    
    char method[16] = {0};
    char path[256] = {0};
    char version[16] = {0};
    
    int items = sscanf(headers, "%15s %255s %15s", method, path, version);
    
    if (items != 3) {
        printf("ERROR: Could not parse request line\n");
        free(parsed_request);
        free(buffer);
        return NULL;
    }

    char* question_mark = strchr(path, '?');
    if (question_mark) {
        *question_mark = '\0';
        char* params_str = question_mark + 1;

        if(params_str != NULL ){
            int params_count = 0;
            query_param* params = parse_query_params(params_str, &params_count);

            parsed_request->query_params = params; 
            parsed_request->query_params_count = params_count; 
        }
    }

    parsed_request->method = parse_method(method);      /* Convert to enum */
    parsed_request->path = strdup(path);                /* Copy path */         
    parsed_request->body = body ? strdup(body) : NULL;  /* Copy body if exists */

    free(buffer);  /* Free the temporary copy */

    return parsed_request;  /* Return the filled request object */
}



void* handle_request(void* arg){ 
    int client_socket = *(int*)arg; 
    free(arg);

    char buffer[BUFFER_SIZE] = {0}; 
    char response_header[512];
    char logs_buffer[256] ; 
    ssize_t bytes_read = read(client_socket, buffer, BUFFER_SIZE - 1);
    
    if (bytes_read < 0) {
        perror("Read error");
        close(client_socket);
        return NULL;
    }
    
    if (bytes_read == 0) {
        printf("Client disconnected without sending data\n");
        close(client_socket);
        return NULL;
    }
    buffer[bytes_read] = '\0';

    http_request* request = parse_request(buffer); 

    if (!request) {
        fprintf(stderr, "Failed to parse request\n");
        const char* error = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
        write(client_socket, error, strlen(error));
        close(client_socket);
        return NULL;
    }

    http_response response = not_found_handler(NULL);  /* Default 404 */
    route_param* route_params = malloc(sizeof(route_param) * 10) ; 
    int route_params_count = 0; 

    for(int i = 0; i < NUM_ROUTES_VALUE; i++) {
        if(strcmp(request->path, routes[i].path) == 0) {
            route_params_count = 0;
        } else if(routes[i].has_path_param) {
            if(!compare_path_and_parameterized_route(request->path, routes[i].path, route_params, &route_params_count)) {
                continue;
            }
        } else {
            /* No match and route has no params */
            continue;
        }
        
        if(route_params_count > 0){
            request->path_params = route_params; 
            request->path_params_count = route_params_count; 
        }else {
            request->path_params = NULL;
            request->path_params_count = 0;
        }

        /* Find handler for this method */
        for(int j = 0; j < routes[i].handler_count; j++) {
            if(routes[i].handlers[j].method == request->method) {
                response = routes[i].handlers[j].callback(request);
                goto send_response;
            }
        }

        /* Method not allowed */
        response = method_not_allowed_handler(request); 
        goto send_response;
    }       

    send_response:
    snprintf(response_header, sizeof(response_header),
        "HTTP/1.1 %d OK\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: GET, POST, DELETE, PATCH, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %lu\r\n"
        "\r\n",
        response.status_code,
        response.content_type,
        strlen(response.body));
    
    write(client_socket, response_header, strlen(response_header));
    write(client_socket, response.body, strlen(response.body));

    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, ip_str, sizeof(ip_str));
    const char* method_str = http_method_to_str(request->method); 
    
    snprintf(logs_buffer, sizeof(logs_buffer), "%s - \"%s %s\" - %d",
        ip_str,
        method_str,
        request->path,
        response.status_code
    );

    log(logs_buffer);
    
    if (request) {
        /* Free query params */
        if (request->query_params) {
            for (int i = 0; i < request->query_params_count; i++) {
                if (request->query_params[i].key) 
                    free(request->query_params[i].key);
                if (request->query_params[i].value) 
                    free(request->query_params[i].value);
            }
            free(request->query_params);
        }
        
        /* Free path params - includes name and value strings */
        if (request->path_params && request->path_params_count > 0) {
            for (int i = 0; i < request->path_params_count; i++) {
                if (request->path_params[i].name) 
                    free(request->path_params[i].name);
                if (request->path_params[i].value) 
                    free(request->path_params[i].value);
            }
            /* Don't free the array itself - it's route_params which we free below */
        }
        
        /* Free request fields */
        if (request->path) free(request->path);
        if (request->body) free(request->body);
        free(request);
    }
 
    free(response.body);
    close(client_socket);

    return NULL;
}

int loop() { 
    int client_socket ; 
    char logs_buffer[256] ; 

    print_startup_banner(); 
    signal(SIGINT, handle_shutdown) ; 
    start_server(); 
    
    while(1) { 
        client_socket = accept(socket_fd, (struct sockaddr*)&client_addr, &addrlen); 
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        pthread_t thread_id;
        int* socket_ptr = malloc(sizeof(int));
        *socket_ptr = client_socket;
        
        if (pthread_create(&thread_id, NULL, handle_request, socket_ptr) < 0) {
            perror("Thread creation failed");
            close(client_socket);
            free(socket_ptr);
            continue;
        }
        

        pthread_detach(thread_id);
    }

    close(socket_fd); 
}