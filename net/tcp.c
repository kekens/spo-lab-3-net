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
    printf("server file path %s\n", fd->path);

    char cmd[3];
    int file = open(fd->path, O_RDONLY);
    int offset = 0;

    strncpy(cmd, "xxx", 3);
    tcp_server_file_answer file_answer;

    while (strncmp(cmd, "end", 3) != 0) {
        read(connfd, &cmd, sizeof(cmd));
        if (strncmp(cmd, "dwn", sizeof(cmd)) == 0) {
            int size = 4096;
            if (size*offset + 4096 > fd->size) {
                size = fd->size - size * offset;
            }
            file_answer.file_part_len = size;
            printf("size %d\n", size);
            pread(file, file_answer.file_part, size, 4096 * offset);
            offset++;
            write(connfd, &file_answer, sizeof(file_answer));
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


    char *file_path = calloc(1, 1024);

    printf("%s\n", app_context->root_path);
    strcpy(file_path, app_context->root_path);
    strcat(file_path, "/");
    strcat(file_path, fd->name);
    printf("final file %s\n", file_path);

    int file = open(file_path, O_CREAT | O_WRONLY, 0777);

    if (file >= 0) {
        printf("file created\n");
        push_fd_by_head(app_context->list_fd_head, fd);

        char command[3];
        strncpy(command, "dwn", 3);
        int offset = 0;
        int current_size = 0;
        tcp_server_file_answer file_answer;

        while (1) {
            int current_file_size = calculate_file_size(file_path);
            int current_percents = (int) ((((float) current_file_size) / fd->size) * 100);

            printf("1 %d %d\n", current_size, fd->size);
            if (current_size < fd->size) {
                write(sockfd, &command, sizeof(command));
                read(sockfd, &file_answer, sizeof(file_answer));
                printf("2 %d\n", file_answer.file_part_len);
                pwrite(file, &file_answer.file_part, file_answer.file_part_len, offset * 4096);
                current_size += 4096;
                offset++;
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
