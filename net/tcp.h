//
// Created by kirill on 25.05.2021.
//
#include <netinet/in.h>

#ifndef LR3_NET_TCP_H
#define LR3_NET_TCP_H

typedef struct tcp_description {
    int port;
    int sockfd;
    struct sockaddr_in address;
} tcp_description;

void start_tcp_listener(tcp_description *td);

void start_tcp_client();

#endif //LR3_NET_TCP_H
