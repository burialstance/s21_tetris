#include "views.h"

// ROOT VIEW HEADER

/**
 * @brief Draws the header layout with a specific theme and content.
 *
 * This function clears the header layout's window and sets its background color
 * using a predefined color pair. It then inserts a Tetris logo and an S21 logo
 * into the window at specified positions. The `wnoutrefresh` function marks the
 * window for refresh without immediately updating the screen, allowing for more
 * efficient screen updates.
 *
 * @param self Pointer to the Layout instance representing the header.
 */
static void header_draw_handler(Layout *self) {
  if (!self) return;

  wclear(self->window);
  wbkgd(self->window, COLOR_PAIR(THEME_SURFACE_PAIR));

  insert_tetris_logo(self->window, 40, 0);
  insert_s21_logo(self->window, 20, 0);

  wnoutrefresh(self->window);
}

/**
 * @brief Adjusts the header layout's window size and position.
 *
 * This function resizes the header layout's window to match its configured
 * height and the full width of the parent window. It then moves the window to
 * the top of the parent window, ensuring it is positioned correctly. The
 * `wresize` function is used to resize the window, and `mvderwin` is used to
 * move the window to the correct position. Finally, `wnoutrefresh` marks the
 * window for refresh without immediately updating the screen, allowing for more
 * efficient screen updates.
 *
 * @param self Pointer to the Layout instance representing the header.
 */
static void header_adjust_handler(Layout *self) {
  if (!self) return;
  wresize(self->window, self->config.height, getmaxx(self->window->_parent));
  mvderwin(self->window, getbegy(self->window->_parent), 0);
  wnoutrefresh(self->window);
}

// ROOT VIEW CONTENT

/**
 * @brief Draws the content layout with a specific theme and structure.
 *
 * This function clears the content layout's window and sets its background
 * color using a predefined color pair. It then draws a box around the window
 * using the `box` function, which is a common way to visually delineate areas
 * in a text-based UI. The `wnoutrefresh` function marks the window for refresh
 * without immediately updating the screen, allowing for more efficient screen
 * updates.
 *
 * @param self Pointer to the Layout instance representing the content.
 */
static void content_draw_handler(Layout *self) {
  if (!self) return;

  wclear(self->window);

  print_window_cords(self->window);
  box(self->window, 0, 0);
  wbkgd(self->window, COLOR_PAIR(THEME_SURFACE_PAIR));

  wattron(self->window, COLOR_PAIR(THEME_BACKGROUND_PAIR));
  mvwprintw(self->window, 3, 0, "THEME_BACKGROUND_PAIR");
  wattroff(self->window, COLOR_PAIR(THEME_BACKGROUND_PAIR));

  wattron(self->window, COLOR_PAIR(THEME_SURFACE_PAIR));
  mvwprintw(self->window, 4, 0, "THEME_SURFACE_PAIR");
  wattroff(self->window, COLOR_PAIR(THEME_SURFACE_PAIR));

  wattron(self->window, COLOR_PAIR(THEME_PRIMARY_PAIR));
  mvwprintw(self->window, 5, 0, "THEME_PRIMARY_PAIR");
  wattroff(self->window, COLOR_PAIR(THEME_PRIMARY_PAIR));

  wattron(self->window, COLOR_PAIR(THEME_SECONDARY_PAIR));
  mvwprintw(self->window, 6, 0, "THEME_SECONDARY_PAIR");
  wattroff(self->window, COLOR_PAIR(THEME_SECONDARY_PAIR));

  wattron(self->window, COLOR_PAIR(THEME_WARNING_PAIR));
  mvwprintw(self->window, 8, 0, "THEME_WARNING_PAIR");
  wattroff(self->window, COLOR_PAIR(THEME_WARNING_PAIR));

  wattron(self->window, COLOR_PAIR(THEME_SUCCESS_PAIR));
  mvwprintw(self->window, 9, 0, "THEME_SUCCESS_PAIR");
  wattroff(self->window, COLOR_PAIR(THEME_SUCCESS_PAIR));

  wnoutrefresh(self->window);
}

