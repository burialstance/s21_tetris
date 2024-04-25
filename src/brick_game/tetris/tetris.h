#ifndef BRICKGAME_TETRIS_TETRIS_H
#define BRICKGAME_TETRIS_TETRIS_H

#define TETRIS_FIELD_WIDTH 10
#define TETRIS_FIELD_HEIGHT 20

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "bricks/bricks.h"
#include "timer/timer.h"
#include "utils/utils.h"

/**
 * @brief Enumeration representing user actions in a Tetris game.
 *
 * This enumeration defines the possible actions a user can perform in a Tetris
 * game, such as starting the game, pausing, terminating, and moving the active
 * brick in different directions. Each action corresponds to a specific keyboard
 * input or control scheme.
 *
 * @enum UserAction_t
 * @var Start Start the game.
 * @var Pause Pause the game.
 * @var Terminate End the game.
 * @var Left Move the active brick to the left.
 * @var Right Move the active brick to the right.
 * @var Up Not used.
 * @var Down Move the active brick down faster.
 * @var Action Perform the default action for the active brick (usually
 * rotating).
 */
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

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
void userInput(UserAction_t action, bool hold);

/**
 * @brief Dispatches user actions to the Finite State Machine (FSM) for Tetris
 * game.
 *
 * This function acts as an adapter between user input and the game's Finite
 * State Machine (FSM). It is called by the `userInput` function to process user
 * actions, such as starting, pausing, terminating, and moving the active piece
 * in the Tetris game. The action to be dispatched is specified by the `action`
 * parameter, which is an enumeration of possible user actions. The `hold`
 * parameter indicates whether the action should be held (e.g., moving a piece
 * down continuously).
 *
 * @param action The action to be dispatched, as defined by the UserAction_t
 * enumeration.
 * @param hold A boolean value indicating whether the action should be held.
 */
void dispatch(UserAction_t action, bool hold);

/**
 * @brief Structure representing the game state in Tetris.
 *
 * This structure holds various information about the current state of the
 * Tetris game, including the game field, the next piece to be played, the
 * current score, high score, game level, speed, and pause status.
 *
 * @struct GameInfo_t
 * @var field A 2D array representing the current state of the game field.
 * @var next A 2D array representing the next piece to be played.
 * @var score The current score of the player.
 * @var high_score The highest score achieved in the current game session.
 * @var level The current level of the game.
 * @var speed The current speed of the game.
 * @var pause A boolean value indicating whether the game is paused.
 */
typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

/**
 * @brief Return the current state of the Tetris game.
 *
 * This function is responsible for updating the game state, including the game
 * field, the next piece, the current score, high score, game level, speed, and
 * pause status. It is called from the frontend to retrieve the latest game
 * state for rendering or game logic updates.
 *
 * @return A GameInfo_t structure containing the current game state.
 */

GameInfo_t updateCurrentState();

/**
 * @brief Enumeration representing the states of the Tetris game.
 *
 * This enumeration defines the various states the Tetris game can be in,
 * such as ready, spawning, moving, attaching, game over, paused, and
 * terminated. Each state corresponds to a specific phase in the game's
 * lifecycle.
 *
 * @enum TetriState
 * @var TETRIS_READY_STATE The tetris is ready to start.
 * @var TETRIS_SPAWN_STATE A new piece is being spawned.
 * @var TETRIS_MOVING_STATE The active piece is moving.
 * @var TETRIS_ATTACH_STATE The active piece is being attached with another
 * pieces.
 * @var TETRIS_GAMEOVER_STATE The game has ended.
 * @var TETRIS_PAUSE_STATE The tetris is paused.
 * @var TETRIS_TERMINATED_STATE The tetris has been terminated.
 */
typedef enum {
  TETRIS_READY_STATE = 0,
  TETRIS_SPAWN_STATE,
  TETRIS_MOVING_STATE,
  TETRIS_ATTACH_STATE,
  TETRIS_GAMEOVER_STATE,
  TETRIS_PAUSE_STATE,
  TETRIS_TERMINATED_STATE
} TetriState;

/**
 * @brief Structure containing the current state and data of the Tetris game.
 *
 * This structure encapsulates the game state and relevant data for the Tetris
 * game, including the current game information, the current and next bricks
 * (pieces) in play.
 *
 * @struct TetrisData
 * @var info A GameInfo_t structure containing the current game state
 * information.
 * @var current_brick A pointer to the Brick structure representing the current
 * active piece.
 * @var next_brick A pointer to the Brick structure representing the next piece
 * to be played.
 */
typedef struct {
  GameInfo_t info;
  Brick *current_brick;
  Brick *next_brick;
} TetrisData;

/**
 * @brief Structure representing the Tetris game engine.
 *
 * This structure encapsulates the core functionality of the Tetris game engine,
 * including the game timer, current state, game data, and various function
 * pointers for game actions and lifecycle events.
 *
 * @struct Tetris
 * @var timer A Timer structure for managing game timing.
 * @var state The current state of the game, as defined by the TetriState
 * enumeration.
 * @var data A TetrisData structure containing the current game state and data.
 * @var repository A pointer to a TetrisBrickRepository structure for managing
 * brick (piece) data.
 * @var start A function pointer for starting the game.
 * @var pause A function pointer for pausing the game.
 * @var terminate A function pointer for terminating the game.
 * @var up A function pointer for moving the current piece up.
 * @var down A function pointer for moving the current piece down.
 * @var left A function pointer for moving the current piece left.
 * @var right A function pointer for moving the current piece right.
 * @var action A function pointer for performing the default action for the
 * current piece.
 * @var _tick A function pointer for the game's tick function, which updates the
 * game state.
 * @var _spawn A function pointer for spawning a new piece.
 * @var on_startup A function pointer for actions to be performed on game
 * startup.
 * @var on_shutdown A function pointer for actions to be performed on game
 * shutdown.
 * @var destroy A function pointer for destroying the Tetris game engine
 * instance.
 */
typedef struct __tetris {
  Timer timer;
  TetriState state;
  TetrisData data;

  TetrisBrickRepository *repository;

  void (*start)(struct __tetris *self);
  void (*pause)(struct __tetris *self);
  void (*terminate)(struct __tetris *self);

  void (*up)(struct __tetris *self, bool hold);
  void (*down)(struct __tetris *self, bool hold);
  void (*left)(struct __tetris *self, bool hold);
  void (*right)(struct __tetris *self, bool hold);
  void (*action)(struct __tetris *self, bool hold);

  bool (*_tick)(struct __tetris *self);
  void (*_spawn)(struct __tetris *self);

  void (*on_startup)(struct __tetris *self);
  void (*on_shutdown)(struct __tetris *self);

  void (*destroy)(struct __tetris *self);
} Tetris;

/**
 * @brief Creates a new Tetris game engine instance.
 *
 * This function initializes a new Tetris game engine instance with a given
 * brick repository. The brick repository is used to manage the different types
 * of bricks (pieces) available in the game.
 *
 * @param repository A pointer to a TetrisBrickRepository structure for managing
 * brick data.
 * @return A pointer to the newly created Tetris game engine instance.
 */
Tetris *new_tetris(TetrisBrickRepository *repository);

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
Tetris *provide_tetris();

#endif