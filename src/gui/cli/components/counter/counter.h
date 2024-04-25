#ifndef CLI_COMPONENTS_COUNTER_COUNTER_H
#define CLI_COMPONENTS_COUNTER_COUNTER_H

#include <ncurses.h>
#include <string.h>

/**
 * @brief Defines the data structure for a counter component, including its
 * value and title.
 *
 * This structure encapsulates the essential information for a counter
 * component, such as its value and a title or label.
 */
typedef struct {
  int value;
  char title[24];
} CounterComponentData;

/**
 * @brief Defines the position of a counter component in a 2D space.
 *
 * This structure represents the x and y coordinates of a counter component
 * within a grid or board. It is used to specify the location of a counter
 * component, which can be crucial for rendering and positioning in a game or
 * application that involves grid-based layouts.
 */
typedef struct {
  int x;
  int y;
} CounterComponentPosition;

/**
 * @brief Defines the properties of a counter component, including its data,
 * position, dimensions, and attributes.
 *
 * This structure encapsulates the essential properties for a counter component,
 * such as its data (including value and title), position, dimensions (width and
 * height), and attributes for rendering. It is used to configure and manage the
 * visual and functional aspects of counter components in applications, allowing
 * for the display and manipulation of numerical values with associated labels.
 */
typedef struct {
  CounterComponentData data;
  CounterComponentPosition pos;
  size_t width;
  size_t height;
  int attrs;
} CounterComponentProps;

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
void counter_component(WINDOW *window, CounterComponentProps props);

#endif  // !CLI_COMPONENTS_COUNTER_COUNTER_H