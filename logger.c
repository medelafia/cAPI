#include "headers/logger.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void log(const char *message)
{
    time_t currentTime = time(NULL);
    char *time_str = ctime(&currentTime);

    if (time_str == NULL) {
        return;
    }

    time_str[strcspn(time_str, "\n")] = '\0';

    printf("[%s] %s\n", time_str, message);
    fflush(stdout);
}