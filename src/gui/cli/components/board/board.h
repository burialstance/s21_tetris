#ifndef CLI_COMPONENTS_BOARD_BOARD_H
#define CLI_COMPONENTS_BOARD_BOARD_H

#include <ncurses.h>

#include "../../theme/theme.h"

#define BOARD_COMPONENT_HEIGHT 20
#define BOARD_COMPONENT_WIDTH 10

/**
 * @brief Defines the data structure for a board component.
 *
 * This structure contains a 2D matrix, represented as a double pointer, which
 * is used to store the data associated with a board component.
 *
 * @struct BoardComponentData
 * @var **matrix A double pointer to an integer matrix, representing the data of
 * the board component.
 */
typedef struct {
  int **matrix;
} BoardComponentData;

/**
 * @brief Defines the position of a board component.
 *
 * This structure represents the x and y coordinates of a board component's
 * position.
 *
 * @struct BoardComponentPosition
 * @var x The x-coordinate of the board component's position.
 * @var y The y-coordinate of the board component's position.
 */
typedef struct {
  int x;
  int y;
} BoardComponentPosition;

/**
 * @brief Enumeration for board component rendering types.
 *
 * This enumeration defines the types of rendering that can be applied to board
 * components. It includes a default rendering type and a rendering type for
 * displaying digits.
 *
 */
enum BoardRenderTypeEnum {
  BOARD_RENDER_TYPE_DEFAULT = 0,
  BOARD_RENDER_TYPE_COLORLESS,
};

/**
 * @brief Defines the properties of a board component for a UI library using
 * ncurses.
 *
 * This structure encapsulates the data, position, rendering type, and
 * attributes of a board component, designed for use within a terminal-based UI
 * library that leverages the ncurses library for rendering.
 */
typedef struct {
  BoardComponentData data;
  BoardComponentPosition pos;
  enum BoardRenderTypeEnum render_type;
  int attrs;
} BoardComponentProps;

/**
 * @brief Renders a board component on a specified window using its properties.
 *
 * This function takes a window and the properties of a board component.
 *
 * @param window The ncurses window where the board component will be rendered.
 * @param props The properties of the board component, including data, position,
 * rendering type, and attributes.
 */
void board_component(WINDOW *window, BoardComponentProps props);

#endif  // !CLI_COMPONENTS_BOARD_BOARD_H