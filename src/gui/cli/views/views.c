#include "views.h"

#include <stdlib.h>

/**
 * @brief Updates the RootView instance and its components.
 *
 * This function iterates through the components of a RootView instance (header,
 * content, footer) and calls their update methods if they exist. This is useful
 * for refreshing the UI or re-rendering the components based on new data or
 * state changes.
 *
 * @param self Pointer to the RootView instance to be updated.
 */
static void _update(RootView *self) {
  if (!self) return;

  if (self->header) {
    self->header->update(self->header);
  }
  if (self->content) {
    self->content->update(self->content);
  }
  if (self->footer) {
    self->footer->update(self->footer);
  }
}

/**
 * @brief Destroys a RootView instance and its components.
 *
 * This function safely deallocates memory for a RootView instance and its
 * components (header, content, footer). It checks for the existence of each
 * component before calling its destroy method and then sets the pointer to NULL
 * to avoid dangling pointers. Finally, it frees the memory allocated for the
 * RootView instance itself.
 *
 * @param self Pointer to the RootView instance to be destroyed.
 */
static void _destroy(RootView *self) {
  if (!self) return;

  if (self->header) {
    self->header->destroy(self->header);
    self->header = NULL;
  }
  if (self->content) {
    self->content->destroy(self->content);
    self->content = NULL;
  }
  if (self->footer) {
    self->footer->destroy(self->footer);
    self->footer = NULL;
  }
  free(self);
}

/**
 * @brief Creates a new RootView instance with specified configurations.
 *
 * This function initializes a new RootView object with the given parent window
 * and configuration. It allocates memory for the RootView, sets up its layouts
 * (header, content, footer), and assigns function pointers for destroying and
 * updating the view.
 *
 * @param parent The parent window for the RootView.
 * @param config Configuration for the RootView, including header, content, and
 * footer layouts.
 *
 * @return A pointer to the newly created RootView instance.
 *         Exits the program with an error message if memory allocation fails.
 */
RootView *new_view(WINDOW *parent, RootViewConfig config) {
  RootView *self = (RootView *)malloc(sizeof(RootView));
  if (!self) {
    fprintf(stderr, "Cannot allocate mem for RootView\n");
    exit(-1);
  }

  self->header = new_layout(parent, config.header);
  self->content = new_layout(parent, config.content);
  self->footer = new_layout(parent, config.footer);

  self->destroy = _destroy;
  self->update = _update;
  return self;
}
