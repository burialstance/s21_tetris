#include <stdbool.h>
#include <time.h>

/**
 * @brief Structure representing a game timer for Tetris.
 *
 * This structure encapsulates the functionality of a game timer, including the
 * number of ticks, the timeout in seconds, the last tick time, and a function
 * pointer for the tick function. The timer is used to manage the timing of game
 * events, such as piece movement and game updates.
 *
 * @struct Timer
 * @var ticks The number of ticks that have occurred since the timer was
 * started.
 * @var timeout_sec The timeout in seconds for the timer.
 * @var _last_tick A timespec structure representing the time of the last tick.
 * @var tick A function pointer for the tick function, which is called on each
 * tick.
 */
typedef struct __timer {
  int ticks;
  double timeout_sec;
  struct timespec _last_tick;
  bool (*tick)(struct __timer *self);
} Timer;

/**
 * @brief Creates a new timer with a specified timeout.
 *
 * This function initializes a new timer with a given timeout period in seconds.
 * The timer can be used to schedule events or actions to occur after the
 * specified timeout.
 *
 * @param timeout_sec The timeout period in seconds for the timer.
 * @return A Timer structure representing the newly created timer.
 */
Timer create_timer(double timeout_sec);