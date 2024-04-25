#include "board.h"

/**
 * @brief Renders a board component on a specified window using its properties.
 *
 * This function takes a window and the properties of a board component,
 * including data, position, rendering type, and attributes, and renders the
 * component accordingly. It creates a subwindow within the given window, sets
 * its background color, and then renders the board component based on the
 * specified render type.
 *
 * @param window The ncurses window where the board component will be rendered.
 * @param props The properties of the board component, including data, position,
 * rendering type, and attributes.
 */
void board_component(WINDOW *window, BoardComponentProps props) {
  if (!window) return;

  WINDOW *wrapper =
      derwin(window, BOARD_COMPONENT_HEIGHT + 2,
             (BOARD_COMPONENT_WIDTH * 2) + 2, props.pos.y, props.pos.x);
  box(wrapper, 0, 0);
  wbkgd(wrapper, props.attrs);

  Pallete *pallete = provide_pallete();

  if (props.render_type == BOARD_RENDER_TYPE_DEFAULT) {
    for (size_t row = 0; (row < BOARD_COMPONENT_HEIGHT) && props.data.matrix;
         row++) {
      for (size_t col = 0; (col < BOARD_COMPONENT_WIDTH) && props.data.matrix;
           col++) {
        int brick_attr = props.data.matrix[row][col]
                             ? COLOR_PAIR(pallete->get_brick_pair(
                                   props.data.matrix[row][col]))
                             : WA_DIM;
        wattron(wrapper, brick_attr);
        mvwprintw(wrapper, row + 1, (col * 2) + 1, "%s",
                  props.data.matrix[row][col] ? "  " : "");
        wattroff(wrapper, brick_attr);
      }
    }

  } else if (props.render_type == BOARD_RENDER_TYPE_COLORLESS) {
    for (size_t row = 0; row < BOARD_COMPONENT_HEIGHT; row++) {
      for (size_t col = 0; col < BOARD_COMPONENT_WIDTH; col++) {
        int brick_attr = props.data.matrix[row][col]
                             ? COLOR_PAIR(pallete->get_brick_pair(
                                   props.data.matrix[row][col])) |
                                   WA_REVERSE
                             : WA_DIM;
        wattron(wrapper, brick_attr);
        mvwprintw(wrapper, row + 1, (col * 2) + 1,
                  props.data.matrix[row][col] ? "  " : "");
        wattroff(wrapper, brick_attr);
      }
    }
  }

  wnoutrefresh(wrapper);
  delwin(wrapper);
  wrapper = NULL;
}
