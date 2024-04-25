#include "bricks.h"

/**
 * @brief Advances the state of a Brick to the next state in a circular manner.
 *
 * This function is used to cycle through the different states of a Brick
 * object, such as rotating a piece in the Tetris game.
 *
 * @param self A pointer to the Brick object whose state is to be advanced.
 */
static void _next_state(Brick *self) {
  if (!self) return;

  if (self->state + 1 < self->total_states) {
    self->state++;
  } else {
    self->state = 0;
  }
}

/**
 * @brief Changes the state of a Brick to the previous state.
 *
 * This static function updates the state of a Brick object to the previous
 * state. This is useful for cycling through different states of a Brick, such
 * as rotating a piece in Tetris.
 *
 * @param self A pointer to the Brick object whose state is to be advanced.
 */
static void _prev_state(Brick *self) {
  if (!self) return;

  if (self->state == 0) {
    self->state = self->total_states - 1;
  } else {
    self->state--;
  }
}

/**
 * @brief Retrieves a Brick from the TetrisBrickRepository by index.
 *
 * This static function retrieves a Brick from the TetrisBrickRepository by its
 * index. It initializes the Brick's state to 0 and sets its position to (0, 0).
 * (bcs tetris manipualte with pieces from ptr)
 *
 * @param self A pointer to the TetrisBrickRepository instance.
 * @param index The index of the Brick to retrieve.
 * @return A pointer to the retrieved Brick.
 */
static Brick *_get(TetrisBrickRepository *self, int index) {
  if (!self || !self->items) return NULL;

  Brick *item = &self->items[index];
  item->state = 0;
  item->pos.x = 0;
  item->pos.y = 0;

  return item;
}

/**
 * @brief Retrieves a random brick from the Tetris brick repository.
 *
 * This function selects a random brick from the Tetris brick repository,
 * ensuring that the same brick is not selected consecutively. It uses a static
 * variable to keep track of the last selected index and a guard mechanism to
 * prevent the same brick from being selected too frequently in succession. This
 * mechanism resets the guard block after a certain number of selections to
 * maintain randomness.
 *
 * @param self A pointer to the TetrisBrickRepository structure.
 * @return A pointer to the randomly selected Brick structure.
 */
static Brick *_get_random(TetrisBrickRepository *self) {
  if (!self) return NULL;
  int index = 0;

  // static int guard_block = 0;
  // guard_block = (index == 0) ? 0 : guard_block + 1;
  // if (guard_block >= 7) {
  //   index = guard_block = 0;
  // }

  static int last_index = 0;
  do {
    index = rand() % self->items_count;
  } while ((index == last_index) && (self->items_count > 1));
  last_index = index;

  return self->get(self, index);
}

/**
 * @brief Adds a new brick to the Tetris brick repository.
 *
 * This function dynamically adds a new brick to the Tetris brick repository.
 * It reallocates the memory for the items array to accommodate the new brick,
 * ensuring that the repository can grow in size as needed. This approach is
 * efficient for managing a collection of bricks, as it allows for the addition
 * of new bricks without the need to pre-allocate a large array.
 *
 * @param self A pointer to the TetrisBrickRepository structure.
 * @param brick The brick to be added to the repository.
 */
static void _create(TetrisBrickRepository *self, Brick brick) {
  if (!self) return;
  self->items = realloc(self->items, sizeof(Brick) * (self->items_count + 1));
  self->items[self->items_count] = brick;
  self->items_count++;
}

/**
 * @brief Frees the memory allocated for a TetrisBrickRepository instance.
 *
 * This function is responsible for deallocating the memory used by a
 * TetrisBrickRepository instance. Frees the memory allocated for the `items`
 * array, sets the `items` pointer to NULL, and finally frees the memory
 * allocated for the TetrisBrickRepository instance itself. This approach
 * ensures that all dynamically allocated memory is properly released,
 * preventing memory leaks.
 *
 * @param self A pointer to the TetrisBrickRepository instance to be destroyed.
 */
static void _destroy(TetrisBrickRepository *self) {
  if (!self) return;

  free(self->items);
  self->items = NULL;
  self->items_count = 0;
  free(self);
}

