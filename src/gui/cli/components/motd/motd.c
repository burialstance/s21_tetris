#include "motd.h"

/**
 * @brief Inserts a themes selection block into a specified window.
 *
 * This function creates a subwindow within a specified ncurses window at given
 * x and y coordinates. It renders a block that allows users to select from
 * different themes, such as default, dark, and light.
 *
 * @param window The ncurses window where the themes selection block will be
 * inserted.
 * @param x The x-coordinate where the block will start.
 * @param y The y-coordinate where the block will start.
 * @param props The properties of the MotD component, including attributes for
 * styling.
 */
static void insert_themes_block(WINDOW *window, int x, int y,
                                MotdComponentProps props) {
  if (!window) return;

  int width = 24;
  int height = 8;

  WINDOW *wrapper = derwin(window, height, width, y, x);
  box(wrapper, 0, 0);
  wbkgd(wrapper, props.attrs);

  wattron(wrapper, WA_REVERSE);
  mvwprintw(wrapper, 1, 1, "%s", " Themes               ");
  wattroff(wrapper, WA_REVERSE);

  mvwprintw(wrapper, 3, 3, "%s", "default");
  mvwprintw(wrapper, 4, 3, "%s", "dark");
  mvwprintw(wrapper, 5, 3, "%s", "light");

  wattron(wrapper, WA_DIM);
  mvwprintw(wrapper, 3, 12, "%s", "shift + 1");
  mvwprintw(wrapper, 4, 12, "%s", "shift + 2");
  mvwprintw(wrapper, 5, 12, "%s", "shift + 3");
  wattroff(wrapper, WA_DIM);

  wnoutrefresh(window);
  delwin(wrapper);
  wrapper = NULL;
}

/**
 * @brief Inserts a brightness adjustment block into a specified window.
 *
 * This function creates a subwindow within a specified ncurses window at given
 * x and y coordinates. It renders a block that show to user brightness ajust
 * abilities of the display, with buttons to increase or decrease the brightness
 *
 * @param window The ncurses window where the brightness adjustment block will
 * be inserted.
 * @param x The x-coordinate where the block will start.
 * @param y The y-coordinate where the block will start.
 * @param props The properties of the MotD component, including attributes for
 * styling.
 */
static void insert_brightness_block(WINDOW *window, int x, int y,
                                    MotdComponentProps props) {
  if (!window) return;

  int width = 24;
  int height = 7;

  WINDOW *wrapper = derwin(window, height, width, y, x);
  box(wrapper, 0, 0);
  wbkgd(wrapper, props.attrs);

  wattron(wrapper, WA_REVERSE);
  mvwprintw(wrapper, 1, 1, "%s", " Brightness           ");
  wattroff(wrapper, WA_REVERSE);

  mvwprintw(wrapper, 3, 3, "%s", "increase");
  mvwprintw(wrapper, 4, 3, "%s", "decrease");

  wattron(wrapper, WA_DIM);
  mvwprintw(wrapper, 3, 12, "%s", "key ]");
  mvwprintw(wrapper, 4, 12, "%s", "key [");
  wattroff(wrapper, WA_DIM);

  wnoutrefresh(window);
  delwin(wrapper);
  wrapper = NULL;
}

/**
 * @brief Inserts a player keyboard configuration block into a specified window.
 *
 * This function creates a subwindow within a specified ncurses window at given
 * x and y coordinates. It renders a block that displays the keyboard controls
 * for a player, including standard movement keys, action keys, and custom
 * commands.
 *
 * @param window The ncurses window where the player keyboard configuration
 * block will be inserted.
 * @param x The x-coordinate where the block will start.
 * @param y The y-coordinate where the block will start.
 * @param props The properties of the MotD component, including attributes for
 * styling.
 */
static void insert_player_kb_block(WINDOW *window, int x, int y,
                                   MotdComponentProps props) {
  if (!window) return;

  int width = 30;
  int height = 15;

  WINDOW *wrapper = derwin(window, height, width, y, x);
  box(wrapper, 0, 0);
  wbkgd(wrapper, props.attrs);

  wattron(wrapper, WA_REVERSE);
  mvwprintw(wrapper, 1, 1, "%s", " Keyboard                   ");
  wattroff(wrapper, WA_REVERSE);

  mvwprintw(wrapper, 3, 4, "%s", "up");
  mvwprintw(wrapper, 4, 4, "%s", "down");
  mvwprintw(wrapper, 5, 4, "%s", "left");
  mvwprintw(wrapper, 6, 4, "%s", "right");
  mvwprintw(wrapper, 7, 4, "%s", "action");
  mvwprintw(wrapper, 8, 4, "%s", "pause");
  mvwprintw(wrapper, 9, 4, "%s", "terminate");

  wattron(wrapper, WA_DIM);
  mvwprintw(wrapper, 3, 16, "%s", "up-arrow");
  mvwprintw(wrapper, 4, 16, "%s", "down-arrow");
  mvwprintw(wrapper, 5, 16, "%s", "left-arrow");
  mvwprintw(wrapper, 6, 16, "%s", "right-arrow");
  mvwprintw(wrapper, 7, 16, "%s", "<space>");
  mvwprintw(wrapper, 8, 16, "%s", "esc");
  mvwprintw(wrapper, 9, 16, "%s", "q");
  wattroff(wrapper, WA_DIM);

  // custom
  wattron(wrapper, COLOR_PAIR(THEME_PRIMARY_PAIR));
  mvwprintw(wrapper, 11, 4, "%s", "new bricks");
  mvwprintw(wrapper, 12, 4, "%s", "add exp");
  wattroff(wrapper, COLOR_PAIR(THEME_PRIMARY_PAIR));

  wattron(wrapper, COLOR_PAIR(THEME_PRIMARY_PAIR) | WA_DIM);
  mvwprintw(wrapper, 11, 16, "%s", "<n>");
  mvwprintw(wrapper, 12, 16, "%s", "<x>");
  wattroff(wrapper, COLOR_PAIR(THEME_PRIMARY_PAIR) | WA_DIM);

  wnoutrefresh(window);
  delwin(wrapper);
  wrapper = NULL;
}

