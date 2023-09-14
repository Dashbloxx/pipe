#pragma once

#include "http.h"

typedef struct
{
    char * path;
    request_t request;
    char * (* route_handler)(int index);
} route_t;

void * handler(void * argument);