#ifndef CLI_COMPONENTS_CLOAK_CLOAK_H
#define CLI_COMPONENTS_CLOAK_CLOAK_H

#include <ncurses.h>
#include <string.h>
#include <time.h>

/**
 * @brief Inserts a cloak component at a specified position within a parent
 * window.
 *
 * This function is designed to add a cloak component, which could be a visual
 * element or a game piece, to a specified position within a parent window. The
 * position is defined by x and y coordinates.
 *
 * @param parent The parent window where the cloak component will be inserted.
 * @param x The x-coordinate where the cloak component will be placed.
 * @param y The y-coordinate where the cloak component will be placed.
 */
void insert_cloak_component(WINDOW *parent, int x, int y);

#endif