#ifndef CLI_COMPONENTS_LOGO_LOGO_H
#define CLI_COMPONENTS_LOGO_LOGO_H

#include <ncurses.h>

#include "../../theme/theme.h"

void insert_s21_logo(WINDOW *window, int x, int y);
void insert_tetris_logo(WINDOW *window, int x, int y);
void insert_gameover_logo(WINDOW *window, int x, int y);
void insert_pause_logo(WINDOW *window, int x, int y);
void terminated_screen(WINDOW *window, int timeout);

#endif