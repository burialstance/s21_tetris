#include "tetris.h"

/**
 * @brief Clears the Tetris game field.
 *
 * This static function resets the game field to its initial state by setting
 * all cells to 0. It is typically used to clear the game field at the start of
 * a new game or when the game is reset.
 *
 * @param tetris A pointer to the Tetris game engine instance.
 */
static void clear_tetris_fields(Tetris *tetris) {
  if (!tetris) return;

  for (int row = 0; row < TETRIS_FIELD_HEIGHT; row++) {
    for (int col = 0; col < TETRIS_FIELD_WIDTH; col++) {
      tetris->data.info.field[row][col] = 0;
    }
  }
}

/**
 * @brief Removes a brick from the game matrix.
 *
 * This function removes a specified brick from the game matrix, effectively
 * clearing its presence from the game field. It iterates over the brick's state
 * matrix to determine which cells to clear. The function returns a boolean
 * value indicating the success of the operation.
 *
 * @param matrix A 2D array representing the game field.
 * @param brick A pointer to the Brick structure representing the brick to be
 * removed.
 * @return Always true. why? idk, becs i can)
 */
static bool remove_brick(int **matrix, Brick *brick) {
  if (!matrix || !brick) return false;

  bool is_success = true;
  int x = brick->pos.x;
  int y = brick->pos.y;

  for (int row = 0; row < BRICK_HEIGHT; row++) {
    for (int col = 0; col < BRICK_WIDTH; col++) {
      if (brick->states[brick->state][row][col]) {
        int brick_in_matrix_pos_x = x + (-BRICK_WIDTH / 2) + col;
        int brick_in_matrix_pos_y = y + (-BRICK_HEIGHT / 2) + 1 + row;
        matrix[brick_in_matrix_pos_y][brick_in_matrix_pos_x] = 0;
      }
    }
  }
  return is_success;
}

/**
 * @brief Checks for collisions between a brick and the game field.
 *
 * This function checks if a given brick collides with the game field boundaries
 * or with other bricks already placed on the field. It iterates over the cells
 * of the brick's current state, calculating their positions on the game field
 * matrix. If any of these positions are outside the game field boundaries or if
 * they correspond to a cell already occupied by another brick, the function
 * returns true, indicating a collision. Otherwise, it returns false.
 *
 * @param matrix A 2D array representing the game field.
 * @param brick A pointer to the Brick structure representing the brick to check
 * for collisions.
 * @return true if a collision is detected, false otherwise.
 */
static bool is_collide(int **matrix, Brick *brick) {
  if (!matrix || !brick) return false;

  bool result = false;

  int x = brick->pos.x;
  int y = brick->pos.y;
  for (int row = 0; row < BRICK_HEIGHT && !result; row++) {
    for (int col = 0; col < BRICK_WIDTH && !result; col++) {
      if (brick->states[brick->state][row][col]) {
        int matrix_pos_x = x + (-BRICK_WIDTH / 2) + col;
        int matrix_pos_y = y + ((-BRICK_HEIGHT / 2) + 1) + row;

        if ((matrix_pos_x < 0) || (matrix_pos_x >= TETRIS_FIELD_WIDTH)) {
          result = true;  // width excided
        } else if ((matrix_pos_y < 0) ||
                   (matrix_pos_y >= TETRIS_FIELD_HEIGHT)) {
          result = true;  // height excided
        } else {
          result = matrix[matrix_pos_y][matrix_pos_x] ? true
                                                      : false;  // cell is busy
        }
      }
    }
  }
  return result;
}

/**
 * @brief Places a brick on the game matrix at its current position.
 *
 * This function places a brick on the game matrix at its current position,
 * updating the matrix with the brick's color. The position and state of the
 * brick are determined by its properties. The function checks if the matrix and
 * brick pointers are not null before proceeding.
 *
 * @param matrix A 2D array representing the game matrix where the brick will be
 * placed.
 * @param brick A pointer to the Brick structure representing the brick to be
 * placed.
 */
static void place_brick(int **matrix, Brick *brick) {
  if (!matrix || !brick) return;

  int x = brick->pos.x;
  int y = brick->pos.y;
  for (int row = 0; row < BRICK_HEIGHT; row++) {
    for (int col = 0; col < BRICK_WIDTH; col++) {
      if (brick->states[brick->state][row][col]) {
        int brick_in_matrix_pos_x = x + (-BRICK_WIDTH / 2) + col;
        int brick_in_matrix_pos_y = y + ((-BRICK_HEIGHT / 2) + 1) + row;
        matrix[brick_in_matrix_pos_y][brick_in_matrix_pos_x] = brick->color;
      }
    }
  }
}

