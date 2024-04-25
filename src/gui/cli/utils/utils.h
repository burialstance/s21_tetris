#ifndef CLI_UTILS_UTILS_H
#define CLI_UTILS_UTILS_H

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

int get_window_width(WINDOW *window);
int get_window_height(WINDOW *window);
void print_window_cords(WINDOW *window);
bool has_parent(WINDOW *window);

int min(int a, int b);
int max(int a, int b);
int random_between(int min, int max);

#endif  // !CLI_UTILS_UTILS_H