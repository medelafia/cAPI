#include "headers/c_api.h"
#include "headers/server.h"
#include "headers/config.h"
#include "headers/db.h"

void capi_create_app() { 
    char* error = malloc(255) ; 

    load_config_from_json(error); 
    app_ctx->db = initDB(); 
    loop(); 
}


int main() {
    capi_create_app(); 

    return 0; 
}
