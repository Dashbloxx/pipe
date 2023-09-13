#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "http.h"

int parse_header(char * input, header_t * output)
{
    output->key = strtok(input, ": ");

    if(output->key != NULL)
    {
        output->value = strtok(NULL, ": ");
        
        if(output->value != NULL)
        {
            return 0;
        }

        return -2;
    }

    return -1;
}

int generate_header(header_t * input, char * output)
{
    if(input == NULL)
    {
        return -1;
    }

    if(output == NULL)
    {
        return -1;
    }

    if(input->key == NULL)
    {
        return -1;
    }

    if(input->value == NULL)
    {
        return -1;
    }

    strcpy(output, input->key);
    strcat(output, ": ");
    strcat(output, input->value);
    strcat(output, "\r\n");

    return 0;
}

int generate_response(response_t * input, char * output)
{
    if(input == NULL)
    {
        return -1;
    }
    
    if(output == NULL)
    {
        return -2;
    }

    if(input->code == -1)
    {
        return -3;
    }

    if(input->header == NULL)
    {
        return -4;
    }

    if(input->body == NULL)
    {
        return -5;
    }

    switch(input->code)
    {
    case 200:
        strcpy(output, "HTTP/1.0 200 OK\r\n");
        break;
    default:
        return -6;
    }

    for(int i = 0; i < input->header_size; i++)
    {
        char * buffer0 = malloc(sizeof(char) * 1024);
        if(buffer0 == NULL)
        {
            return -7;
        }

        if(generate_header(&input->header[i], buffer0) != 0)
        {
            return -8;
        }

        strcat(output, buffer0);

        free(buffer0);
    }

    strcat(output, input->body);
}