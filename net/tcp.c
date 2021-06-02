//
// Created by kirill on 25.05.2021.
//

#include "tcp.h"

#include <stdio.h>
#include <string.h>

#define TCP_PORT 9999

void start_tcp_listener(tcp_description *td) {
    int sockfd;
    struct sockaddr_in server_address;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("error when creating tcp socket");
        return;
    }

    memset(&server_address, 0, sizeof(server_address));

    int tcp_port = TCP_PORT;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = tcp_port;




}
