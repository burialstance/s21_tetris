#include "keyboard.h"

/**
 * @brief Calculates the time difference between two timespec structures.
 *
 * This function computes the difference in time between two timespec
 * structures, returning the result as a double value representing the time
 * difference in seconds. It accounts for both seconds and nanoseconds
 * components of the timespec structures.
 *
 * @param start The start timespec structure.
 * @param end The end timespec structure.
 * @return The time difference in seconds as a double value.
 */
static double time_delta(struct timespec start, struct timespec end) {
  return (start.tv_sec - end.tv_sec) +
         ((start.tv_nsec - end.tv_nsec) / 1000000000.0);
}

/**
 * @brief Determines if a key is being held down for a specified duration.
 *
 * This function checks if a given key has been pressed and held for a duration
 * specified by the `hold_timeout_ms` parameter. It uses the current time and
 * the time of the last call to determine if the key press meets the hold
 * condition.
 *
 * @param key The key code to check for a hold condition.
 * @param hold_timeout_ms The duration in milliseconds for which the key must be
 * held.
 * @return TRUE if the key is being held down for the specified duration, FALSE
 * otherwise.
 */
static bool is_press_and_hold(int key, double hold_timeout_ms) {
  static struct timespec last_call = {0};
  struct timespec now;
  timespec_get(&now, TIME_UTC);

  bool is_hold = FALSE;
  static int prev_key = ERR;

  if (prev_key != key) {
    prev_key = key;
  } else {
    is_hold = time_delta(now, last_call) <= hold_timeout_ms;
  }

  last_call = now;
  return is_hold;
}

/**
 * @brief Adds a new listener for a specific key to the keyboard controller.
 *
 * This function dynamically grows the array of listeners by reallocating memory
 * to accommodate the new listener. It then initializes a new
 * KeyboardButtonListener structure with the specified key and callback
 * function, and adds it to the array. The listeners array is expanded to ensure
 * that it can hold the new listener without losing existing data, following the
 * principle of doubling the capacity to minimize the number of reallocations
 * and improve performance.
 *
 * @param self A pointer to the KeyboardController instance.
 * @param key The key code for which the listener is being added.
 * @param callback The callback function to be executed when the key is pressed.
 */
static void _add_listener(KeyboardController *self, int key,
                          listener_callback callback) {
  self->listeners = realloc(self->listeners, sizeof(KeyboardButtonListener) *
                                                 (self->listeners_count + 1));

  KeyboardButtonListener new_listener = {.key = key, .callback = callback};
  self->listeners[self->listeners_count] = new_listener;
  self->listeners_count++;
}

/**
 * @brief Frees the memory allocated for the KeyboardController and its
 * listeners.
 *
 * This function is responsible for safely deallocating the memory used by the
 * KeyboardController instance and its associated listeners array. It first
 * checks if the KeyboardController pointer is not NULL and if the listeners
 * array exists. If so, it frees the memory allocated for the listeners array,
 * resets the listeners count to 0, and sets the listeners pointer to NULL.
 * Finally, it frees the memory allocated for the KeyboardController instance
 * itself.
 *
 * @param self A pointer to the KeyboardController instance to be destroyed.
 */
static void _destroy(KeyboardController *self) {
  if (!self) return;

  if (self->listeners) {
    free(self->listeners);
    self->listeners_count = 0;
    self->listeners = NULL;
  }
  free(self);
}

/**
 * @brief Emits a button press event to the registered listeners and a general
 * handler.
 *
 * This function is responsible for triggering the appropriate callback
 * functions when a button press event occurs. It first checks if there is a
 * general handler (`on_emit`) registered with the KeyboardController. If so, it
 * calls this handler with the button information. Then, it iterates over the
 * array of registered listeners, checking if any of them are listening for the
 * pressed button. If a match is found, it calls the corresponding callback
 * function with the button information.
 *
 * @param self A pointer to the KeyboardController instance.
 * @param btn The Button structure containing the button's key and hold state.
 */
static void _emit(KeyboardController *self, Button btn) {
  if (self->on_emit) self->on_emit(btn);

  for (size_t i = 0; (i < self->listeners_count) && self->listeners; i++) {
    if (self->listeners[i].key == btn.key) self->listeners[i].callback(btn);
  }
}

