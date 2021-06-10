//
// Created by kirill on 10.06.2021.
//

#ifndef LR3_NET_UI_H
#define LR3_NET_UI_H

#include "termios.h"
#include "../application_context.h"

typedef struct command_result {
    char cmd[30];
    char arg[256];
} command_result;

void start_ui(application_context *app_context);
void print_horizontal_line(char symbol[1], int count, int startX, int y);
void print_vertical_line(char symbol[1], int count, int x, int startY);
int get_pos(int *y, int *x);
command_result process_command();

#endif //LR3_NET_UI_H