/**
 * @brief Shifts the contents of a 2D array down, filling the top row with
 * zeros.
 *
 * This function is used to shift the contents of a 2D array representing the
 * Tetris game field downwards, effectively removing the top row of the array.
 * This is typically used after a row of the game field has been completely
 * filled with blocks, indicating that the row should be cleared and the blocks
 * above it should move down to fill the gap. The function starts shifting from
 * the specified row downwards, moving each element to the row above it, and
 * then sets all elements in the top row to zero.
 *
 * @param matrix A 2D array representing the game field.
 * @param row The row from which to start shifting downwards.
 */
static void shift_ereased(int **matrix, int row) {
  if (!matrix) return;
  for (; row >= 1; row--) {
    for (int col = 0; col < TETRIS_FIELD_WIDTH; col++) {
      matrix[row][col] = matrix[row - 1][col];
    }
  }
  for (int col = 0; col < TETRIS_FIELD_WIDTH; col++) matrix[0][col] = 0;
}

/**
 * @brief Erases fullfiled lines in the Tetris game field.
 *
 * This function scans the Tetris game field from bottom to top, checking each
 * row to see if it is fully populated. If a row is fully populated, it is
 * considered a completed line and is removed from the game field. The function
 * keeps track of the number of lines erased and returns this count. The removal
 * of a line is achieved by shifting all lines above the erased line down by one
 * row.
 *
 * @param matrix A 2D array representing the Tetris game field.
 * @return The number of lines erased from the game field.
 */
static int erase_lines(int **matrix) {
  if (!matrix) return 0;

  size_t erase_count = 0;
  for (int row = TETRIS_FIELD_HEIGHT - 1; row >= 0; row--) {
    int populated = 0;
    for (int col = 0; col < TETRIS_FIELD_WIDTH; col++)
      if (matrix[row][col]) populated++;

    if (populated == TETRIS_FIELD_WIDTH) {
      erase_count++;
      shift_ereased(matrix, row);
      row++;
    };
  }
  return erase_count;
}

/**
 * @brief Initializes the Tetris game engine on startup.
 *
 * This static function is called during the initialization of the Tetris game
 * engine. It reads the high score from a file and updates the game's high score
 * accordingly.
 *
 * @param self A pointer to the Tetris game engine instance.
 */
static void _on_startup(Tetris *self) {
  if (!self) return;
  self->data.info.high_score = read_highscore_from_file("highscore.txt");
}

/**
 * @brief Handles the shutdown process of the Tetris game engine.
 *
 * This static function is called during the shutdown process of the Tetris game
 * engine.
 *
 * @param self A pointer to the Tetris game engine instance.
 */
static void _on_shutdown(Tetris *self) {
  if (!self) return;
  write_highscore_to_file("highscore.txt", self->data.info.high_score);
}

/**
 * @brief Placeholder function for moving.
 *
 * This function is a placeholder for the functionality of moving the current
 * piece up in the Tetris game. It is currently not implemented and does not
 * perform any actions. The `hold` parameter is also not used.
 *
 * @param self A pointer to the Tetris game engine instance.
 * @param hold A boolean value indicating whether the action should be held.
 * This parameter is not used.
 */
static void _up(Tetris *self, bool hold) {
  if (!self) return;
  (void)hold;
}

/**
 * @brief Moves the current brick down in the Tetris game.
 *
 * This function is responsible for moving the current active piece (brick) down
 * in the game field. It checks for collisions with the bottom of the field or
 * other bricks. If the `hold` parameter is false, the brick is moved down by
 * one unit, and if a collision is detected, it is moved back up. If `hold` is
 * true, the function attempts to move the brick down until it collides with
 * another brick or the bottom of the field. Once a collision is detected, the
 * brick is placed in its final position, and if the collision is with the
 * bottom of the field, the game state is updated to indicate that the brick has
 * been attached to the field.
 *
 * @param self A pointer to the Tetris game engine instance.
 * @param hold A boolean value indicating whether the action should be held
 * (continuously move down).
 */
