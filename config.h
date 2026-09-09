#ifndef CONFIG_H
#define CONFIG_H

#include "common.h"
#include "controllers.h"


typedef struct {
    sqlite3* db;
    int port;
    const char* host;
} app_context;

extern app_context* app_ctx;
extern struct route routes[];  
extern int NUM_ROUTES_VALUE ;

struct route* get_routes();
int get_num_routes(); 
void init_app_context(sqlite3* db, int port, const char* host);
#endif