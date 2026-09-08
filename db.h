#ifndef DB_H 
#define DB_H
#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>
#include "cJSON.h"



sqlite3 *initDB(); 
char* getJokes(sqlite3* db); 
bool createNewJoke(sqlite3* db, char* data);
bool deleteJokeById(sqlite3* db, int id);
bool jokeCount(sqlite3* db);

#endif