#include "theme.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Increases the brightness of a color palette.
 *
 * This function increases the brightness of a given color palette by 20 units,
 * ensuring that the brightness does not exceed a maximum value. After adjusting
 * the brightness, it applies the new brightness level to the palette by calling
 * the change_theme method with the current theme.
 *
 * @param self Pointer to the Pallete instance whose brightness is to be
 * increased.
 */
static void _increase_brightness(Pallete *self) {
  if (self->brightness < 200) {
    self->brightness += 20;
    self->change_theme(self, self->theme);
  }
}

/**
 * @brief Decreases the brightness of a Pallete.
 *
 * This function reduces the brightness of a Pallete instance by 20 units,
 * provided that the current brightness is above -200. After adjusting the
 * brightness, it calls the change_theme method to apply the new brightness
 * setting to the theme.
 *
 * @param self Pointer to the Pallete instance whose brightness is to be
 * decreased.
 */
static void _decrease_brightness(Pallete *self) {
  if (self->brightness > -200) {
    self->brightness -= 20;
    self->change_theme(self, self->theme);
  }
}

static void _setup_brick_colors(int brightness) {
  // rgb(44, 216, 208)
  init_color(BRICK_LIGHT_BLUE_COLOR, 200 + (brightness / 3),
             600 + (brightness / 3), 750 + (brightness / 3));

  // rgb(5, 48, 112)
  init_color(BRICK_DARK_BLUE_COLOR, 26 + (brightness / 3),
             250 + (brightness / 3), 500 + (brightness / 3));

  // rgb(218, 137, 37)
  init_color(BRICK_ORANGE_COLOR, 750 + (brightness / 3), 350 + (brightness / 3),
             50 + (brightness / 3));

  // rgb(226, 207, 40)
  init_color(BRICK_YELLOW_COLOR, 880 + (brightness / 3), 810 + (brightness / 3),
             150 + (brightness / 3));
  // rgb(33, 162, 21)
  init_color(BRICK_GREEN_COLOR, 60 + (brightness / 3), 370 + (brightness / 3),
             40 + (brightness / 3));

  // rgb(182, 53, 53)
  init_color(BRICK_RED_COLOR, 715 + (brightness / 3), 205 + (brightness / 3),
             205 + (brightness / 3));

  // rgb(99, 11, 121)
  init_color(BRICK_MAGENTA_COLOR + (brightness / 3), 380 + (brightness / 3),
             43 + (brightness / 3), 470 + (brightness / 3));

  init_pair(THEME_BRICK_LIGHT_BLUE_PAIR, ON_SURFACE_COLOR,
            BRICK_LIGHT_BLUE_COLOR);
  init_pair(THEME_BRICK_DARK_BLUE_PAIR, ON_SURFACE_COLOR,
            BRICK_DARK_BLUE_COLOR);
  init_pair(THEME_BRICK_ORANGE_PAIR, ON_SURFACE_COLOR, BRICK_ORANGE_COLOR);
  init_pair(THEME_BRICK_YELLOW_PAIR, ON_SURFACE_COLOR, BRICK_YELLOW_COLOR);
  init_pair(THEME_BRICK_GREEN_PAIR, ON_SURFACE_COLOR, BRICK_GREEN_COLOR);
  init_pair(THEME_BRICK_RED_PAIR, ON_SURFACE_COLOR, BRICK_RED_COLOR);
  init_pair(THEME_BRICK_MAGENTA_PAIR, ON_SURFACE_COLOR, BRICK_MAGENTA_COLOR);
}

static int _get_brick_pair(int value) {
  int pair = THEME_SURFACE_PAIR;

  switch (value) {
    case 1:
      pair = THEME_BRICK_LIGHT_BLUE_PAIR;
      break;
    case 2:
      pair = THEME_BRICK_DARK_BLUE_PAIR;
      break;
    case 3:
      pair = THEME_BRICK_ORANGE_PAIR;
      break;
    case 4:
      pair = THEME_BRICK_YELLOW_PAIR;
      break;
    case 5:
      pair = THEME_BRICK_GREEN_PAIR;
      break;
    case 6:
      pair = THEME_BRICK_RED_PAIR;
      break;
    case 7:
      pair = THEME_BRICK_MAGENTA_PAIR;
      break;
    default:
      break;
  }

  return pair;
}