/**
 * @brief Adjusts the content layout's window size and position based on
 * padding.
 *
 * This function calculates the height and width for the content layout's window
 * by subtracting the padding from the parent window's dimensions. It then
 * resizes the content layout's window to these calculated dimensions and moves
 * it to the position specified by the padding. The `wresize` function is used
 * to resize the window, and `mvderwin` is used to move the window to the
 * correct position. Finally, `wnoutrefresh` marks the window for refresh
 * without immediately updating the screen, allowing for more efficient screen
 * updates.
 *
 * @param self Pointer to the Layout instance representing the content.
 */
static void content_adjust_handler(Layout *self) {
  if (!self) return;

  int height = getmaxy(self->window->_parent) -
               (self->config.padding.bottom + self->config.padding.top);
  int width = getmaxx(self->window->_parent) -
              (self->config.padding.left + self->config.padding.right);

  wresize(self->window, height, width);
  mvderwin(self->window, self->config.padding.top, self->config.padding.left);
  wnoutrefresh(self->window);
}

// ROOT VIEW FOOTER

/**
 * @brief Draws the footer layout with a specific theme and content.
 *
 * This function clears the footer layout's window, sets its background color
 * using a predefined color pair, and applies a dim attribute to the text. It
 * then inserts a "cloak" component, which is a visual element typically used
 * for decoration or to obscure part of the window. Following this, it prints a
 * username in the center of the footer, adjusting the position to ensure the
 * username is centered horizontally. Finally, it removes the dim attribute from
 * the text and marks the window for refresh without immediately updating the
 * screen, allowing for more efficient screen updates.
 *
 * @param self Pointer to the Layout instance representing the footer.
 */
static void footer_draw_handler(Layout *self) {
  if (!self) return;

  wclear(self->window);
  wbkgd(self->window, COLOR_PAIR(THEME_SURFACE_PAIR));
  wattron(self->window, WA_DIM);

  insert_cloak_component(self->window, (getmaxx(self->window) - 9) / 2, 0);

  char username[] = "tyberora@student.21-school.ru";
  mvwprintw(self->window, 0, (getmaxx(self->window) - strlen(username)) - 1,
            "%s", username);

  wattroff(self->window, WA_DIM);
  wnoutrefresh(self->window);
}

/**
 * @brief Adjusts the footer layout's window size and position.
 *
 * This function resizes the footer layout's window to match its configured
 * height and adjusts its position to be at the bottom of the parent window. It
 * uses the ncurses functions `wresize` to resize the window and `mvderwin` to
 * move the window to the correct position. Finally, it marks the window for
 * refresh without immediately updating the screen, allowing for more efficient
 * screen updates.
 *
 * @param self Pointer to the Layout instance representing the footer.
 */
static void footer_adjust_handler(Layout *self) {
  if (!self) return;
  wresize(self->window, self->config.height, getmaxx(self->window->_parent));
  mvderwin(self->window, (getmaxy(self->window->_parent) - self->config.height),
           0);
  wnoutrefresh(self->window);
}

/**
 * @brief Provides a singleton instance of RootView.
 *
 * This function ensures that only one instance of RootView is created and used
 * throughout the application. It initializes the RootView with specific
 * configurations for header, content, and footer, including dimensions,
 * padding, and handlers for adjusting and drawing each component.
 *
 * @return A pointer to the singleton RootView instance.
 *         Initializes the RootView with specific configurations if it hasn't
 * been initialized before.
 */
RootView *provide_root_view() {
  static RootView *view = NULL;
  if (!view) {
    int min_width = 64;
    view =
        new_view(stdscr, (RootViewConfig){.header = {.height = 5,
                                                     .adjust_on_update = TRUE,
                                                     .min_width = min_width,
                                                     .min_height = 5},
                                          .content = {.adjust_on_update = TRUE,
                                                      .padding = {.top = 5 + 1,
                                                                  .bottom = 2,
                                                                  .left = 8,
                                                                  .right = 8},
                                                      .min_height = 24,
                                                      .min_width = min_width},
                                          .footer = {.adjust_on_update = TRUE,
                                                     .height = 1,
                                                     .min_height = 1,
                                                     .min_width = min_width}});

    view->header->adjust_window = header_adjust_handler;
    view->header->draw = header_draw_handler;

    view->content->adjust_window = content_adjust_handler;
    view->content->draw = content_draw_handler;

    view->footer->adjust_window = footer_adjust_handler;
    view->footer->draw = footer_draw_handler;
  }
  return view;
}