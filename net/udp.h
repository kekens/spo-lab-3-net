//
// Created by kirill on 25.05.2021.
//

#ifndef LR3_NET_UDP_H
#define LR3_NET_UDP_H

#include "../application_context.h"

void start_udp_listener(application_context *app_context);

void search_other_servers(application_context *app_context, char *search_file_description, int port);

#endif //LR3_NET_UDP_H
