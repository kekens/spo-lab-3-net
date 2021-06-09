//
// Created by kirill on 25.05.2021.
//

#include "tcp.h"
#include "../file_controller/file_controller.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>

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

    printf(";;;\n");

    tcp_description *td = tcp_server_thread_description->tcp_description;
    file_description *fd = tcp_server_thread_description->file_desc;

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

    printf("Uploading file:\n");

    char cmd[3];
    int file = open(fd->path, O_RDONLY);
    int offset = 0;

    while (strncmp(cmd, "end", 3) != 0) {
        read(connfd, &cmd, sizeof(cmd));
        if (strncmp(cmd, "dwn", sizeof(cmd))) {
            int size = 4096;
            int file_part[4096];
            pread(file, file_part, size, size * offset);
            offset++;
            write(connfd, &file_part, sizeof(file_part));
        }
    }

    printf("end\n");


}

void download_file(int sockfd, file_description file_desc, application_context *app_context) {

}

void start_tcp_client(tcp_client_thread_description *tcp_client_thread_description) {
    int sockfd;
    struct sockaddr_in server_address;
    application_context *app_context = tcp_client_thread_description->app_context;
    file_description *fd = calloc(1, sizeof(file_description));
    fd->size = tcp_client_thread_description->file_desc_send.size;
    fd->name = calloc(1, 256);
    fd->hash = calloc(1, 512);
    fd->path = calloc(1, 512);
    strcpy(fd->name, tcp_client_thread_description->file_desc_send.name);
    strcpy(fd->hash, tcp_client_thread_description->file_desc_send.hash);
    strcpy(fd->path, tcp_client_thread_description->file_desc_send.path);

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


    char *file_name = calloc(1, 512);

    printf("server suck\n%s\n", fd->name);

    strcat(file_name, fd->name);

    int file = open(fd->path, O_CREAT | O_WRONLY, 0777);

    if (file >= 0) {
        push_fd_by_head(app_context->list_fd_head, fd);

        char command[3];
        strncpy(command, "dwn", 3);
        int file_part[4096];
        int offset = 0;

        while (1) {
            int current_file_size = calculate_file_size(file_name);
            int current_percents = (int) ((((float) current_file_size) / fd->size) * 100);

            printf("1\n");
            if (current_file_size < fd->size) {
                write(sockfd, &command, sizeof(command));
                read(sockfd, &file_part, sizeof(file_part));
                pwrite(file, &file_part, 4096, offset * 0);
            } else {
                strncpy(command, "end", 3);
                write(sockfd, &command, sizeof(command));
                printf("done\n");
                return;
            }

        }
    } else {
        perror("creating downloading file failed");
    }


}
