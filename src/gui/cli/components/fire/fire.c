#include "fire.h"

/**
 * @brief Initializes color pairs for the fire component.
 *
 */
static void init_fire_component_colors() {
  init_pair(100, BRICK_RED_COLOR, SURFACE_COLOR);
  init_pair(101, BRICK_YELLOW_COLOR, SURFACE_COLOR);
  init_pair(102, BRICK_ORANGE_COLOR, SURFACE_COLOR);
  init_pair(103, ON_BACKGROUND_COLOR, SURFACE_COLOR);
}

/**
 * @brief Renders a fire component on a specified window using its properties.
 *
 * This function renders a dynamic fire effect within a specified window,
 * utilizing the properties defined in `FireComponentProps`. It dynamically
 * allocates a buffer to store the fire effect's intensity values and
 * initializes color pairs for rendering different shades of fire. The fire
 * effect is simulated by randomly setting intensity values in the buffer, which
 * are then used to determine the color of each cell in the fire component. The
 * intensity values are averaged across neighboring cells to simulate the spread
 * of fire. The function uses ncurses to create a subwindow for the fire
 * component, sets its background color, and renders the fire effect by printing
 * characters with varying colors based on the intensity values.
 *
 * @param window The ncurses window where the fire component will be rendered.
 * @param props The properties of the fire component, including dimensions,
 * position, and other relevant data.
 */
void fire_component(WINDOW *window, FireComponentProps props) {
  static int size = 0;
  static char *buffer = NULL;
  if (size != (props.width * props.height)) {
    size = props.width * props.height;
    if (buffer) free(buffer);
    buffer = (char *)calloc(size, sizeof(char));
  }

  static bool colors_inited = FALSE;
  if (!colors_inited) {
    init_fire_component_colors();
    colors_inited = TRUE;
  }

  WINDOW *wrapper = derwin(window, props.height, props.width, props.y, props.x);
  wbkgd(wrapper, COLOR_PAIR(THEME_BACKGROUND_PAIR));

  for (int i = 0; i < (props.width / 9); i++) {
    int index = (rand() % props.width) + props.width * (props.height - 1);
    buffer[index] = 65;
  }

  static char chars[] = " school21";
  for (int i = 0; i < size; i++) {
    buffer[i] = (char)(buffer[i] + buffer[i + 1] + buffer[i + props.width] +
                       buffer[i + props.width + 1]) /
                4;

    int color = 103;
    if (buffer[i] > 15)
      color = 102;
    else if (buffer[i] > 9)
      color = 101;
    else if (buffer[i] > 4)
      color = 100;

    if (i < size - 1) {
      int x = i % props.width;
      int y = i / props.width;
      wattron(wrapper, COLOR_PAIR(color) | WA_DIM);
      mvwaddch(wrapper, y, x, chars[buffer[i] > 9 ? 9 : buffer[i]]);
      wattroff(wrapper, COLOR_PAIR(color) | WA_DIM);
    }
  }

  wnoutrefresh(window);
  delwin(wrapper);
  wrapper = NULL;
}