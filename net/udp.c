//
// Created by kirill on 25.05.2021.
//

#include "udp.h"
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#include "../file_controller/file_controller.h"

#define UDP_PORT 8888
#define BUF_SIZE 1024

void inform_new_instance(int count, int port) {
    int sockfd;
    struct sockaddr_in server_address;

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        return;
    }

    memset(&server_address, 0, sizeof(server_address));

    int broadcast = 1;

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("socket timer failed");
        return;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
        perror("socket broadcast failed");
        return;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    server_address.sin_port = htons(port);

    char count_str[10];

    sprintf(count_str, "%d", count);
    strcat(count_str, " instance");

    sendto(sockfd, count_str, sizeof(count_str), MSG_CONFIRM,
           (const struct sockaddr *) &server_address, sizeof(server_address));
}

void start_udp_listener(application_context *app_context) {

    int sockfd;
    struct sockaddr_in server_address, foreign_address;

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        return;
    }

    memset(&server_address, 0, sizeof(server_address));
    memset(&foreign_address, 0, sizeof(foreign_address));

    int udp_port = UDP_PORT;
    int broadcast = 1;

    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
        perror("socket broadcast failed");
        return;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(udp_port);

    while (bind(sockfd, (const struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        server_address.sin_port = htons(++udp_port);
        printf("\nport++\n");
    }

    printf("UDP started on port %d\n", udp_port);

    if (udp_port > 8888) {
        for (int i = 0; i < udp_port - 8888; i++) {
            inform_new_instance(udp_port - 8888, i + 8888);
        }
    } else {
        app_context->instance_count = 1;
    }

    while (!app_context->exit_code) {

        char *buf = calloc(1, BUF_SIZE);
        int len, n;
        memset(buf, 0, BUF_SIZE);

        len = sizeof(foreign_address);
        n = recvfrom(sockfd, buf, BUF_SIZE, MSG_WAITALL, (struct sockaddr*) &foreign_address, &len);

        file_description file_desc = find_file_description(app_context->list_fd_head, buf);

        if (strstr(buf, " instance")) {
            app_context->instance_count = buf[0] - '0';
        } else {

            if (file_desc.name != NULL) {


                sendto(sockfd,)
            }
        }

        printf("\nbuf %s\n", buf);
        printf("n %d\n", n);

    }

}

void search_other_servers(application_context *app_context, char *file_description_str, int port) {

    int sockfd;
    struct sockaddr_in server_address, foreign_address;

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        return;
    }

    memset(&server_address, 0, sizeof(server_address));
    memset(&foreign_address, 0, sizeof(foreign_address));

    int udp_port = UDP_PORT;
    int broadcast = 1;

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("socket timer failed");
        return;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
        perror("socket broadcast failed");
        return;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    server_address.sin_port = htons(9999);

    sendto(sockfd, file_description_str, strlen(file_description_str), MSG_CONFIRM,
               (const struct sockaddr *) &server_address, sizeof(server_address));

    while (1) {
        char *buf = calloc(1, BUF_SIZE);
        int n, len;

        len = sizeof(foreign_address);

        n = recvfrom(sockfd, buf, BUF_SIZE, MSG_WAITALL, (struct sockaddr *) &foreign_address, &len);

        printf("Response %d\n", n);
        printf("Response %s\n", buf);

        return;
    }

}
