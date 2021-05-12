#include <stdio.h>
#include "file_ctrl/file_controller.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Please, enter just path as program argument");
    }

    else {
        scan_dir(argv[1]);
    }
    return 0;
}
