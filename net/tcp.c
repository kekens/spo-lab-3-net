//
// Created by kirill on 25.05.2021.
//

#include "tcp.h"

#include <stdio.h>
#include <string.h>

#define TCP_PORT 11111

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
    td->server_address = server_address;

}

void start_tcp_listener(tcp_server_thread_description *tcp_server_thread_description) {
    int connfd, len;

    tcp_description *td = tcp_server_thread_description->tcp_description;

    if ((listen(td->sockfd, 5)) != 0) {
        perror("error when listening");
        return;
    }

    len = sizeof(td->client_address);

    connfd = accept(td->sockfd, (struct sockaddr *) &td->client_address, (socklen_t *) &len);
    if (connfd < 0) {
        perror("accept fail");
        return;
    }

    printf("Accepted on TCP\n");



}
