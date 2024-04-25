#ifndef CLI_VIEWS_VIEWS_H
#define CLI_VIEWS_VIEWS_H

#include <string.h>
#include <time.h>

#include "../components/components.h"
#include "../layouts/layouts.h"
#include "../theme/theme.h"
#include "../utils/utils.h"

/**
 * @brief Configuration structure for RootView.
 *
 * This structure defines the configuration for the RootView, including the
 * configurations for the header, content, and footer layouts. Each layout
 * configuration specifies dimensions, padding, and other properties that
 * determine how each part of the RootView is displayed.
 *
 * @struct RootViewConfig
 * @field header Configuration for the header layout.
 * @field content Configuration for the content layout.
 * @field footer Configuration for the footer layout.
 */
typedef struct {
  LayoutConfig header;
  LayoutConfig content;
  LayoutConfig footer;
} RootViewConfig;

/**
 * @brief Structure representing the RootView.
 *
 * This structure encapsulates the RootView, which consists of three main
 * components: header, content, and footer. Each component is represented by a
 * pointer to a Layout structure. The structure also includes function pointers
 * for updating and destroying the RootView, allowing for dynamic behavior and
 * resource management.
 *
 * @struct __view
 * @var Layout *header
 *      Pointer to the Layout instance representing the header.
 * @var Layout *content
 *      Pointer to the Layout instance representing the content.
 * @var Layout *footer
 *      Pointer to the Layout instance representing the footer.
 * @var void (*update)(struct __view *self)
 *      Function pointer for updating the RootView.
 * @var void (*destroy)(struct __view *self)
 *      Function pointer for destroying the RootView.
 */
typedef struct __view {
  Layout *header;
  Layout *content;
  Layout *footer;

  void (*update)(struct __view *self);
  void (*destroy)(struct __view *self);
} RootView;

RootView *new_view(WINDOW *parent, RootViewConfig config);
RootView *provide_root_view();

#endif  // !CLI_VIEWS_VIEWS_H