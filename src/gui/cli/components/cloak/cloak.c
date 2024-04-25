#include "cloak.h"

/**
 * @brief Inserts a cloak component displaying the current time at a specified
 * position within a window.
 *
 * This function inserts a cloak component into a specified window at given x
 * and y coordinates. The cloak component displays the current time, formatted
 * as hours, minutes, and seconds. It uses the `time` function to get the
 * current time, `localtime` to convert it to local time, and `strftime` to
 * format the time string. The formatted time string is then printed to the
 * window at the specified position.
 *
 * @param window The ncurses window where the cloak component will be inserted.
 * @param x The x-coordinate where the cloak component will be placed.
 * @param y The y-coordinate where the cloak component will be placed.
 */
void insert_cloak_component(WINDOW *window, int x, int y) {
  time_t t = time(NULL);
  struct tm *tm_info = localtime(&t);
  char time_str[9];
  strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_info);

  mvwprintw(window, y, x, "%s", time_str);
}