/**
 * @brief Displays an ASCII representation of a keyboard layout in a specified
 * window.
 *
 * This function renders an ASCII art representation of a keyboard layout within
 * a specified ncurses window at given x and y coordinates.
 *
 * @param window The ncurses window where the keyboard layout will be displayed.
 * @param x The x-coordinate where the keyboard layout will start.
 * @param y The y-coordinate where the keyboard layout will start.
 */
static void keyboard_ascii(WINDOW *window, int x, int y) {
  static const char keyboard[11][64] = {
      ",---,---,---,---,---,---,---,---,---,---,---,---,---,-------,",
      "|1/2| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 0 | + | ' | <-    |",
      "|---'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-----|",
      "| ->| | Q | W | E | R | T | Y | U | I | O | P | ] | ^ |     |",
      "|-----',--',--',--',--',--',--',--',--',--',--',--',--'|    |",
      "| Caps | A | S | D | F | G | H | J | K | L | \\ | [ | * |    |",
      "|----,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'-,-'---'----|",
      "|    | < | Z | X | C | V | B | N | M | , | . | - |          |",
      "|----'-,-',--'--,'---'---'---'---'---'---'-,-'---',--,------|",
      "| ctrl |  | alt |                          |altgr |  | ctrl |",
      "'------'  '-----'--------------------------'------'  '------'"};
  for (size_t i = 0; i < 11; i++) {
    mvwprintw(window, y + i, x, "%s", keyboard[i]);
  }
}

/**
 * @brief Displays a color palette in a specified window at a given position.
 *
 * This function renders a color palette in a specified ncurses window at
 * coordinates (x, y). It iterates through a predefined set of color pairs,
 * applying each color pair to a small square or character in the window to
 * visually represent the color. The color pairs are defined by the COLOR_PAIR
 * macro, with each pair having a unique identifier.
 *
 * @param window The ncurses window where the color palette will be displayed.
 * @param x The x-coordinate where the color palette will start.
 * @param y The y-coordinate where the color palette will start.
 */
static void collor_pallete(WINDOW *window, int x, int y) {
  // char pallete_pair_names[14][32] = {
  //     "**",
  //     "BACKGROUND",
  //     "SURFACE",
  //     "PRIMARY",
  //     "SECONDARY",

  //     "BRICK_LIGHT_BLUE",
  //     "BRICK_DARK_BLUE",
  //     "BRICK_ORANGE",
  //     "BRICK_YELLOW",
  //     "BRICK_GREEN",
  //     "BRICK_RED",
  //     "BRICK_MAGENTA",

  //     "WARNING",
  //     "SUCCESS"
  // };

  for (size_t i = 0; i < 14; i++) {
    int attrs = COLOR_PAIR(i + 1);
    wattron(window, attrs);
    // mvwprintw(window, (y + i) % 13, x, "%s", pallete_pair_names[i + 1]);
    mvwprintw(window, y, x + (i * 2), "%s", "  ");
    wattroff(window, attrs);
  }

  wnoutrefresh(window);
}

/**
 * @brief Renders a Message of the Day (MotD) component with various settings
 * options on a specified window.
 *
 * This function takes a window and the properties of a MotD component,
 * including its dimensions, position, and attributes. It creates a subwindow
 * within the specified window, sets its background color, and renders the MotD
 * component with a header, footer, and blocks for theme settings, brightness
 * adjustment, and player keyboard configuration. The footer includes a prompt
 * for the user to press Enter to continue, and the function also displays an
 * ASCII keyboard layout and a color palette.
 *
 * @param window The ncurses window where the MotD component will be rendered.
 * @param props The properties of the MotD component, including dimensions,
 * position, and attributes.
 */
void motd_component(WINDOW *window, MotdComponentProps props) {
  if (!window) return;
  (void)props;

  WINDOW *wrapper =
      derwin(window, props.height, props.width, props.pos.y, props.pos.x);
  box(wrapper, 0, 0);
  wbkgd(wrapper, props.attrs);

  const char header_text[] = "Settings";
  mvwprintw(wrapper, 1, (props.width - strlen(header_text)) / 2, "%s",
            header_text);

  insert_themes_block(wrapper, 2, 2, props);
  insert_brightness_block(wrapper, 2, 10, props);
  insert_player_kb_block(wrapper, 28, 2, props);

  const char footer_text[] = "PRESS <ENTER> TO CONTINUE";
  wattron(wrapper, WA_BLINK);
  mvwprintw(wrapper, props.height - 3, (props.width - strlen(footer_text)) / 2,
            "%s", footer_text);
  wattroff(wrapper, WA_BLINK);

  wattron(window, WA_DIM);
  keyboard_ascii(window, props.pos.x, props.pos.y + props.height);
  wattroff(window, WA_DIM);

  collor_pallete(window, (getmaxx(window) - 28) / 2, getmaxy(window) - 2);

  wnoutrefresh(window);
  delwin(wrapper);
  wrapper = NULL;
}
