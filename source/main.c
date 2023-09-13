#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <getopt.h>

#include "http.h"

uint16_t port = 8080;

int main(int argc, char * argv[])
{
    int option;

    while((option = getopt(argc, argv, "p:")) != -1)
    {
        switch(option)
        {
            case 'p':
                port = (uint16_t)atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s -p <port>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    char * helloworld = "Hello, world!";
    char hiworld[1024];

    header_t headers[4];
    headers[0].key = "Key1";
    headers[0].value = "Value1";
    headers[1].key = "Key2";
    headers[1].value = "Value2";
    headers[2].key = "Key3";
    headers[2].value = "Value3";
    headers[3].key = "Key4";
    headers[3].value = "Value4";

    response_t response;

    response.code = 123;
    response.header_size = 4;
    response.header = &headers;
    response.body = helloworld;

    generate_response(&response, &hiworld);

    printf("%s\n", hiworld);

    return 0;
}