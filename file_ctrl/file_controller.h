//
// Created by kirill on 12.05.2021.
//

#ifndef LR3_NET_FILE_CONTROLLER_H
#define LR3_NET_FILE_CONTROLLER_H

#include "stddef.h"

typedef struct file_description_node file_description_node;

typedef struct file_description {
    char *name;
    int size;
    char *hash;
} file_description;

struct file_description_node {
    file_description file_desc_entry;
    file_description_node *file_desc_next;
};

void push_fd(file_description_node **fd_node_current, file_description fd);
void print_list(file_description_node *fd_node_head);

void scan_all_directories(file_description_node *fd_node_head, char *root_path);
void scan_dir(file_description_node **fd_node_current, char *path);
int calculate_file_size(char *path);
char* calculate_file_hash(char *path);

#endif //LR3_NET_FILE_CONTROLLER_H
