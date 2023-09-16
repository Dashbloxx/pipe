#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "handler.h"
#include "http.h"
#include "logger.h"
#include "main.h"

size_t routes_size;
route_t * routes;

uint16_t port = 8080;

char * routehandler(int index)
{
    char * buffer0 = malloc(sizeof(char) * BUFFER_SIZE);
    char * body = "<h1>Hello, world!</h1>\r\n";

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

    response.code = 200;
    response.header_size = 4;
    response.header = &headers;
    response.body = body;

    generate_response(&response, buffer0);

    return buffer0;
}

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
                sendf(stderr, LOG_ERROR, "Usage: %s -p <port>\n", argv[0]);
                return -1;
        }
    }

    routes_size = 512;
    routes = malloc(sizeof(route_t) * routes_size);

    routes[0].route_handler = &routehandler;
    routes[0].path = malloc(sizeof(char) * 1024);
    strcpy(routes[0].path, "/");

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
    {
        sendf(stderr, LOG_ERROR, "Failed to create socked...\n");
        return -2;
    }

    struct sockaddr_in host_addr;
    int host_addrlen = sizeof(host_addr);

    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(port);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sockfd, (struct sockaddr *)&host_addr, host_addrlen) != 0)
    {
        sendf(stderr, LOG_ERROR, "Failed to bind to socket...\n");
        return -2;
    }

    if(listen(sockfd, SOMAXCONN) != 0)
    {
        sendf(stderr, LOG_ERROR, "error: Failed to listen to port %d...\n", port);
        return -3;
    }

    sendf(stdout, LOG_INFO, "Listening for connections on port %d!\n", port);

    for(;;)
    {
        int newsockfd = accept(sockfd, (struct sockaddr *)&host_addr, (socklen_t *)&host_addrlen);
        if(newsockfd < 0)
        {
            sendf(stderr, LOG_ERROR, "Failed to accept connection...\n");
            continue;
        }

        pthread_t thread;

        if(pthread_create(&thread, NULL, handler, (void *)&newsockfd) != 0)
        {
            sendf(stderr, LOG_ERROR, "Failed to create a new thread...\n");
            return -4;
        }
    }

    free(routes);

    return 0;
}