#ifndef CLI_KEYBOARD_KEYBOARD_H
#define CLI_KEYBOARD_KEYBOARD_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Structure representing a button with a key and a hold state.
 *
 * This structure defines a button with two properties: a key, which could
 * represent the button's identifier or function, and a hold state, which is a
 * boolean indicating whether the button is currently being held down or not.
 * This structure can be used to model buttons in a user interface, where the
 * key might correspond to a specific action, and the hold state could be used
 * to track whether the action is continuously being performed (e.g., holding
 * down a key in a game to move a bricks).
 *
 * @struct Button
 * @var int key
 *      The identifier or function of the button.
 * @var bool hold
 *      Indicates whether the button is currently being held down.
 */
typedef struct {
  int key;
  bool hold;
} Button;

/**
 * @brief Typedef for a function pointer that takes a Button as an argument and
 * returns void.
 *
 * This typedef defines a function pointer type named `listener_callback`. The
 * function pointer points to a function that takes a single argument of type
 * `Button` and returns no value (`void`). This can be used to define callback
 * functions that are triggered by button events, where the `Button` structure
 * contains information about the button's state, such as its key and whether it
 * is being held down.
 *
 * @typedef listener_callback
 * @param btn The Button structure containing the button's key and hold state.
 */
typedef void (*listener_callback)(Button btn);

/**
 * @brief Structure representing a keyboard button listener.
 *
 * This structure encapsulates a keyboard button listener, which is designed to
 * listen for a specific key press event and trigger a callback function when
 * that event occurs. The structure contains a `key` field, which specifies the
 * key code that the listener is interested in, and a `callback` field, which is
 * a function pointer to the callback function that should be executed when the
 * key is pressed. The callback function is expected to take a single argument
 * of type `Button`, which contains information about the button's state, such
 * as its key and whether it is being held down.
 *
 * @struct __keyboard_listener
 * @var int key
 *      The key code that the listener is interested in.
 * @var listener_callback callback
 *      Function pointer to the callback function to be executed when the key is
 * pressed.
 */
typedef struct __keyboard_listener {
  int key;
  listener_callback callback;
} KeyboardButtonListener;

/**
 * @brief Structure representing a keyboard controller with listeners for
 * specific keys.
 *
 * This structure encapsulates a keyboard controller that manages listeners for
 * specific key presses. It contains an array of `KeyboardButtonListener`
 * structures, each of which specifies a key and a callback function to be
 * executed when that key is pressed. The structure also includes function
 * pointers for adding a new listener, emitting a button press event, handling a
 * button press event for any button, listening for button presses, and
 * destroying the keyboard controller.
 *
 * @struct __keyboard
 * @var KeyboardButtonListener *listeners
 *      Array of KeyboardButtonListener structures, each specifying a key and a
 * callback function.
 * @var size_t listeners_count
 *      The number of listeners currently registered.
 * @var void (*add_listener)(struct __keyboard *self, int key, listener_callback
 * callback) Function pointer for adding a new listener for a specific key.
 * @var void (*emit)(struct __keyboard *self, Button btn)
 *      Function pointer for emitting a button press event.
 * @var void (*on_emit)(Button btn)
 *      Function pointer for handling a button press event for any button.
 * @var Button (*listen)(struct __keyboard *self)
 *      Function pointer for listening for button presses.
 * @var void (*destroy)(struct __keyboard *self)
 *      Function pointer for destroying the keyboard controller.
 */
typedef struct __keyboard {
  KeyboardButtonListener *listeners;
  size_t listeners_count;

  void (*add_listener)(struct __keyboard *self, int key,
                       listener_callback callback);
  void (*emit)(struct __keyboard *self, Button btn);
  void (*on_emit)(Button btn);  // can use for any button
  Button (*listen)(struct __keyboard *self);
  void (*destroy)(struct __keyboard *self);
} KeyboardController;

KeyboardController *new_keyboard();
KeyboardController *provide_keyboard();

#endif  // !CLI_KEYBOARD_KEYBOARD_H