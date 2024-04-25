#include "tetris.h"

/**
 * @brief Dispatches user actions to the Tetris game engine based on the current
 * game state.
 *
 * This function processes user actions in the Tetris game and dispatches them
 * to the appropriate game engine functions based on the current game state. The
 * game state is determined by the `state` field of the Tetris game engine
 * instance. The `action` parameter specifies the user action to be performed,
 * and the `hold` parameter indicates whether the action should be held (e.g.,
 * moving a piece down continuously).
 *
 * The function handles various game states, including ready, spawn, moving,
 * pause, and game over states, and performs actions such as starting, pausing,
 * terminating, and moving the active piece.
 *
 * @param action The action to be performed, as defined by the UserAction_t
 * enumeration.
 * @param hold A boolean value indicating whether the action should be held.
 */
void dispatch(UserAction_t action, bool hold) {
  (void)hold;

  Tetris *tetris = provide_tetris();
  switch (tetris->state) {
    case TETRIS_READY_STATE:
      switch (action) {
        case Start:
          tetris->start(tetris);
          break;
        case Terminate:
          tetris->terminate(tetris);
          break;

        default:
          break;
      }
      break;

    case TETRIS_SPAWN_STATE:
      switch (action) {
        case Pause:
          tetris->pause(tetris);
          break;
        case Terminate:
          tetris->terminate(tetris);
          break;

        default:
          break;
      }
      break;

    case TETRIS_MOVING_STATE:
      switch (action) {
        case Left:
          tetris->left(tetris, hold);
          break;
        case Right:
          tetris->right(tetris, hold);
          break;
        case Up:
          tetris->up(tetris, hold);
          break;
        case Down:
          tetris->down(tetris, hold);
          break;
        case Action:
          tetris->action(tetris, hold);
          break;
        case Pause:
          tetris->pause(tetris);
          break;
        case Terminate:
          tetris->terminate(tetris);
          break;

        default:
          break;
      }
      break;

    case TETRIS_PAUSE_STATE:
      switch (action) {
        case Start:
        case Pause:
          tetris->pause(tetris);
          break;
        case Terminate:
          tetris->terminate(tetris);
          break;

        default:
          break;
      }
      break;

    case TETRIS_GAMEOVER_STATE:
      switch (action) {
        case Start:
          tetris->start(tetris);
          break;
        case Terminate:
          tetris->terminate(tetris);
          break;

        default:
          break;
      }
      break;

    default:
      break;
  }
}