static void _down(Tetris *self, bool hold) {
  if (!self) return;
  (void)hold;

  Brick *brick = self->data.current_brick;

  if (brick) {
    remove_brick(self->data.info.field, brick);
    bool is_collided = true;
    if (!hold) {
      brick->pos.y++;
      if ((is_collided = is_collide(self->data.info.field, brick))) {
        brick->pos.y--;
      }
    } else {
      while ((is_collided != is_collide(self->data.info.field, brick))) {
        brick->pos.y++;
      }
      brick->pos.y--;
    }

    place_brick(self->data.info.field, brick);
    if (is_collided) {
      self->state = TETRIS_ATTACH_STATE;
      self->data.current_brick = NULL;
    }
  }
}

/**
 * @brief Moves the current Tetris piece to the left.
 *
 * This function attempts to move the current Tetris piece to the left by
 * decrementing its x-coordinate. Before moving the piece, it checks for
 * collisions with the game field boundaries or other pieces. If a collision is
 * detected, the piece's position is not updated.
 *
 * @param self A pointer to the Tetris game engine instance.
 * @param hold A boolean value indicating whether the action should be held.
 * This parameter is not used in this function.
 */
static void _left(Tetris *self, bool hold) {
  if (!self) return;
  (void)hold;

  Brick *brick = self->data.current_brick;
  if (brick) {
    remove_brick(self->data.info.field, brick);

    brick->pos.x--;
    if (is_collide(self->data.info.field, brick)) {
      brick->pos.x++;
    }
    place_brick(self->data.info.field, brick);
  }
}

/**
 * @brief Moves the current Tetris piece to the right.
 *
 * This function attempts to move the current Tetris piece one block to the
 * right. It first checks if the piece can be moved without colliding with the
 * game field boundaries or other pieces. If a collision is detected, the
 * piece's position is not updated.
 *
 * @param self A pointer to the Tetris game engine instance.
 * @param hold A boolean value indicating whether the action should be held.
 * This parameter is not used in this function.
 */
static void _right(Tetris *self, bool hold) {
  if (!self) return;
  (void)hold;

  Brick *brick = self->data.current_brick;
  if (brick) {
    remove_brick(self->data.info.field, brick);
    brick->pos.x++;
    if (is_collide(self->data.info.field, brick)) {
      brick->pos.x--;
    }
    place_brick(self->data.info.field, brick);
  }
}

/**
 * @brief Rotates the current Tetris piece.
 *
 * This function rotates the current Tetris piece by 90 degrees. It first
 * removes the current piece from the game field, then attempts to rotate the
 * piece. If the rotation results in a collision with the game field boundaries
 * or other pieces, the piece is rotated back to its previous state. Finally,
 * the piece is placed back onto the game field.
 *
 * The rotation is performed by changing the brick state of each available
 * states, based on a fixed matrix with brick states.
 *
 * @param self A pointer to the Tetris game engine instance.
 * @param hold A boolean value indicating whether the action should be held.
 * This parameter is not used in this function.
 */
static void _action(Tetris *self, bool hold) {
  if (!self) return;
  (void)hold;

  Brick *brick = self->data.current_brick;
  if (brick) {
    remove_brick(self->data.info.field, brick);

    brick->next_state(brick);
    if (is_collide(self->data.info.field, brick)) {
      brick->prev_state(brick);
    }

    place_brick(self->data.info.field, brick);
  }
}

/**
 * @brief Destroys a Tetris game engine instance.
 *
 * This function is responsible for cleaning up and freeing resources associated
 * with a Tetris game engine instance.
 *
 * @param self A pointer to the Tetris game engine instance to be destroyed.
 */
static void _destroy(Tetris *self) {
  if (!self) return;

  self->data.current_brick = NULL;
  self->data.next_brick = NULL;
  if (self->data.info.field) {
    destroy_matrix(self->data.info.field, TETRIS_FIELD_HEIGHT);
    self->data.info.field = NULL;
  }
  if (self->data.info.next) {
    destroy_matrix(self->data.info.next, BRICK_HEIGHT);
    self->data.info.next = NULL;
  }
  if (self->repository) {
    self->repository->destroy(self->repository);
    self->repository = NULL;
  }

  free(self);
}

