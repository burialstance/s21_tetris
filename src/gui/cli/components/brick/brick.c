#include "brick.h"

/**
 * @brief Renders a brick component on a specified window using its properties.
 *
 * This function takes a window and the properties of a brick component,
 * including its position, data (including title and matrix), attributes, and
 * rendering type. It creates a subwindow within the specified window, sets its
 * background color, and renders the brick component based on the provided
 * rendering type. The rendering can be either default or digits, with the
 * latter displaying the matrix values as digits.
 *
 * @param window The ncurses window where the brick component will be rendered.
 * @param props The properties of the brick component, including position, data,
 * attributes, and rendering type.
 */
void brick_component(WINDOW *window, BrickComponentProps props) {
  if (!window) return;

  WINDOW *wrapper =
      derwin(window, props.height, props.width, props.pos.y, props.pos.x);
  box(wrapper, 0, 0);
  wbkgd(wrapper, props.attrs);

  wattron(wrapper, WA_REVERSE);
  for (size_t i = 0; i < props.width; i++) mvwprintw(wrapper, 0, i, "%c", ' ');
  mvwprintw(wrapper, 0, (props.width - strlen(props.data.title)) / 2, "%s",
            props.data.title);
  wattroff(wrapper, WA_REVERSE);
  Pallete *pallete = provide_pallete();

  if (props.render_type == BRICK_RENDER_TYPE_DEFAULT) {
    for (size_t row = 0; (row < props.data.height) && props.data.matrix;
         row++) {
      for (size_t col = 0; (col < props.data.width) && props.data.matrix;
           col++) {
        wattron(wrapper, props.data.matrix[row][col]
                             ? COLOR_PAIR(pallete->get_brick_pair(
                                   props.data.matrix[row][col]))
                             : WA_DIM);
        mvwprintw(wrapper, row + 1, (col * 2) + 2, "%s",
                  props.data.matrix[row][col] ? "  " : "");
        wattroff(wrapper, WA_DIM);
      }
    }

  } else if (props.render_type == BRICK_RENDER_TYPE_COLORLESS) {
    // for (size_t row = 0; (row < props.data.height) && props.data.matrix;
    //      row++) {
    //   for (size_t col = 0; (col < props.data.width) && props.data.matrix;
    //        col++) {
    //     if (!props.data.matrix[row][col]) {
    //       wattron(wrapper, WA_DIM);
    //     }
    //     mvwprintw(wrapper, row + 1, (col * 2) + 2, "%d ",
    //               props.data.matrix[row][col]);
    //     wattroff(wrapper, WA_DIM);
    //   }
    // }
    for (size_t row = 0; row < props.data.height; row++) {
      for (size_t col = 0; col < props.data.width; col++) {
        int brick_attr = props.data.matrix[row][col]
                             ? COLOR_PAIR(pallete->get_brick_pair(
                                   props.data.matrix[row][col])) |
                                   WA_REVERSE
                             : WA_DIM;
        wattron(wrapper, brick_attr);
        mvwprintw(wrapper, row + 1, (col * 2) + 2,
                  props.data.matrix[row][col] ? "  " : "");
        wattroff(wrapper, brick_attr);
      }
    }
  }

  wnoutrefresh(window);
  delwin(wrapper);
  wrapper = NULL;
}