//
// Created by kirill on 12.05.2021.
//
#include "file_controller.h"
#include "dirent.h"
#include "sys/types.h"
#include "stdio.h"
#include "string.h"
#include "stddef.h"

void scan_dir(char *path) {
    DIR *dir;
    struct dirent *entry;
    size_t len = strlen(path);

    if (!(dir = opendir(path))) {
        printf("Unable to open directory");
        return;
    }

    puts(path);
    while ((entry = readdir(dir)) != NULL) {
        char *name = entry->d_name;
        if (entry->d_type == DT_DIR) {
            if (!strcmp(name, ".") || !strcmp(name, ".."))
                continue;

            path[len] = '/';
            strcpy(path + len + 1, name);
            scan_dir(path);
            path[len] = '\0';
        } else if (entry->d_type == DT_REG){
            printf("AAAAA");
        } else {
            printf("%s/%s\n", path, name);
            }
    }
    closedir(dir);
}