/**
 * @brief Starts the Tetris game or resets it to a new game state.
 *
 * This static function is responsible for starting the Tetris game or resetting
 * it to a new game state. It checks the current state of the game and performs
 * the necessary actions to either start a new game or reset the game to its
 * initial state after a game over. This includes clearing the game fields,
 * resetting the game level, score, and pause status, and spawning a new piece.
 *
 * @param self A pointer to the Tetris game engine instance.
 */
static void _start(Tetris *self) {
  if (!self) return;

  if (self->state == TETRIS_READY_STATE && self->on_startup) {
    self->on_startup(self);
  }

  if (self->state == TETRIS_GAMEOVER_STATE) {
    clear_tetris_fields(self);
    self->data.info.level = 1;
    self->data.info.score = 0;
    self->data.info.pause = 0;
  }

  self->_spawn(self);
}

/**
 * @brief Toggles the pause state of the Tetris game.
 *
 * This static function toggles the pause state of the Tetris game. If the game
 * is currently paused, it resumes the game by setting the pause flag to 0 and
 * changing the game state to TETRIS_MOVING_STATE. If the game is not paused, it
 * pauses the game by setting the pause flag to 1 and changing the game state to
 * TETRIS_PAUSE_STATE.
 *
 * @param self A pointer to the Tetris game engine instance.
 */
static void _pause(Tetris *self) {
  if (!self) return;
  if (self->data.info.pause) {
    self->data.info.pause = 0;
    self->state = TETRIS_MOVING_STATE;
  } else {
    self->data.info.pause = 1;
    self->state = TETRIS_PAUSE_STATE;
  }
}

/**
 * @brief Terminates the Tetris game engine instance.
 *
 * This static function is responsible for terminating the Tetris game engine
 * instance. It first checks if the instance is valid, then calls the
 * `on_shutdown` callback if it exists, and finally sets the game state to
 * `TETRIS_TERMINATED_STATE`.
 *
 * @param self A pointer to the Tetris game engine instance to be terminated.
 */
static void _terminate(Tetris *self) {
  if (!self) return;

  if (self->on_shutdown) {
    self->on_shutdown(self);
  }
  self->state = TETRIS_TERMINATED_STATE;
}

/**
 * @brief Updates the game state based on the game timer.
 *
 * This function is called periodically to update the game state based on the
 * game timer. It adjusts the game level and speed based on the current score,
 * checks if the game is in a moving state, and if so, moves the active piece
 * down. If the game is in an attach state, it checks for completed lines,
 * updates the score, and spawns a new piece.
 *
 * @param self A pointer to the Tetris game engine instance.
 * @return A boolean value indicating whether the game timer has ticked.
 */
static bool __tick(Tetris *self) {
  if (!self) return false;

  self->data.info.level = get_level_by_score(self->data.info.score);
  self->timer.timeout_sec = (11 - self->data.info.level) * 0.05;
  self->data.info.speed = self->timer.timeout_sec * 1000;

  bool is_ticked = self->timer.tick(&self->timer);

  if (is_ticked && self->state == TETRIS_MOVING_STATE) {
    self->down(self, false);
  } else if (self->state == TETRIS_ATTACH_STATE) {
    int ereased = erase_lines(self->data.info.field);
    self->data.info.score += get_reward_count(ereased);
    if (self->data.info.score > self->data.info.high_score) {
      self->data.info.high_score = self->data.info.score;
      write_highscore_to_file("highscore.txt", self->data.info.high_score);
    }

    self->_spawn(self);
  }
  return is_ticked;
}

/**
 * @brief Spawns a new piece in the Tetris game.
 *
 * This function is responsible for spawning a new piece in the Tetris game. It
 * first checks if there is a next piece to be spawned. If not, it fetches a
 * random piece from the repository. It then sets the current piece to the next
 * piece and fetches a new next piece. The function populates the game's next
 * piece display with the next piece's data. It attempts to place the current
 * piece at the center of the game field. If the placement is successful, the
 * game state is updated to moving; otherwise, the game state is set to game
 * over.
 *
 * @param self A pointer to the Tetris game engine instance.
 */
