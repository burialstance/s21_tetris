#ifndef BRICKGAME_TETRIS_BRICKS_BRICKS_H
#define BRICKGAME_TETRIS_BRICKS_BRICKS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BRICK_HEIGHT 4
#define BRICK_WIDTH 4

/**
 * @brief Enumeration representing the colors of Tetris bricks.
 *
 * This enumeration defines the various colors that a Tetris brick can have,
 * such as light blue, dark blue, orange, yellow, green, red, and magenta.
 * Each color corresponds to a specific type of brick in the Tetris game.
 *
 */
enum BrickColor {
  BrickLightBlueColor = 1,
  BrickDarkBlueColor,
  BrickOrangeColor,
  BrickYellowColor,
  BrickGreenColor,
  BrickRedColor,
  BrickMagentaColor,
};

/**
 * @brief Structure representing the position of a brick in the Tetris game.
 *
 * This structure holds the x and y coordinates of a brick's position on the
 * game field. These coordinates are used to determine the brick's location
 * within the game grid.
 *
 * @struct BrickPosition
 * @var x The x-coordinate of the brick's position.
 * @var y The y-coordinate of the brick's position.
 */
typedef struct {
  int x;
  int y;
} BrickPosition;

/**
 * @brief Structure representing a Tetris brick (piece).
 *
 * This structure encapsulates the properties and behavior of a Tetris brick,
 * including its position, color, state, and the functions to transition between
 * states.
 *
 * @struct __brick
 * @var pos The position of the brick on the game field.
 * @var color The color of the brick.
 * @var state The current state of the brick, which determines its shape.
 * @var states A 3D array representing the possible states of the brick.
 * @var total_states The total number of states available for the brick.
 * @var next_state A function pointer for transitioning to the next state of the
 * brick.
 * @var prev_state A function pointer for transitioning to the previous state of
 * the brick.
 */
typedef struct __brick {
  BrickPosition pos;
  int color;

  int state;
  int states[4][BRICK_HEIGHT][BRICK_WIDTH];
  int total_states;
  void (*next_state)(struct __brick *self);
  void (*prev_state)(struct __brick *self);
} Brick;

/**
 * @brief Structure representing the repository for Tetris bricks (pieces).
 *
 * This structure manages the collection of Tetris bricks (pieces), including
 * their storage, retrieval, and manipulation. It provides functionality for
 * accessing bricks by index, retrieving a random brick, creating new bricks,
 * populating the repository with custom bricks, and destroying the repository.
 *
 * @struct TetrisBrickRepository
 * @var items An array of Brick structures representing the bricks in the
 * repository.
 * @var items_count The number of bricks currently stored in the repository.
 * @var get A function pointer for retrieving a brick by its index.
 * @var get_random A function pointer for retrieving a random brick.
 * @var create A function pointer for creating a new brick and adding it to the
 * repository.
 * @var populate_custom A function pointer for populating the repository with
 * custom bricks.
 * @var destroy A function pointer for destroying the repository and freeing its
 * resources.
 */
typedef struct __brick_repository {
  Brick *items;
  size_t items_count;

  Brick *(*get)(struct __brick_repository *self, int index);
  Brick *(*get_random)(struct __brick_repository *self);
  void (*create)(struct __brick_repository *self, Brick brick);

  void (*populate_custom)(struct __brick_repository *self);

  void (*destroy)(struct __brick_repository *self);
} TetrisBrickRepository;

/**
 * @brief Creates a new Tetris brick repository.
 *
 * This function initializes a new Tetris brick repository, which is used to
 * manage the different types of bricks (pieces) available in the game. The
 * brick repository is essential for the game's functionality, as it stores the
 * data for each type of brick, including its shape, color, and any special
 * properties.
 *
 * @return A pointer to the newly created TetrisBrickRepository instance.
 */
TetrisBrickRepository *new_brick_repository();

/**
 * @brief Provides a singleton instance of the Tetris brick repository.
 *
 * This function ensures that only one instance of the Tetris brick repository
 * is created and used throughout the application. It uses a static variable to
 * store the instance, and if the instance does not exist, it creates a new one.
 *
 * @return A pointer to the singleton Tetris brick repository instance.
 */
TetrisBrickRepository *provide_brick_repository();

#endif  // !BRICKGAME_TETRIS_BRICKS_BRICKS_H
