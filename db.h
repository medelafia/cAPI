#ifndef DB_H 
#define DB_H
#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>
#include "cJSON.h"



sqlite3 *initDB(); 
char* getJokes(); 
bool createNewJoke(char* data , char* error, int* status_code);
bool deleteJokeById(char* data, char* error, int * status_code);
bool jokeCount(char* error, int * status_code); 

#endif