static void _setup_dark_theme(int brightness) {
  _setup_brick_colors(brightness);

  init_color(BACKGROUND_COLOR, 30 + brightness, 30 + brightness,
             45 + brightness);
  init_color(ON_BACKGROUND_COLOR, 700 + (brightness / 2),
             700 + (brightness / 2), 700 + (brightness / 2));

  init_color(SURFACE_COLOR, 60 + brightness, 60 + brightness, 75 + brightness);
  init_color(ON_SURFACE_COLOR, 300 + (brightness / 2), 770 + (brightness / 2),
             500 + (brightness / 2));

  init_color(PRIMARY_COLOR, 60 + brightness, 60 + brightness, 75 + brightness);
  init_color(ON_PRIMARY_COLOR, 900 + (brightness / 2), 700 + (brightness / 2),
             120 + (brightness / 2));

  init_color(SECONDARY_COLOR, 900 + brightness, 700 + brightness,
             120 + brightness);
  init_color(ON_SECONDARY_COLOR, 30 + (brightness / 2), 30 + (brightness / 2),
             45 + (brightness / 2));

  init_pair(THEME_BACKGROUND_PAIR, ON_BACKGROUND_COLOR, BACKGROUND_COLOR);
  init_pair(THEME_SURFACE_PAIR, ON_SURFACE_COLOR, SURFACE_COLOR);
  init_pair(THEME_PRIMARY_PAIR, ON_PRIMARY_COLOR, PRIMARY_COLOR);
  init_pair(THEME_SECONDARY_PAIR, ON_SECONDARY_COLOR, SECONDARY_COLOR);

  init_pair(THEME_WARNING_PAIR, COLOR_RED, BACKGROUND_COLOR);
  init_pair(THEME_SUCCESS_PAIR, COLOR_GREEN, BACKGROUND_COLOR);
}

static void _setup_light_theme(int brightness) {
  _setup_brick_colors(brightness);

  init_color(BACKGROUND_COLOR, 270 + brightness, 270 + brightness,
             270 + brightness);
  init_color(ON_BACKGROUND_COLOR, 700 + (brightness / 2),
             1000 + (brightness / 2), 490 + (brightness / 2));

  init_color(SURFACE_COLOR, 300 + brightness, 300 + brightness,
             300 + brightness);
  init_color(ON_SURFACE_COLOR, 90 + (brightness / 2), 90 + (brightness / 2),
             90 + (brightness / 2));

  init_color(PRIMARY_COLOR, 300 + brightness, 300 + brightness,
             300 + brightness);
  init_color(ON_PRIMARY_COLOR, 90 + (brightness / 2), 90 + (brightness / 2),
             90 + (brightness / 2));

  init_color(SECONDARY_COLOR, 900 + brightness, 900 + brightness,
             200 + brightness);
  init_color(ON_SECONDARY_COLOR, 200 + (brightness / 2), 300 + (brightness / 2),
             400 + (brightness / 2));

  init_pair(THEME_BACKGROUND_PAIR, ON_BACKGROUND_COLOR, BACKGROUND_COLOR);
  init_pair(THEME_SURFACE_PAIR, ON_SURFACE_COLOR, SURFACE_COLOR);
  init_pair(THEME_PRIMARY_PAIR, ON_PRIMARY_COLOR, PRIMARY_COLOR);
  init_pair(THEME_SECONDARY_PAIR, ON_SECONDARY_COLOR, SECONDARY_COLOR);

  init_pair(THEME_WARNING_PAIR, COLOR_RED, BACKGROUND_COLOR);
  init_pair(THEME_SUCCESS_PAIR, COLOR_GREEN, BACKGROUND_COLOR);
}

