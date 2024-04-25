#include "logo.h"

void insert_gameover_logo(WINDOW *window, int x, int y) {
  wattron(window, WA_BOLD);

  static const char gameover[4][64] = {
      " ____   __  ____  ____   __   __ _  ____  _  _   __  ",
      "/    \\ /  \\(_  _)(  _ \\ / _\\ / (/ \\(  __)/ )( \\ /  \\ ",
      ") /\\ ((  O ) )(   ) __//    \\ __  ( ) _) ) __ ((  O )",
      "\\_)(_/ \\__/ (__) (__)  \\_/\\_/  (__/(____)\\_)(_/ \\__/ "};
  for (size_t row = 0; row < 4; row++) {
    if (row <= 1) wattron(window, WA_BOLD);
    if (row >= 3) wattron(window, WA_DIM);
    mvwprintw(window, y + row, x, "%s", gameover[row]);
    wattroff(window, WA_BOLD | WA_DIM);
  }

  wattroff(window, WA_BOLD);
}