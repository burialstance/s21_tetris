#include "counter.h"

/**
 * @brief Renders a counter component on a specified window using its
 * properties.
 *
 * This function takes a window and the properties of a counter component,
 * including its position, data (including title and value), attributes, and
 * dimensions. It creates a subwindow within the specified window, sets its
 * background color, and renders the counter component. The counter component
 * includes a header displaying its title and a value area displaying the
 * current value of the counter. The value is formatted as an integer and
 * displayed in bold.
 *
 * @param window The ncurses window where the counter component will be
 * rendered.
 * @param props The properties of the counter component, including position,
 * data, attributes, and dimensions.
 */
void counter_component(WINDOW *window, CounterComponentProps props) {
  if (!window) return;

  WINDOW *wrapper =
      derwin(window, props.height, props.width, props.pos.y, props.pos.x);
  wbkgd(wrapper, props.attrs);
  box(wrapper, 0, 0);

  int header_attrs = WA_REVERSE;
  wattron(wrapper, header_attrs);
  for (size_t i = 0; i < props.width; i++) mvwprintw(wrapper, 0, i, "%c", ' ');
  mvwprintw(wrapper, 0, (props.width - strlen(props.data.title)) / 2, "%s",
            props.data.title);
  wattroff(wrapper, header_attrs);

  char value_str[props.width];
  snprintf(value_str, props.width, "%d", props.data.value);

  int value_attrs = WA_BOLD;
  wattron(wrapper, value_attrs);
  mvwprintw(wrapper, 1, (props.width - strlen(value_str)) / 2, "%d",
            props.data.value);
  wattroff(wrapper, value_attrs);

  wnoutrefresh(window);  // TODO: OR WRAPPER??
  delwin(wrapper);
  wrapper = NULL;
}