/**
 * @brief Listens for button presses and emits events based on the pressed key.
 *
 * This function continuously listens for button presses
 * using the `getch()` function from ncurses (any "ERR"(-1) signals will ignored
 * for listeners!!!). It creates a Button structure for each key press,
 * including whether the key is being held down. If the key is not being held
 * down, it emits an event immediately. If the key is being held down, it emits
 * an event only if the key was not previously hold down. This function also
 * keeps track of the last button pressed to handle hold events correctly.
 *
 * @param self A pointer to the KeyboardController instance.
 * @return A Button structure containing the key code and hold state of the last
 * button pressed.
 */
static Button _listen(KeyboardController *self) {
  static Button last_btn = {0};
  int key = getch();

  Button btn = {.key = key, .hold = is_press_and_hold(btn.key, 0.075)};

  if (btn.key != ERR) {
    if (!btn.hold) {
      self->emit(self, btn);
    } else if (btn.hold && !last_btn.hold) {
      self->emit(self, btn);
    }

    last_btn = btn;
  }

  return btn;
}

/**
 * @brief Allocates and initializes a new KeyboardController instance.
 *
 * This function dynamically allocates memory for a new KeyboardController
 * instance using `malloc()`. It initializes the instance's fields, including
 * setting the listeners array to NULL and the listeners count to 0. It also
 * assigns function pointers for adding listeners, listening for button presses,
 * emitting events, and destroying the controller. If memory allocation fails,
 * the function prints an error message to stderr and exits the program with a
 * failure status.
 *
 * @return A pointer to the newly created KeyboardController instance.
 */
KeyboardController *new_keyboard() {
  KeyboardController *self =
      (KeyboardController *)malloc(sizeof(KeyboardController));
  if (!self) {
    fprintf(stderr, "Cannot allocate mem for KeyboardController\n");
    exit(-1);
  }

  self->listeners = NULL;
  self->listeners_count = 0;

  self->add_listener = _add_listener;
  self->listen = _listen;
  self->emit = _emit;
  self->destroy = _destroy;

  return self;
}

/**
 * @brief Provides a singleton instance of KeyboardController.
 *
 * This function ensures that only one instance of KeyboardController is created
 * and used throughout the application. It uses a static variable to store the
 * instance, and if the instance does not exist, it creates a new one using the
 * `new_keyboard()` function. This approach is useful for managing resources
 * that should have a single, global point of access, such as keyboard input
 * handling.
 *
 * @return A pointer to the singleton KeyboardController instance.
 */
KeyboardController *provide_keyboard() {
  static KeyboardController *keyboard = NULL;
  if (!keyboard) {
    keyboard = new_keyboard();
  }
  return keyboard;
}

// EXAMPLE

// void on_up_pressed(Button btn){
//     mvprintw(0, 0, "on_up_pressed(key=%d, hold=%s)\n", btn.key, btn.hold ?
//     "TRUE" : "FALSE");
// };
// void on_down_pressed(Button btn){
//     mvprintw(0, 0, "on_down_pressed(key=%d, hold=%s)\n", btn.key, btn.hold ?
//     "TRUE" : "FALSE");
// };
// void on_left_pressed(Button btn){
//     mvprintw(0, 0, "on_left_pressed(key=%d, hold=%s)\n", btn.key, btn.hold ?
//     "TRUE" : "FALSE");
// };
// void on_right_pressed(Button btn){
//     mvprintw(0, 0, "on_right_pressed(key=%d, hold=%s)\n", btn.key, btn.hold ?
//     "TRUE" : "FALSE");
// };
// void manual_handler(Button btn){
//     mvprintw(1, 0, "manual_handler(key=%d, hold=%s)\n", btn.key, btn.hold ?
//     "TRUE" : "FALSE");
// }
// int main(){
//     KeyboardController *kb = provide_keyboard();
//     kb->add_listener(kb, KEY_UP, on_up_pressed);
//     kb->add_listener(kb, KEY_DOWN, on_down_pressed);
//     kb->add_listener(kb, KEY_LEFT, on_left_pressed);
//     kb->add_listener(kb, KEY_RIGHT, on_right_pressed);
//
//     initscr();
//     curs_set(0);
//     noecho();
//     cbreak();
//     keypad(stdscr, TRUE);
//     timeout(400);
//
//     // case 1
//     int counter = 0;
//     while(kb->listen(kb).key != KEY_END) {
//         mvprintw(5, 0, "%s %d", "case 1", counter++);
//         refresh();
//     }
//
//     // case 2
//     counter = 0;
//     Button btn = {0};
//     while(btn.key != KEY_END){
//         btn = kb->listen(kb);
//         manual_handler(btn);
//         mvprintw(5, 0, "%s %d", "case 2", counter++);
//         refresh();
//     }
//
//     kb->destroy(kb);
//     endwin();
//     return 0;
// }
