// config.c
#include "headers/config.h"
#define NUM_ROUTES 1

int NUM_ROUTES_VALUE = NUM_ROUTES;
app_context* app_ctx = NULL;


struct handler jokes_handlers[] = {
    {GET, get_jokes_handler},
    {POST, post_jokes_handler}, 
    {DELETE, delete_joke_handler}
};

struct route routes[NUM_ROUTES] = {
    {
        .path = "/api/jokes",
        .handlers = jokes_handlers,
        .handler_count = 3
    }
};

struct route* get_routes(){
    return routes; 
}
int get_num_routes(){
    return NUM_ROUTES_VALUE; 
}
void load_config_from_json(char* error){
    FILE *fp = fopen("resources/conf.json", "r");
    if (fp == NULL) {
        printf("Error: Unable to open config file.\n");
        return ;
    }
    char buffer[1024];
    int len = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    if (len <= 0) {
        snprintf(error, 256, "Error: Failed to read config file");
        return;
    }

    buffer[len] = '\0';

    cJSON * conf_json = cJSON_Parse(buffer);
    if (conf_json == NULL) {
       snprintf(error, 256, "Error: Invalid JSON - %s", cJSON_GetErrorPtr() ? cJSON_GetErrorPtr() : "Unknown error"); 
        cJSON_Delete(conf_json);
        return ;
    }

    cJSON *host = cJSON_GetObjectItemCaseSensitive(conf_json, "host");
    app_ctx = malloc(sizeof(app_context));
    if (cJSON_IsString(host) && (host->valuestring != NULL)) {
        strncpy(app_ctx->host, host->valuestring, 
        sizeof(app_ctx->host) - 1);
            app_ctx->host[sizeof(app_ctx->host) - 1] = '\0';
    }
    cJSON *port = cJSON_GetObjectItemCaseSensitive(conf_json, "port");
    if (cJSON_IsNumber(port)) {
        app_ctx->port= port->valueint; 
    }

    // delete the JSON object
    cJSON_Delete(conf_json);
}