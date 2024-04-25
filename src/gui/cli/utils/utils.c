#include "utils.h"

int get_window_width(WINDOW *window) { return getmaxx(window); }

int get_window_height(WINDOW *window) { return getmaxy(window); }

bool has_parent(WINDOW *window) {
  return (window && window->_parent) ? TRUE : FALSE;
}

void print_window_cords(WINDOW *window) {
  int height = get_window_height(window);
  int width = get_window_width(window);

  mvwprintw(window, height / 2, width / 2, "width=%d height=%d", width, height);
}

int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b) { return a > b ? a : b; }

int random_between(int min, int max) {
  srand(time(NULL));
  return (rand() % (max - min + 1)) + min;
}