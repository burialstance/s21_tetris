#include "logo.h"

void insert_tetris_logo(WINDOW *window, int x, int y) {
  static const char logo_sm[4][30] = {
      " _____ ___ _____ ___ ___ ___ ",
      "|_   _| __|_   _| _ \\_ _/ __|",
      "  | | | _|  | | |   /| |\\__ \\",
      "  |_| |___| |_| |_|_\\___|___/",
  };

  for (int row = 0; row < 4; row++) {
    if (row <= 1) wattron(window, WA_BOLD);
    if (row >= 3) wattron(window, WA_DIM);
    mvwprintw(window, y + row, x, "%s", logo_sm[row]);
    wattroff(window, WA_BOLD | WA_DIM);
  }
}