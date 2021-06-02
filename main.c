#include <stdio.h>
#include <stdlib.h>
#include "file_controller/file_controller.h"
#include "net/udp.h"
#include <pthread.h>
#include "string.h"
#include "application_context.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Please, enter just path as program argument");
    }

    else {

        application_context *app_context = calloc(1, sizeof(application_context));
        file_description_node *list_fd_head = malloc(sizeof(file_description_node));
        app_context->list_fd_head = list_fd_head;
        app_context->exit_code = 0;
        scan_all_directories(list_fd_head, argv[1]);
        print_list(list_fd_head);

        pthread_t *udp_thread = malloc(sizeof(pthread_t));
        pthread_create(udp_thread, NULL, (void *) start_udp_listener, app_context);

        while (1) {
            char cmd[20];
            printf("enter command: ");
            scanf("%s", cmd);
            if (strcmp(cmd, "send") == 0) {
                search_other_servers(app_context, "dbinfo.sql/37/bf6a0713edc66cc2dabce97aaee06bf8", 8888);
            } else if (strcmp(cmd, "exit") == 0) {
                app_context->exit_code = 1;
                break;
            }
        }

        pthread_join(*udp_thread, NULL);


        free(list_fd_head);
    }
    return 0;
}
