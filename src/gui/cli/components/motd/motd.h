#ifndef CLI_COMPONENTS_MOTD_MOTD_H
#define CLI_COMPONENTS_MOTD_MOTD_H

#include <ncurses.h>
#include <string.h>

#include "../../theme/theme.h"

/**
 * @brief Defines the position of a Message of the Day (MotD) component in a 2D
 * space.
 *
 * This structure represents the x and y coordinates of a Message of the Day
 * (MotD) component.
 */
typedef struct {
  int x;
  int y;
} MotdComponentPosition;

/**
 * @brief Defines the properties of a Message of the Day (MotD) component,
 * including its dimensions, position, and attributes.
 *
 * This structure encapsulates the essential properties for a MotD component,
 * such as its dimensions (height and width), position, and attributes for
 * rendering.
 */
typedef struct {
  size_t height;
  size_t width;
  MotdComponentPosition pos;
  int attrs;
} MotdComponentProps;

/**
 * @brief Renders a Message of the Day (MotD) component on a specified window
 * using its properties.
 *
 * This function takes a window and the properties of a MotD component.
 * It creates a subwindow within the specified window, sets its background
 * color, and renders the MotD component. The MotD component typically displays
 * a message or announcement, which can be used for various purposes such as
 * providing information, instructions, or updates to the user.
 *
 * @param window The ncurses window where the MotD component will be rendered.
 * @param props The properties of the MotD component, including dimensions,
 * position, and attributes.
 */
void motd_component(WINDOW *window, MotdComponentProps props);

#endif  // !CLI_COMPONENTS_MOTD_MOTD_H