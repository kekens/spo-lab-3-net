//
// Created by kirill on 25.05.2021.
//

#include "tcp.h"
#include "../file_controller/file_controller.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

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
    server_address.sin_port = htons(tcp_port);

    while (bind(sockfd, (const struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        server_address.sin_port = htons(++tcp_port);
        printf("\nport++\n");
    }

    td->tcp_port = tcp_port;
    td->sockfd = sockfd;
    td->server_address = server_address;
}

void start_tcp_server(tcp_server_thread_description *tcp_server_thread_description) {
    int connfd, len;

    tcp_description *td = tcp_server_thread_description->tcp_description;

    if ((listen(td->sockfd, 5)) < 0) {
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

void download_file(int sockfd, file_description file_desc, application_context *app_context) {

}

void start_tcp_client(tcp_client_thread_description *tcp_client_thread_description) {
    int sockfd;
    struct sockaddr_in server_address;
    application_context *app_context = tcp_client_thread_description->app_context;
    file_description *file_desc = tcp_client_thread_description->file_desc;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        perror("socket creation failed");

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = tcp_client_thread_description->address;
    server_address.sin_port = htons(tcp_client_thread_description->port);

    if (connect(sockfd, (const struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        perror("\nconnection with server failed");
        return;
    } else {
        printf("Connected to the server\n");
    }

    char *file_name = calloc(1, 256);

    strcat(file_name, tcp_client_thread_description->file_desc->name);

    int file = open(file_name, O_CREAT | O_WRONLY, 0777);

    if (file >= 0) {
        push_fd_by_head(app_context->list_fd_head, file_desc);

        while (1) {
            int current_file_size = calculate_file_size(file_name);
            int current_percents = (int) ((((float) current_file_size) / file_desc->size) * 100);

            return;
        }
    } else {
        perror("creating downloading file failed");
    }


}