static void populate_repo_defaults(TetrisBrickRepository *repo) {
  if (!repo) return;

  // I
  repo->create(
      repo,
      (Brick){.pos = {0, 0},
              .color = BrickLightBlueColor,
              .state = 0,
              .total_states = 2,
              .next_state = _next_state,
              .prev_state = _prev_state,
              .states = {
                  {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
                  {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
              }});

  // O
  repo->create(
      repo,
      (Brick){.pos = {0, 0},
              .color = BrickYellowColor,
              .state = 0,
              .total_states = 1,
              .next_state = _next_state,
              .prev_state = _prev_state,
              .states = {
                  {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}}});

  // S
  repo->create(
      repo,
      (Brick){.pos = {0, 0},
              .color = BrickGreenColor,
              .state = 0,
              .total_states = 2,
              .next_state = _next_state,
              .prev_state = _prev_state,
              .states = {
                  {{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}},
                  {{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}}}});

  // Z
  repo->create(
      repo,
      (Brick){.pos = {0, 0},
              .color = BrickRedColor,
              .state = 0,
              .total_states = 2,
              .next_state = _next_state,
              .prev_state = _prev_state,
              .states = {
                  {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
                  {{0, 0, 0, 1}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}}}});

  // L
  repo->create(
      repo,
      (Brick){.pos = {0, 0},
              .color = BrickOrangeColor,
              .state = 0,
              .total_states = 4,
              .next_state = _next_state,
              .prev_state = _prev_state,
              .states = {
                  {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 1, 0, 0}, {0, 0, 0, 0}},
                  {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
                  {{0, 0, 0, 1}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
                  {{0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}}}});

  // J
  repo->create(
      repo,
      (Brick){.pos = {0, 0},
              .color = BrickDarkBlueColor,
              .state = 0,
              .total_states = 4,
              .next_state = _next_state,
              .prev_state = _prev_state,
              .states = {
                  {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}},
                  {{0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
                  {{0, 1, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
                  {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}}});

  // T
  repo->create(
      repo,
      (Brick){.pos = {0, 0},
              .color = BrickMagentaColor,
              .state = 0,
              .total_states = 4,
              .next_state = _next_state,
              .prev_state = _prev_state,
              .states = {
                  {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}},
                  {{0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 1, 0}, {0, 0, 0, 0}},
                  {{0, 0, 1, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
                  {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}}}});
}

void _populate_custom(TetrisBrickRepository *self) {
  self->create(
      self,
      (Brick){.pos = {0, 0},
              .color = BrickYellowColor,
              .state = 0,
              .total_states = 4,
              .next_state = _next_state,
              .prev_state = _prev_state,
              .states = {
                  {{0, 0, 0, 0}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}},
                  {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 1, 1, 1}, {0, 1, 1, 1}},
                  {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}},
                  {{0, 0, 0, 0}, {1, 1, 1, 0}, {1, 1, 1, 0}, {1, 1, 1, 0}},
              }});

  self->create(
      self,
      (Brick){.pos = {0, 0},
              .color = BrickRedColor,
              .state = 0,
              .total_states = 2,
              .next_state = _next_state,
              .prev_state = _prev_state,
              .states = {
                  {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 1, 1, 0}, {1, 0, 0, 1}},
                  {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 0, 0, 1}, {0, 1, 1, 0}}}});
}

/**
 * @brief Creates a new TetrisBrickRepository instance.
 *
 * This function initializes a new TetrisBrickRepository instance,
 * which is used to manage the different types of bricks (pieces) available in
 * the Tetris game. It allocates memory for the repository, initializes its
 * fields, and assigns function pointers for various operations such as getting
 * a brick, getting a random brick, creating a new brick, populating custom
 * bricks, and destroying the repository.
 *
 * @return A pointer to the newly created TetrisBrickRepository instance.
 */
TetrisBrickRepository *new_brick_repository() {
  TetrisBrickRepository *self =
      (TetrisBrickRepository *)malloc(sizeof(TetrisBrickRepository));
  if (!self) {
    fprintf(stderr, "Cannot allocate mem for TetrisBrickRepository\n");
    exit(-1);
  }

  self->items = NULL;
  self->items_count = 0;

  self->get = _get;
  self->get_random = _get_random;
  self->create = _create;
  self->populate_custom = _populate_custom;
  self->destroy = _destroy;

  return self;
}

/**
 * @brief Provides a singleton instance of the Tetris brick repository.
 *
 * This function ensures that only one instance of the Tetris brick repository
 * is created and used throughout the application. It uses a static variable to
 * store the instance, and if the instance does not exist, it creates a new one
 * using the `new_brick_repository()` function. After creating the new instance,
 * it populates the repository with default brick data using the
 * `populate_repo_defaults()` function. This approach is useful for managing
 * resources that should have a single, global point of access, such as the
 * Tetris brick repository.
 *
 * @return A pointer to the singleton Tetris brick repository instance.
 */
TetrisBrickRepository *provide_brick_repository() {
  static TetrisBrickRepository *repo = NULL;
  if (!repo) {
    repo = new_brick_repository();
    populate_repo_defaults(repo);
  }
  return repo;
}

// // USECASES
// int main(){
//     TetrisBrickRepository *repo = provide_brick_repository();
//     printf("repo count %d\n", repo->items_count);
//     for (size_t i = 0; i < repo->items_count; i++){
//         Brick brick = repo->get(repo, i);
//         printf("--------id %ld--------\n", i);
//         for (size_t state = 0; state < brick.total_states; state++) {
//             printf("state %d\n", state);
//             for (size_t r = 0; r < BRICK_HEIGTH; r++){
//                 for (size_t c = 0; c < BRICK_WIDTH; c++){
//                     printf("%d", brick.states[state][r][c]);
//                 }
//                 printf("\n");
//             }
//         }
//         printf("---------------------\n\n");
//     }
//     // Brick *item = repo->get(repo, 0);
//     // item->rotate_right(item);
//     // item->rotate_right(item);
//     // for (size_t r = 0; r < BRICK_HEIGTH; r++){
//     //     for (size_t c = 0; c < BRICK_WIDTH; c++){
//     //         printf("%d", item->matrix[r][c]);
//     //     }
//     //     printf("\n");
//     // }
//     repo->destroy(repo);
//     return 0;
// }
