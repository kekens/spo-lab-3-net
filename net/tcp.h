//
// Created by kirill on 25.05.2021.
//
#include <netinet/in.h>

#include "../application_context.h"
#include "../file_controller/file_controller.h"

#ifndef LR3_NET_TCP_H
#define LR3_NET_TCP_H

typedef struct tcp_description {
    int tcp_port;
    int sockfd;
    struct sockaddr_in address;
} tcp_description;

typedef struct tcp_server_thread_description {
    application_context *app_context;
    file_description *file_desc;
    tcp_description *tcp_description;
} tcp_server_thread_description;

void create_tcp_socket(tcp_description *td);

void start_tcp_listener(tcp_server_thread_description *tcp_server_thread_description);

void start_tcp_client();

#endif //LR3_NET_TCP_H
