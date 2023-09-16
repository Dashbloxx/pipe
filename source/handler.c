#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#include "handler.h"
#include "http.h"
#include "logger.h"
#include "main.h"

void * handler(void * argument)
{
    int sockfd = *((int *)argument);

    char buffer0[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    int client_addrlen = sizeof(client_addr);

    int sockn = getsockname(sockfd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addrlen);
    if(sockn < 0)
    {
        sendf(stderr, LOG_ERROR, "Failed to get client's address...\n");
        pthread_exit(NULL);
    }

    int valread = read(sockfd, buffer0, BUFFER_SIZE);
    if(valread < 0)
    {
        sendf(stderr, LOG_ERROR, "Failed to read from socket...\n");
        pthread_exit(NULL);
    }

    char method[BUFFER_SIZE], uri[BUFFER_SIZE], version[BUFFER_SIZE];
    sscanf(buffer0, "%s %s %s", method, uri, version);
    sendf(stdout, LOG_DEBUG, "Client %s:%s made a %s request to %s!\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), method, uri);

    char * response0;
    bool has_route_been_found = false;

    for(int i = 0; i < routes_size; i++)
    {
        if(strcmp(routes->path, uri) == 0)
        {
            has_route_been_found = true;
            response0 = routes->route_handler(i);
        }
    }

    if(has_route_been_found == false)
    {
        response0 = malloc(sizeof(char) * BUFFER_SIZE);
        char * body = "<h1>Not found...</h1>\r\n";

        header_t headers[1];
        headers[0].key = "Content-type";
        headers[0].value = "text/html";

        response_t response1;

        response1.code = 404;
        response1.header_size = 1;
        response1.header = &headers;
        response1.body = body;

        generate_response(&response1, response0);
    }

    int valwrite = write(sockfd, response0, strlen(response0));
    if(valwrite < 0)
    {
        sendf(stderr, LOG_ERROR, "Failed to write to socket...\n");
        pthread_exit(NULL);
    }

    free(response0);

    close(sockfd);
}