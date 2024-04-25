#ifndef CLI_LAYOUTS_LAYOUTS_H
#define CLI_LAYOUTS_LAYOUTS_H

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "../theme/theme.h"

/**
 * @brief Configuration structure for padding in a layout.
 *
 * This structure defines the padding for a layout, specifying the amount of
 * space to be left on each side (left, right, top, bottom) of the layout. This
 * is useful for creating margins or spacing between elements within a layout.
 *
 * @struct LayoutConfigPadding
 * @var int left
 *      The amount of space to be left on the left side of the layout.
 * @var int right
 *      The amount of space to be left on the right side of the layout.
 * @var int top
 *      The amount of space to be left on the top side of the layout.
 * @var int bottom
 *      The amount of space to be left on the bottom side of the layout.
 */
typedef struct {
  int left;
  int right;
  int top;
  int bottom;
} LayoutConfigPadding;

/**
 * @brief Configuration structure for a layout.
 *
 * This structure defines the configuration for a layout, including dimensions,
 * minimum dimensions, adjustment behavior on update, and padding. The
 * dimensions specify the current size of the layout, while the minimum
 * dimensions ensure that the layout does not shrink below these values. The
 * adjust_on_update flag indicates whether the layout should be automatically
 * adjusted when updated. The padding structure specifies the amount of space to
 * be left on each side of the layout.
 *
 * @struct LayoutConfig
 * @var int height
 *      The current height of the layout.
 * @var int width
 *      The current width of the layout.
 * @var int min_width
 *      The minimum width the layout can shrink to.
 * @var int min_height
 *      The minimum height the layout can shrink to.
 * @var bool adjust_on_update
 *      Flag indicating whether the layout should be automatically adjusted when
 * updated.
 * @var LayoutConfigPadding padding
 *      Configuration for padding around the layout.
 */
typedef struct {
  int height;
  int width;

  int min_width;
  int min_height;

  bool adjust_on_update;
  LayoutConfigPadding padding;
} LayoutConfig;

/**
 * @brief Structure representing a layout.
 *
 * This structure encapsulates a layout, which is a fundamental building block
 * for creating user interfaces in text-based applications. It includes a
 * pointer to a WINDOW structure, which represents the actual window or area on
 * the screen where the layout is drawn. The structure also contains a
 * LayoutConfig instance that defines the configuration for the layout,
 * including its dimensions, minimum dimensions, adjustment behavior on update,
 * and padding. Additionally, it includes function pointers for drawing,
 * updating, adjusting the window size, and destroying the layout. These
 * function pointers allow for dynamic behavior and resource management.
 *
 * @struct __layout
 * @var WINDOW *window
 *      Pointer to the WINDOW structure representing the layout's window.
 * @var LayoutConfig config
 *      Configuration for the layout, including dimensions, minimum dimensions,
 * adjustment behavior, and padding.
 * @var void (*draw)(struct __layout *self)
 *      Function pointer for drawing the layout.
 * @var void (*update)(struct __layout *self)
 *      Function pointer for updating the layout.
 * @var void (*adjust_window)(struct __layout *self)
 *      Function pointer for adjusting the window size of the layout.
 * @var void (*destroy)(struct __layout *self)
 *      Function pointer for destroying the layout.
 */
typedef struct __layout {
  WINDOW *window;
  LayoutConfig config;

  void (*draw)(struct __layout *self);
  void (*update)(struct __layout *self);
  void (*adjust_window)(struct __layout *self);
  void (*destroy)(struct __layout *self);
} Layout;

/**
 * @brief Creates a new layout with a specified parent window and configuration.
 *
 * This function initializes a new layout with a given parent window and
 * configuration. The parent window is used as the reference for the layout's
 * position and size. The configuration specifies the layout's dimensions,
 * minimum dimensions, adjustment behavior on update, and padding. The function
 * returns a pointer to the newly created Layout structure.
 *
 * @param parent Pointer to the parent WINDOW structure.
 * @param config LayoutConfig structure containing the layout's configuration.
 * @return Pointer to the newly created Layout structure.
 */
Layout *new_layout(WINDOW *parent, LayoutConfig config);

#endif  // !CLI_LAYOUTS_LAYOUTS_H
