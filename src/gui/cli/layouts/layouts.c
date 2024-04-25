#include "layouts.h"

/**
 * @brief Draws a Layout instance.
 *
 * U can set own draw handler for any layout.
 * First make clear, place new elements on self->window
 * and then mark self->window as updateble by call wnoutrefresh(self->window)
 *
 * @param self Pointer to the Layout instance to be drawn.
 */
static void _draw(Layout *self) {
  if (!self) return;

  wclear(self->window);
  wbkgd(self->window, COLOR_PAIR(1));
  box(self->window, 0, 0);

  mvwprintw(self->window, getmaxy(self->window) - 1,
            (getmaxx(self->window) / 2) - 3, "layout");
  wnoutrefresh(self->window);
}

/**
 * @brief Handles the case where a Layout instance cannot be drawn due to
 * insufficient size.
 *
 * This function is called when a Layout instance's window dimensions are not
 * sufficient to draw the layout as intended. It first retrieves the current
 * dimensions of the window using `getmaxyx`. It then clears the window, sets
 * its background color to a warning color using a predefined color pair, and
 * draws a box around the window. The function then prints an error message
 * indicating that the terminal is too small, specifying the required dimensions
 * and the current dimensions. The error message is displayed in a blinking
 * attribute to draw attention to it. Finally, the window is marked for refresh
 * without immediately updating the screen, allowing for more efficient screen
 * updates.
 *
 * @param layout Pointer to the Layout instance that cannot be drawn due to
 * insufficient size.
 */
static void _not_enaugh_sizes(Layout *layout) {
  int width = 0, height = 0;
  getmaxyx(layout->window, height, width);

  static const char error_title[] = "SMALL TERMINAL";
  static const char error_desc[] = "required %dx%d (now %dx%d)";

  wclear(layout->window);
  wbkgd(layout->window, COLOR_PAIR(THEME_WARNING_PAIR));
  box(layout->window, 0, 0);

  wattron(layout->window, WA_BLINK);
  mvwprintw(layout->window, height / 2, ((width - strlen(error_title)) / 2),
            error_title);
  mvwprintw(layout->window, height / 2 + 1, ((width - strlen(error_desc)) / 2),
            error_desc, layout->config.min_width, layout->config.min_height,
            width, height);
  wattroff(layout->window, WA_BLINK);

  wnoutrefresh(layout->window);
}

/**
 * @brief Updates a Layout instance, adjusting its window size if necessary and
 * redrawing it.
 *
 * This function updates a Layout instance by first checking if the layout's
 * configuration specifies that the window should be adjusted on update. If so,
 * it calls the `adjust_window` function to resize the window accordingly. It
 * then retrieves the current dimensions of the window using `getmaxyx`, which
 * is a macro that updates the provided variables with the window's height and
 * width. If the window's dimensions are greater than or equal to the minimum
 * dimensions specified in the layout's configuration, the layout is drawn using
 * the `draw` function. If the dimensions are not sufficient, a function
 * `_not_enaugh_sizes` is called, presumably to handle the case where the layout
 * cannot be drawn due to insufficient size.
 *
 * @param self Pointer to the Layout instance to be updated.
 */
static void _update(Layout *self) {
  if (!self) return;

  if (self->config.adjust_on_update) {
    self->adjust_window(self);
  }

  int width = 0, height = 0;
  getmaxyx(self->window, height, width);
  if ((width >= self->config.min_width) &&
      (height >= self->config.min_height)) {
    if (self->draw) {
      self->draw(self);
    }
  } else {
    _not_enaugh_sizes(self);
  }
}

/**
 * @brief Adjusts the layout's window size and position.
 *
 * This function resizes the layout's and adjust window position to match its
 * configured height and the full width of the parent window. It then moves the
 * window to the top of the parent window, ensuring it is positioned correctly.
 * Finally, `wnoutrefresh` marks the window for refresh without
 * immediately updating the screen, allowing for more efficient screen updates.
 *
 * @param self Pointer to the Layout instance representing the header.
 */
static void _adjust_window(Layout *self) {
  if (!self) return;
}

/**
 * @brief Destroys a Layout instance, freeing its resources.
 *
 * This function is responsible for cleaning up a Layout instance. It first
 * checks if the Layout pointer is not NULL. If the Layout has an associated
 * window, it deletes the window using `delwin` and sets the window pointer to
 * NULL. Finally, it frees the memory allocated for the Layout instance using
 * `free`. This ensures that all resources associated with the Layout are
 * properly released, preventing memory leaks.
 *
 * @param self Pointer to the Layout instance to be destroyed.
 */
static void _destroy(Layout *self) {
  if (!self) return;

  if (self->window) {
    delwin(self->window);
    self->window = NULL;
  }
  free(self);
}

/**
 * @brief Creates a new layout with a specified parent window and configuration.
 *
 * This function initializes a new layout with a given parent window and
 * configuration. The parent window is used as the reference for the layout's
 * position and size. The configuration specifies the layout's dimensions,
 * minimum dimensions, adjustment behavior on update, and padding. The function
 * returns a pointer to the newly created Layout structure.
 *
 * @param parent Pointer to the parent WINDOW structure. If NULL, stdscr is used
 * as the default parent.
 * @param config LayoutConfig structure containing the layout's configuration.
 * @return Pointer to the newly created Layout structure.
 */
Layout *new_layout(WINDOW *parent, LayoutConfig config) {
  Layout *self = (Layout *)malloc(sizeof(Layout));
  if (!self) {
    fprintf(stderr, "Cannot allocate mem for Layout\n");
    exit(-1);
  }

  if (!parent) parent = stdscr;
  self->config = config;
  self->window = derwin(parent, self->config.height, self->config.width,
                        getbegy(parent) + self->config.padding.top,
                        getbegx(parent) + self->config.padding.left);

  self->draw = _draw;
  self->adjust_window = _adjust_window;
  self->update = _update;
  self->destroy = _destroy;

  return self;
}
