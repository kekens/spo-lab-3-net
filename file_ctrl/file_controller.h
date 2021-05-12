//
// Created by kirill on 12.05.2021.
//

#ifndef LR3_NET_FILE_CONTROLLER_H
#define LR3_NET_FILE_CONTROLLER_H

#include "stddef.h"

struct file_description {
    char *name;
    int size;
    char *hash;
};

struct file_description_list {
    file_description file_desc_entry;
    file_description *file_description_next;
};

void scan_dir(char *path);
long calculate_file_size(char *path);
long calculate_hash(char *path);

#endif //LR3_NET_FILE_CONTROLLER_H
