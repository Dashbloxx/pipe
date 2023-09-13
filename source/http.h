#pragma once

#include <stddef.h>

typedef struct
{
    char * key;
    char * value;
} header_t;

typedef struct
{
    int code;
    size_t header_size;
    header_t * header;
    char * body;
} response_t;

int parse_header(char * input, header_t * output);
int generate_header(header_t * input, char * output);
int generate_response(response_t * input, char * output);