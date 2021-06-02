//
// Created by kirill on 25.05.2021.
//

#include "tcp.h"

#include <stdio.h>
#include <string.h>

#define TCP_PORT 9999

void create_tcp_socket(tcp_description *td) {
    int sockfd;
    struct sockaddr_in server_address;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("error when creating tcp socket");
        return;
    }

    memset(&server_address, 0, sizeof(server_address));

    int tcp_port = TCP_PORT;

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = tcp_port;

    while (bind(sockfd, (const struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        server_address.sin_port = htons(++tcp_port);
        printf("\nport++\n");
    }

    td->tcp_port = tcp_port;
    td->sockfd = sockfd;
    td->address = server_address;

}

void start_tcp_listener(tcp_server_thread_description *tcp_server_thread_description) {

}
