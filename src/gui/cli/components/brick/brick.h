#ifndef CLI_COMPONENTS_BRICK_BRICK_H
#define CLI_COMPONENTS_BRICK_BRICK_H

#include <ncurses.h>
#include <string.h>

#include "../../theme/theme.h"

#define BRICK_COMPONENT_HEIGHT 4
#define BRICK_COMPONENT_WIDTH 4

/**
 * @brief Defines the data structure for a brick component, including its title,
 * matrix, and dimensions.
 *
 * This structure encapsulates the essential information for a brick component,
 * such as its title, a dynamically allocated 2D matrix representing its
 * content, and the dimensions (width and height) of the matrix. The matrix is
 * dynamically allocated to accommodate varying sizes.
 */
typedef struct {
  char title[24];
  int **matrix;
  size_t width;
  size_t height;
} BrickComponentData;

/**
 * @brief Defines the position of a brick component in a 2D space.
 *
 * This structure represents the x and y coordinates of a brick component within
 * a grid or board. It is used to specify the location of a brick component,
 * which can be crucial for rendering.
 */
typedef struct {
  int x;
  int y;
} BrickComponentPosition;

/**
 * @brief Enumeration for brick component rendering types.
 *
 * This enumeration defines the types of rendering available for brick
 * components within a UI library. It includes a default rendering type and a
 * rendering type specifically for displaying digits.
 *
 */
enum BrickComponentRenderTypeEnum {
  BRICK_RENDER_TYPE_DEFAULT = 0,
  BRICK_RENDER_TYPE_COLORLESS,
};

/**
 * @brief Defines the properties of a brick component for a Tetris game.
 *
 * This structure encapsulates the data, position, dimensions, rendering type,
 * and attributes of a brick component. It is used to configure and manage the
 * visual and functional aspects of bricks in the Tetris game, including their
 * shape, color, size, and how they are rendered on the game board.
 *
 * @struct BrickComponentProps
 * @var BrickComponentData data The data associated with the brick component,
 * likely including its shape.
 * @var BrickComponentPosition pos The position of the brick component on the
 * game board.
 * @var size_t width The width of the brick component.
 * @var size_t height The height of the brick component.
 * @var enum BrickComponentRenderTypeEnum render_type The type of rendering for
 * the brick component.
 * @var int attrs Additional attributes or flags for subwin component.
 */
typedef struct {
  BrickComponentData data;
  BrickComponentPosition pos;
  size_t width;
  size_t height;
  enum BrickComponentRenderTypeEnum render_type;
  int attrs;
} BrickComponentProps;

/**
 * @brief Renders a brick component on a specified window using its properties.
 *
 * It takes a window and the properties of a brick component, and renders the
 * brick accordingly. The properties of the brick component, encapsulated in
 * `BrickComponentProps`, likely include information such as the brick's shape,
 * color, and position on the game board.
 *
 * @param window The ncurses window where the brick component will be rendered.
 * @param props The properties of the brick component, including shape, color,
 * and position.
 */
void brick_component(WINDOW *window, BrickComponentProps props);

#endif  // !CLI_COMPONENTS_BRICK_BRICK_H
