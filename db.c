#include "headers/db.h" 
#include "headers/config.h"


sqlite3 *initDB(){
    sqlite3* db;
    int rc = sqlite3_open("databases/jokes.db", &db) ; 
    if(rc!= SQLITE_OK) {
        fprintf(stderr, "Cannot open database : %s", sqlite3_errmsg(db)); 
        return NULL; 
    } 
    sqlite3_exec(db, "PRAGMA mmap_size=0;", NULL, NULL, NULL);
    const char* sql = "CREATE TABLE IF NOT EXISTS JOKES("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "joke TEXT NOT NULL"
                ");" ;
    char* messaggeError;
    int exit = 0;
    exit = sqlite3_exec(db, sql, NULL , 0 , &messaggeError);
    if(exit != -1 ) {
        sqlite3_free(messaggeError); 
    }
    printf("database init\n"); 
    return db; 
}
char* getJokes(){
    sqlite3* db = app_ctx->db ; 
    sqlite3_stmt* stmt;
    char *sql = "SELECT * FROM JOKES"; 
    
    if(sqlite3_prepare_v2(db, sql, -1 , &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    
    cJSON* data_array = cJSON_CreateArray(); 
    while(sqlite3_step(stmt) == SQLITE_ROW){
        cJSON* joke = cJSON_CreateObject(); 

        int id = sqlite3_column_int(stmt, 0); 
        const char* content = (const char*)sqlite3_column_text(stmt, 1); 

        cJSON_AddNumberToObject(joke, "id", id); 
        cJSON_AddStringToObject(joke, "joke", content); 

        cJSON_AddItemToArray(data_array, joke) ; 
    }

    char *json_str = cJSON_Print(data_array);
    sqlite3_finalize(stmt); 
    cJSON_Delete(data_array); 

    return json_str; 
}
bool createNewJoke(char* data, char* error, int * status_code){
    sqlite3* db = app_ctx->db ; 
    sqlite3_stmt* stmt;
    const char *sql = "INSERT INTO JOKES(joke) VALUES (?)"; 
    cJSON* joke = cJSON_Parse(data); 

    cJSON* joke_item = cJSON_GetObjectItem(joke, "joke");
    if (!joke_item || !joke_item->valuestring) {  
        strcpy(error, "Missing 'joke' field:");
        *status_code = 400; 
        cJSON_Delete(joke);
        return false;
    }

    if(sqlite3_prepare_v2(db, sql, -1 , &stmt, NULL) != SQLITE_OK) {
        sprintf(error, "SQL error: %s\n", sqlite3_errmsg(db));
        *status_code = 500; 
        cJSON_Delete(joke);
        return false;
    }

    const char* joke_content = joke_item->valuestring;
    if(sqlite3_bind_text(stmt, 1, joke_content, -1, SQLITE_STATIC) != SQLITE_OK){
        sprintf(error, "SQL error: %s\n", sqlite3_errmsg(db));
        *status_code = 500; 
        sqlite3_finalize(stmt); 
        cJSON_Delete(joke);
        return false;
    }

    if (sqlite3_step(stmt)!= SQLITE_DONE) {
        sprintf(error, "Execution failed: %s\n", sqlite3_errmsg(db));
        *status_code = 500; 
        sqlite3_finalize(stmt); 
        cJSON_Delete(joke);
        return false; 
    } else {
        printf("Record deleted successfully.\n");
    }
    
    sqlite3_finalize(stmt); 
    cJSON_Delete(joke);
    return true;    
}
bool deleteJokeById(char* data, char* error, int * status_code){
    sqlite3* db = app_ctx->db ; 
    sqlite3_stmt* stmt;
    const char *sql = "DELETE FROM JOKES WHERE id=?"; 
    cJSON* json_data = cJSON_Parse(data); 

    cJSON* joke_item = cJSON_GetObjectItem(json_data, "id");
    if (!joke_item || !joke_item->valueint) {  
        strcpy(error, "Missing 'id' field:");
        *status_code = 400; 
        cJSON_Delete(json_data);
        return false;
    }
    if(sqlite3_prepare_v2(db, sql, -1 , &stmt, NULL) != SQLITE_OK) {
        sprintf(error, "SQL error: %s\n", sqlite3_errmsg(db));
        *status_code = 500; 
        cJSON_Delete(json_data);
        return false;
    }

    int id = joke_item->valueint; 
    if(sqlite3_bind_int(stmt, 1, id) != SQLITE_OK){
        sprintf(error, "SQL error: %s\n", sqlite3_errmsg(db));
        *status_code = 500; 
        sqlite3_finalize(stmt); 
        cJSON_Delete(json_data);
        return false;
    }

    if (sqlite3_step(stmt)!= SQLITE_DONE) {
        sprintf(error, "Execution failed: %s\n", sqlite3_errmsg(db));
        *status_code = 500; 
        sqlite3_finalize(stmt); 
        cJSON_Delete(json_data);
        return false; 
    } else {
        printf("Record inserted successfully.\n");
    }
    
    sqlite3_finalize(stmt); 
    cJSON_Delete(json_data);
    return true;    
}
bool jokeCount(char* error, int * status_code);
