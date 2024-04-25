#include "logo.h"

void terminated_screen(WINDOW *window, int timeout) {
  if (!window) window = stdscr;

  timeout(timeout);
  wclear(window);
  char terminated[4][56] = {
      " _____ ___ ___ __  __ ___ _  _   _ _____ ___ ___  ",
      "|_   _| __| _ \\  \\/  |_ _| \\| | /_\\_   _| __|   \\ ",
      "  | | | _||   / |\\/| || || .` |/ _ \\| | | _|| |) |",
      "  |_| |___|_|_\\_|  |_|___|_|\\_/_/ \\_\\_| |___|___/ ",
  };

  int x = (getmaxx(window) - 52) / 2;
  int y = (getmaxy(window) - 4) / 2;

  wattron(window, COLOR_PAIR(THEME_WARNING_PAIR));
  for (int row = 0; row < 4; row++) {
    if (row >= 3) wattron(window, WA_DIM);
    mvwprintw(window, y + row, x, "%s", terminated[row]);
    wattroff(window, WA_BOLD | WA_DIM);
  }
  wattroff(window, COLOR_PAIR(THEME_WARNING_PAIR));

  wattron(window, WA_DIM | WA_BLINK);
  const char desc[] = "PRESS ANY BUTTON TO FORCE";
  mvwprintw(window, y + 4, (getmaxx(window) - 25) / 2, "%s", desc);
  wattroff(window, WA_DIM | WA_BLINK);

  doupdate();
  getch();
}