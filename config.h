#ifndef CONFIG_H
#define CONFIG_H

#include "common.h"
#include "controllers.h"

extern struct route routes[];  
extern int NUM_ROUTES_VALUE ;

struct route* get_routes();
int get_num_routes(); 

#endif