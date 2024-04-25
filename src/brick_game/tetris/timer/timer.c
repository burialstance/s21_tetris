#include "timer.h"

#include <stdio.h>

/**
 * @brief Updates timer and checks if a tick has occurred.
 *
 * This function calculates the time elapsed since the last tick and determines
 * if the specified timeout has been reached. If the timeout is reached, it
 * increments the tick count and updates the last tick time. This function is
 * typically used in game loops to manage timing and game state updates.
 *
 * @param self A pointer to the Timer structure representing the game timer.
 * @return A boolean value indicating whether a tick has occurred (true) or not
 * (false).
 */
static bool _tick(Timer *self) {
  struct timespec now;
  timespec_get(&now, TIME_UTC);

  double time_delta_sec = (now.tv_sec - self->_last_tick.tv_sec) +
                          ((now.tv_nsec - self->_last_tick.tv_nsec) / 1e9);

  bool is_ticked = time_delta_sec >= self->timeout_sec;
  if (is_ticked) {
    self->ticks += 1;
    self->_last_tick = now;
  }
  return is_ticked;
}

/**
 * @brief Creates a new Timer instance with a specified timeout.
 *
 * This function initializes a new Timer structure with a given timeout in
 * seconds. It sets the initial tick count to 0 and captures the current time as
 * the last tick. The Timer structure is designed to manage timing events, such
 * as game ticks or delays.
 *
 * @param timeout_sec The timeout value in seconds for the Timer.
 * @return A Timer structure initialized with the specified timeout and current
 * time.
 */
Timer create_timer(double timeout_sec) {
  struct timespec last_tick = {0};
  timespec_get(&last_tick, TIME_UTC);

  return (Timer){.ticks = 0,
                 .timeout_sec = timeout_sec,
                 ._last_tick = last_tick,
                 .tick = _tick};
}
