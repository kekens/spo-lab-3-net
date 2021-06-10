//
// Created by kirill on 10.06.2021.
//

#include "esc.h"
#include "ui.h"
#include <string.h>
#include <pthread.h>
#include <sys/select.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "../file_controller/file_controller.h"

static struct termios stored_settings;

void set_keypress(void)
{
    struct termios new_settings;

    tcgetattr(0,&stored_settings);

    new_settings = stored_settings;

    new_settings.c_lflag &= (~ICANON & ~ECHO);
    new_settings.c_cc[VTIME] = 1;
    new_settings.c_cc[VMIN] = 1;

    tcsetattr(0,TCSANOW,&new_settings);

    return;
}

void reset_keypress(void)
{
    tcsetattr(0,TCSANOW,&stored_settings);
    return;
}

void start_ui(application_context *app_context) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    clrscr();
    home();

    set_display_atrib(BRIGHT);

    // Downloading and Uploading frame
    int subFrameWidth= (w.ws_col - 8) / 2;
    int subFrameHeight = (w.ws_row - 12) / 2;
    int downloadingXStart = 4;
    int downloadingYStart = 2;
    int uploadingXStart = downloadingXStart + subFrameWidth + 4;
    int uploadingYStart = downloadingYStart;

    gotoxy(downloadingXStart,downloadingYStart);
    printf("Downloading\n");
    print_horizontal_line("-", subFrameWidth, downloadingXStart, downloadingYStart + 1);

    gotoxy(uploadingXStart, uploadingYStart);
    printf("Uploading\n");
    print_horizontal_line("-", subFrameWidth, uploadingXStart, uploadingYStart + 1);

    print_vertical_line("|", subFrameHeight, downloadingXStart - 1, downloadingYStart + 2);
    print_vertical_line("|", subFrameHeight, downloadingXStart + subFrameWidth, downloadingYStart + 2);
    print_vertical_line("|", subFrameHeight, uploadingXStart - 1, uploadingYStart + 2);
    print_vertical_line("|", subFrameHeight, uploadingXStart + subFrameWidth, uploadingYStart + 2);

    print_horizontal_line("-", subFrameWidth, downloadingXStart, downloadingYStart + subFrameHeight + 1);
    print_horizontal_line("-", subFrameWidth, uploadingXStart, downloadingYStart + subFrameHeight + 1);

    //Action/events log frame
    int logXStart = downloadingXStart;
    int logYStart;
    int logFrameWidth = subFrameWidth * 2 + 4;
    int logFrameHeight = subFrameHeight + 2;
    cursor_down(2);
    set_cursor_column(logXStart);
    fflush(stdout);

    get_pos(&logYStart, &logXStart);
    printf("Action/events log\n");
    print_horizontal_line("-", logFrameWidth, logXStart, logYStart + 1);
    print_vertical_line("|", logFrameHeight , logXStart - 1, logYStart + 2);
    print_vertical_line("|", logFrameHeight , logXStart + logFrameWidth, logYStart + 2);
    print_horizontal_line("-", logFrameWidth, logXStart, logYStart + logFrameHeight + 1);

    //Command line frame
    int cmdXStart = logXStart;
    int cmdYStart;
    int cmdFrameWidth = logFrameWidth;
    int cmdFrameHeight = 2;
    cursor_down(2);
    set_cursor_column(cmdXStart);
    fflush(stdout);

    get_pos(&cmdYStart, &cmdXStart);
    printf("Command line\n");
    print_horizontal_line("-", cmdFrameWidth, cmdXStart, cmdYStart + 1);
    print_vertical_line("|", cmdFrameHeight , cmdXStart - 1, cmdYStart + 2);
    print_vertical_line("|", cmdFrameHeight , cmdXStart + cmdFrameWidth, cmdYStart + 2);
    print_horizontal_line("-", cmdFrameWidth, cmdXStart, cmdYStart + cmdFrameHeight + 1);

    cursor_up(1);
    set_cursor_column(cmdXStart);
    printf(" > ");
    fflush(stdout);

    int commandEnterX;
    int commandEnterY;

    get_pos(&commandEnterY, &commandEnterX);
    command_result cmd_res = {0};

    while (1) {
        cmd_res = process_command();
//        printf("\n\n\n%s\n\n%s\n", cmd_res.cmd, cmd_res.arg);
//        printf("\n\n\n|%s|\n\n", cmd_res.cmd);
//        printf("\n\n\n|%s|\n\n", cmd_res.arg);
        if (strcmp(cmd_res.cmd, "show") == 0) {
            file_description *file_desc = find_file_description(app_context->list_fd_head, cmd_res.arg);
            printf("%s/%s/%d\n\n", file_desc->name, file_desc->hash, file_desc->size);
        }
        gotoxy(commandEnterX, commandEnterY);
        clear_string_until_end();
//        _exit(0);
    }

}

command_result process_command() {
    char cmd[30];
    int count_space = 0;
    command_result cmd_res = {0};

    fgets(cmd, 30, stdin);
    cmd[strlen(cmd) - 1] = 0;


    for (int i = 0; i < strlen(cmd); i++) {
        if (cmd[i] == ' ') {
            count_space++;
        }
    }


    if (count_space > 1) {

    } else {
        if (count_space == 1) {
            char *c;
            int index;
            c = strchr(cmd, ' ');
            index = (int) (c - cmd);
            char substr_cmd[30] = {0};
            char substr_arg[256] = {0};

            for (int i = 0; i < index; i++) {
                substr_cmd[i] = cmd[i];
            }

            int j = 0;
            for (int i = index + 1; i < strlen(cmd); i++) {
                substr_arg[j] = cmd[i];
                j++;
            }

            if (strcmp(substr_cmd, "download") || strcmp(substr_arg, "show")) {
                strcpy(cmd_res.cmd, substr_cmd);
                strcpy(cmd_res.arg, substr_arg);
            }
        } else if (strcmp(cmd, "close") == 0) {
            strcpy(cmd_res.cmd, "close");
        }

    }

    return cmd_res;
}

void print_horizontal_line(char symbol[1], int count, int startX, int y) {
    gotoxy(startX, y);
    for (int i = 0; i < count; i++) {
        printf("%c", symbol[0]);
    }
    fflush(stdout);
}

void print_vertical_line(char symbol[1], int count, int x, int startY) {
    int y = startY;
    for (int i = 0; i < count - 1; i++) {
        gotoxy(x, y);
        printf("%c\n", symbol[0]);
        y++;
    }
}

int get_pos(int *y, int *x) {

    char buf[30]={0};
    int ret, i, pow;
    char ch;

    *y = 0; *x = 0;

    struct termios term, restore;

    tcgetattr(0, &term);
    tcgetattr(0, &restore);
    term.c_lflag &= ~(ICANON|ECHO);
    tcsetattr(0, TCSANOW, &term);

    write(1, "\033[6n", 4);

    for( i = 0, ch = 0; ch != 'R'; i++ )
    {
        ret = read(0, &ch, 1);
        if ( !ret ) {
            tcsetattr(0, TCSANOW, &restore);
            return 1;
        }
        buf[i] = ch;
    }

    if (i < 2) {
        tcsetattr(0, TCSANOW, &restore);
        return(1);
    }

    for( i -= 2, pow = 1; buf[i] != ';'; i--, pow *= 10)
        *x = *x + ( buf[i] - '0' ) * pow;

    for( i-- , pow = 1; buf[i] != '['; i--, pow *= 10)
        *y = *y + ( buf[i] - '0' ) * pow;

    tcsetattr(0, TCSANOW, &restore);
    return 0;
}