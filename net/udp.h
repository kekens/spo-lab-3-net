//
// Created by kirill on 25.05.2021.
//

#ifndef LR3_NET_UDP_H
#define LR3_NET_UDP_H

#include "../application_context.h"

typedef struct udp_search_data {
    application_context *app_context;
    char *file_str;
} udp_search_data;

typedef struct udp_answer {
    int success_result;
    int port;
    file_description_send file_desc_send;
} udp_answer;

void start_udp_listener(application_context *app_context);

void check_server(application_context *app_context, char *search_file_description, int port, int *found);

void search_other_servers(udp_search_data *udp_sd);

#endif //LR3_NET_UDP_H
