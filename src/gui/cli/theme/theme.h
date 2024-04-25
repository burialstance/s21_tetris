#ifndef CLI_THEME_THEME_H
#define CLI_THEME_THEME_H

#include "../utils/utils.h"

/**
 * @brief Enumeration for theme selection in the application.
 *
 * This enum defines the available theme options for the application, including
 * DEFAULT_THEME, DARK_THEME, and LIGHT_THEME. These themes can be used to
 * change the appearance of the application's UI, such as colors and
 * backgrounds.
 */
enum PalleteThemeEnum { DEFAULT_THEME = 0, DARK_THEME, LIGHT_THEME };

/**
 * @brief Enumeration for color palette in the application.
 *
 * This enumeration defines various colors used throughout the application,
 * including background, surface, primary, secondary colors, and their
 * corresponding text colors on different backgrounds.
 *
 */
enum PalleteColorEnum {
  BACKGROUND_COLOR = 8,
  SURFACE_COLOR,
  PRIMARY_COLOR,
  SECONDARY_COLOR,
  ON_BACKGROUND_COLOR,
  ON_SURFACE_COLOR,
  ON_PRIMARY_COLOR,
  ON_SECONDARY_COLOR,

  BRICK_LIGHT_BLUE_COLOR,
  BRICK_DARK_BLUE_COLOR,
  BRICK_ORANGE_COLOR,
  BRICK_YELLOW_COLOR,
  BRICK_GREEN_COLOR,
  BRICK_RED_COLOR,
  BRICK_MAGENTA_COLOR,
};

/**
 * @brief Enumeration for color pairs used in the theme.
 *
 * This enum defines a set of color pairs that can be used to style various
 * elements of a user interface, such as backgrounds, surfaces, primary and
 * secondary colors, and specific theme colors.
 *
 */
enum PalleteColorPair {
  THEME_BACKGROUND_PAIR = 1,
  THEME_SURFACE_PAIR,
  THEME_PRIMARY_PAIR,
  THEME_SECONDARY_PAIR,

  THEME_BRICK_LIGHT_BLUE_PAIR,
  THEME_BRICK_DARK_BLUE_PAIR,
  THEME_BRICK_ORANGE_PAIR,
  THEME_BRICK_YELLOW_PAIR,
  THEME_BRICK_GREEN_PAIR,
  THEME_BRICK_RED_PAIR,
  THEME_BRICK_MAGENTA_PAIR,

  THEME_WARNING_PAIR,
  THEME_SUCCESS_PAIR
};

/**
 * @brief Defines the structure for a palette with various functionalities.
 *
 * This structure represents a palette that can manage colors, themes, and
 * brightness. It includes function pointers for operations such as getting a
 * color pair, changing the theme, adjusting brightness, and destroying the
 * palette.
 *
 * @field has_colors Indicates if the palette has color support.
 * @field theme The current theme of the palette.
 * @field get_brick_pair A function pointer for getting a color pair based on a
 * color index.
 * @field change_theme A function pointer for changing the theme of the palette.
 * @field brightness The current brightness level of the palette.
 * @field increase_brightness A function pointer for increasing the brightness.
 * @field decrease_brightness A function pointer for decreasing the brightness.
 * @field destroy A function pointer for destroying the palette.
 */
typedef struct __pallete {
  bool has_colors;
  enum PalleteThemeEnum theme;

  int (*get_brick_pair)(int color);
  void (*change_theme)(struct __pallete *self, enum PalleteThemeEnum theme);

  int brightness;
  void (*increase_brightness)(struct __pallete *self);
  void (*decrease_brightness)(struct __pallete *self);

  void (*destroy)(struct __pallete *self);
} Pallete;

/**
 * @brief Creates a new color palette.
 *
 * This function initializes a new color palette instance.
 *
 * @return A pointer to the newly created color palette instance.
 */
Pallete *new_palete();

/**
 * @brief Provides a palette for use in the application.
 *
 * This function is responsible for creating and returning a palette object.
 *
 * @return A pointer to the newly created palette object.
 */
Pallete *provide_pallete();

#endif  // !CLI_THEME_THEME_H