static void _setup_default_theme(int brightness) {
  _setup_brick_colors(brightness);

  init_color(BACKGROUND_COLOR, 0 + brightness, 0 + brightness, 0 + brightness);
  init_color(ON_BACKGROUND_COLOR, 1000 + (brightness / 2),
             1000 + (brightness / 2), 1000 + (brightness / 2));

  init_color(SURFACE_COLOR, 30 + brightness, 30 + brightness, 30 + brightness);
  init_color(ON_SURFACE_COLOR, 1000 + (brightness / 2), 1000 + (brightness / 2),
             1000 + (brightness / 2));

  init_color(PRIMARY_COLOR, 90 + brightness, 90 + brightness, 90 + brightness);
  init_color(ON_PRIMARY_COLOR, 1000 + (brightness / 2), 1000 + (brightness / 2),
             1000 + (brightness / 2));

  init_color(SECONDARY_COLOR, 150 + brightness, 150 + brightness,
             150 + brightness);
  init_color(ON_SECONDARY_COLOR, 1000 + (brightness / 2),
             1000 + (brightness / 2), 1000 + (brightness / 2));

  init_pair(THEME_BACKGROUND_PAIR, ON_BACKGROUND_COLOR, BACKGROUND_COLOR);
  init_pair(THEME_SURFACE_PAIR, ON_SURFACE_COLOR, SURFACE_COLOR);
  init_pair(THEME_PRIMARY_PAIR, ON_PRIMARY_COLOR, PRIMARY_COLOR);
  init_pair(THEME_SECONDARY_PAIR, ON_SECONDARY_COLOR, SECONDARY_COLOR);

  init_pair(THEME_WARNING_PAIR, COLOR_RED, BACKGROUND_COLOR);
  init_pair(THEME_SUCCESS_PAIR, COLOR_GREEN, BACKGROUND_COLOR);
}

/**
 * @brief Changes the theme of a Pallete instance.
 *
 * This function updates the theme of a Pallete instance based on the provided
 * theme enum value. It first checks if the Pallete has colors defined. If so,
 * it sets the theme of the Pallete and calls the appropriate setup function for
 * the chosen theme, passing the current brightness level to the setup function.
 *
 * @param self Pointer to the Pallete instance.
 * @param theme The theme enum value to apply to the Pallete.
 */
static void _change_theme(Pallete *self, enum PalleteThemeEnum theme) {
  if (self->has_colors) {
    switch (theme) {
      case DEFAULT_THEME:
        self->theme = theme;
        _setup_default_theme(self->brightness);
        break;

      case DARK_THEME:
        self->theme = theme;
        _setup_dark_theme(self->brightness);
        break;

      case LIGHT_THEME:
        self->theme = theme;
        _setup_light_theme(self->brightness);
        break;
    }
  }
}

/**
 * @brief Frees the memory allocated for a Pallete instance.
 *
 * This function safely frees the memory allocated for a Pallete object.
 *
 * @param self A pointer to the Pallete instance to be destroyed.
 */
static void _destroy(Pallete *self) {
  if (!self) return;
  free(self);
}

/**
 * @brief Initializes a new Pallete instance.
 *
 * This function creates a new Pallete object, allocates memory for it, and
 * initializes its properties and methods. It also checks if the terminal
 * supports color changes and initializes color support if possible. Exits the
 * program with an error message if memory allocation fails.
 * @return A pointer to the newly created Pallete instance.
 */
Pallete *new_palete() {
  Pallete *self = (Pallete *)malloc(sizeof(Pallete));
  if (!self) {
    fprintf(stderr, "Cannot allocate mem for Pallete\n");
    exit(-1);
  }

  self->change_theme = _change_theme;
  self->destroy = _destroy;
  self->get_brick_pair = _get_brick_pair;

  self->brightness = 0;  // [-200, +200]
  self->increase_brightness = _increase_brightness;
  self->decrease_brightness = _decrease_brightness;

  self->has_colors = has_colors();

  if (self->has_colors) {
    if (can_change_color()) {
      start_color();
    } else {
      fprintf(stderr, "Terminal not supported change the colors\n");
    }
  } else {
    fprintf(stderr, "Terminal not supported colors\n");
  }

  return self;
}

/**
 * @brief Provides a singleton instance of Pallete.
 *
 * This function ensures that only one instance of Pallete is created and
 * returned. It uses a static variable to hold the instance, which is
 * initialized the first time the function is called. Subsequent calls return
 * the same instance, ensuring that only one Pallete object exists throughout
 * the application.
 *
 * @return A pointer to the singleton Pallete instance.
 */
Pallete *provide_pallete() {
  static Pallete *pallete = NULL;
  if (!pallete) {
    pallete = new_palete();
  }
  return pallete;
}
