#include "logo.h"

void insert_s21_logo(WINDOW *window, int x, int y) {
  int attrs = WA_REVERSE | WA_DIM | COLOR_PAIR(THEME_SECONDARY_PAIR);
  wattron(window, attrs);

  static const int logo[6][6] = {{1, 1, 1, 0, 1, 0},
                                 {0, 0, 0, 1, 0, 1},
                                 {0, 1, 1, 0, 0, 1},
                                 {1, 0, 0, 0, 0, 1},
                                 {0, 1, 1, 1, 0, 1}};
  for (size_t i = 0; i < 6; i++) {
    for (size_t j = 0; j < 6; j++) {
      int y_pos = y + i;
      int x_pos = x + j * 2;
      mvwprintw(window, y_pos, x_pos, "%s", logo[i][j] ? "  " : "");
    }
  }

  wattroff(window, attrs);
}