static void __spawn(Tetris *self) {
  if (!self) return;

  if (!self->data.next_brick) {
    self->data.next_brick = self->repository->get_random(self->repository);
  }
  self->data.current_brick = self->data.next_brick;
  self->data.next_brick = self->repository->get_random(self->repository);

  // populate next dto
  for (size_t row = 0; row < BRICK_HEIGHT; row++) {
    for (size_t col = 0; col < BRICK_WIDTH; col++) {
      if (self->data.next_brick
              ->states[self->data.next_brick->state][row][col]) {
        self->data.info.next[row][col] = self->data.next_brick->color;
      } else {
        self->data.info.next[row][col] = 0;
      }
    }
  }

  // THIS CORDS IS CENTER OF GAME FIELD
  self->data.current_brick->pos.x = TETRIS_FIELD_WIDTH / 2;
  self->data.current_brick->pos.y = 0;

  if (!is_collide(self->data.info.field, self->data.current_brick)) {
    place_brick(self->data.info.field, self->data.current_brick);
    self->state = TETRIS_MOVING_STATE;
  } else {
    self->state = TETRIS_GAMEOVER_STATE;
    self->data.info.pause = -1;
  }
}

/**
 * @brief Allocates and initializes a new Tetris game engine instance.
 *
 * This function dynamically allocates memory for a new Tetris game engine
 * instance using `malloc()`. It initializes the instance's fields, including
 * setting up function pointers for game actions, initializing the game timer,
 * and setting up the game data structure with a given brick repository. The
 * brick repository is used to manage the different types of bricks (pieces)
 * available in the game. If memory allocation fails, the function prints an
 * error message to stderr and exits the program with a failure status.
 *
 * @param repository A pointer to a TetrisBrickRepository structure for managing
 * brick data.
 * @return A pointer to the newly created Tetris game engine instance.
 */
Tetris *new_tetris(TetrisBrickRepository *repository) {
  Tetris *self = (Tetris *)malloc(sizeof(Tetris));
  if (!self) {
    fprintf(stderr, "Cannot allocate mem for Tetris\n");
    exit(-1);
  }

  self->up = _up;
  self->down = _down;
  self->left = _left;
  self->right = _right;
  self->action = _action;

  self->start = _start;
  self->pause = _pause;
  self->terminate = _terminate;
  self->destroy = _destroy;

  self->on_startup = _on_startup;
  self->on_shutdown = _on_shutdown;

  self->timer = create_timer(0.55);
  self->_spawn = __spawn;
  self->_tick = __tick;
  self->repository = repository;
  self->state = TETRIS_READY_STATE;

  self->data = (TetrisData){
      .current_brick = NULL,
      .next_brick = NULL,
      .info = {.field = create_matrix(TETRIS_FIELD_HEIGHT, TETRIS_FIELD_WIDTH),
               .next = create_matrix(BRICK_HEIGHT, BRICK_HEIGHT),
               .high_score = 0,
               .score = 0,
               .pause = 0,
               .speed = 0,
               .level = 1}};
  return self;
}

/**
 * @brief Provides a singleton instance of the Tetris game engine.
 *
 * This function ensures that only one instance of the Tetris game engine is
 * created and used throughout the application. It uses a static variable to
 * store the instance, and if the instance does not exist, it creates a new one
 * using the `new_tetris()` function. This approach is useful for managing
 * resources that should have a single, global point of access, such as the
 * Tetris game engine.
 *
 * @return A pointer to the singleton Tetris game engine instance.
 */
Tetris *provide_tetris() {
  static Tetris *tetris = NULL;
  if (!tetris) {
    tetris = new_tetris(provide_brick_repository());
  }
  return tetris;
}

/**
 * @brief Updates and returns the current state of the Tetris game.
 *
 * This function retrieves the current state of the Tetris game by calling the
 * game engine's tick function and then returns the updated game information. It
 * is designed to be called from the frontend to ensure that the latest game
 * state is available for rendering or game logic updates.
 *
 * @return A GameInfo_t structure containing the updated game state.
 */
GameInfo_t updateCurrentState() {
  Tetris *tetris = provide_tetris();
  tetris->_tick(tetris);
  return tetris->data.info;
}

/**
 * @brief Handles user input for Tetris game actions.
 *
 * This function processes user input to perform actions in the Tetris game,
 * such as starting, pausing, terminating, and moving the active piece. The
 * action to be performed is specified by the `action` parameter, which is an
 * enumeration of possible user actions. The `hold` parameter indicates whether
 * the action should be held (e.g., moving a piece down continuously).
 *
 * @param action The action to be performed, as defined by the UserAction_t
 * enumeration.
 * @param hold A boolean value indicating whether the action should be held.
 */
void userInput(UserAction_t action, bool hold) { dispatch(action, hold); }
