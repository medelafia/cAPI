// config.c
#include "config.h"
#define NUM_ROUTES 1

struct handler jokes_handlers[] = {
    {GET, get_jokes_handler},
    {POST, post_jokes_handler}
};

int NUM_ROUTES_VALUE = NUM_ROUTES;
struct route routes[NUM_ROUTES] = {
    {
        .path = "/api/jokes",
        .handlers = jokes_handlers,
        .handler_count = 2
    }
};

struct route* get_routes(){
    return routes; 
}
int get_num_routes(){
    return NUM_ROUTES_VALUE; 
}

