// config.c
#include "config.h"
#define NUM_ROUTES 1

int NUM_ROUTES_VALUE = NUM_ROUTES;
app_context* app_ctx = NULL;

void init_app_context(sqlite3* db, int port, const char* host) {
    app_ctx = malloc(sizeof(app_context));
    app_ctx->db = db;
    app_ctx->port = port;
    app_ctx->host = host;
}
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

