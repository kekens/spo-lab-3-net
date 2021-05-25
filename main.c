#include <stdio.h>
#include <stdlib.h>
#include "file_ctrl/file_controller.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Please, enter just path as program argument");
    }

    else {
        file_description_node *list_fd_head = malloc(sizeof(file_description_node));
        scan_all_directories(list_fd_head, argv[1]);
        print_list(list_fd_head);

        free(list_fd_head);
    }
    return 0;
}
