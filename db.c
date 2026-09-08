#include "db.h" 

sqlite3 *initDB(){
    sqlite3* db;
    int rc = sqlite3_open("databases/jokes.db", &db) ; 
    if(rc!= SQLITE_OK) {
        fprintf(stderr, "Cannot open database : %s", sqlite3_errmsg(db)); 
        return NULL; 
    } 
    const char* sql = "CREATE TABLE IF NOT EXISTS JOKES("
                "id INTEGER PRIMARY KEY AUTO_INCREMENT,"
                "joke TEXT NOT NULL"
                ");" ;
    char* messaggeError;
    int exit = 0;
    exit = sqlite3_exec(db, sql, NULL , 0 , &messaggeError);
    if(exit != -1 ) {
        sqlite3_free(messaggeError); 
    }
    return db; 
}
char* getJokes(sqlite3* db){
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "name", "John Doe");
    cJSON_AddNumberToObject(json, "age", 30);
    cJSON_AddStringToObject(json, "email", "john.doe@example.com");


    char *json_str = cJSON_Print(json);
    cJSON_Delete(json);

    return json_str; 
}
bool createNewJoke(sqlite3* db, char* data);
bool deleteJokeById(sqlite3* db, int id);
bool jokeCount(sqlite3* db);
