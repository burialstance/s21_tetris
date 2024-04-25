#ifndef CLI_COMPONENTS_FIRE_FIRE_H
#define CLI_COMPONENTS_FIRE_FIRE_H

#include "../../theme/theme.h"
#include "ncurses.h"
#include "stdlib.h"
#include "time.h"

/**
 * @brief Defines the properties of a fire component, including its dimensions
 * and position.
 *
 * This structure encapsulates the essential properties for a fire component,
 * such as its width, height, and the x and y coordinates of its position within
 * a window.
 */
typedef struct {
  int width;
  int height;
  int x;
  int y;
} FireComponentProps;

/**
 * @brief Renders a fire component on a specified window using its properties.
 *
 * This function is designed to render a fire component within a specified
 * window
 *
 * @param window The ncurses window where the fire component will be rendered.
 * @param props The properties of the fire component, which could include
 * position, dimensions, attributes, and other relevant data for rendering the
 * fire component.
 */
void fire_component(WINDOW *window, FireComponentProps props);

#endif  // !CLI_COMPONENTS_FIRE_FIRE_H