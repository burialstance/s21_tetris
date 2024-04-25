#include "brick_game/tetris/tetris.h"

#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "gui/cli/cli.h"

// theme
void set_default_theme_hanlder(Button btn) {
  (void)btn;
  Pallete *pallete = provide_pallete();
  pallete->change_theme(pallete, DEFAULT_THEME);
}

void set_light_theme_hanlder(Button btn) {
  (void)btn;
  Pallete *pallete = provide_pallete();
  pallete->change_theme(pallete, LIGHT_THEME);
}

void set_dark_theme_hanlder(Button btn) {
  (void)btn;
  Pallete *pallete = provide_pallete();
  pallete->change_theme(pallete, DARK_THEME);
}

void increase_brightness(Button btn) {
  (void)btn;
  Pallete *pallete = provide_pallete();
  pallete->increase_brightness(pallete);
}

void decrease_brightness(Button btn) {
  (void)btn;
  Pallete *pallete = provide_pallete();
  pallete->decrease_brightness(pallete);
}

void on_up_pressed(Button btn) { userInput(Up, btn.hold); }
void on_down_pressed(Button btn) { userInput(Down, btn.hold); }
void on_left_pressed(Button btn) { userInput(Left, btn.hold); }
void on_right_pressed(Button btn) { userInput(Right, btn.hold); }
void on_space_pressed(Button btn) { userInput(Action, btn.hold); }
void on_enter_pressed(Button btn) { userInput(Start, btn.hold); }
void on_esc_pressed(Button btn) { userInput(Pause, btn.hold); }
void on_q_pressed(Button btn) { userInput(Terminate, btn.hold); }

void __add_new_blocks(Button btn) {
  (void)btn;

  Tetris *tetris = provide_tetris();
  RootView *view = provide_root_view();

  static bool is_populated = FALSE;
  bool is_success = FALSE;

  if (!is_populated && (tetris->state == TETRIS_READY_STATE)) {
    tetris->repository->populate_custom(tetris->repository);
    is_populated = TRUE;
    is_success = TRUE;
  }

  wbkgd(view->content->window, COLOR_PAIR(THEME_BACKGROUND_PAIR));
  if (is_success) {
    wattron(view->content->window, COLOR_PAIR(THEME_SUCCESS_PAIR));
    mvwprintw(view->content->window, getmaxy(view->content->window) / 2,
              getmaxx(view->content->window) / 2, "%s",
              "SUCCESS ADD NEW BLOCKS");
    wattroff(view->content->window, COLOR_PAIR(THEME_SUCCESS_PAIR));

  } else {
    wattron(view->content->window, COLOR_PAIR(THEME_WARNING_PAIR));
    mvwprintw(view->content->window, getmaxy(view->content->window) / 2,
              getmaxx(view->content->window) / 2, "%s", "ALREADY POPULATED");
    mvwprintw(view->content->window, getmaxy(view->content->window) / 2 + 1,
              getmaxx(view->content->window) / 2, "%s",
              "or Tetris not in ready state");
    wattroff(view->content->window, COLOR_PAIR(THEME_WARNING_PAIR));
  }

  wrefresh(view->content->window);

  napms(1000);
}

void __add_exp(Button btn) {
  (void)btn;
  Tetris *tetris = provide_tetris();
  tetris->data.info.score += 600;
}

void configure_common_keyboard() {
  KeyboardController *kb = provide_keyboard();

  // themes
  kb->add_listener(kb, '!', set_default_theme_hanlder);  // shift + 1
  kb->add_listener(kb, '@', set_dark_theme_hanlder);     // shift + 2
  kb->add_listener(kb, '#', set_light_theme_hanlder);    // shift + 3
  kb->add_listener(kb, '[', decrease_brightness);        // [
  kb->add_listener(kb, ']', increase_brightness);        // ]
}

void configure_game_keyboard() {
  KeyboardController *kb = provide_keyboard();

  // tetris handlers
  kb->add_listener(kb, KEY_UP, on_up_pressed);
  kb->add_listener(kb, KEY_DOWN, on_down_pressed);
  kb->add_listener(kb, KEY_LEFT, on_left_pressed);
  kb->add_listener(kb, KEY_RIGHT, on_right_pressed);
  kb->add_listener(kb, '\n', on_enter_pressed);
  kb->add_listener(kb, ' ', on_space_pressed);
  kb->add_listener(kb, 'q', on_q_pressed);
  kb->add_listener(kb, 'Q', on_q_pressed);
  kb->add_listener(kb, 27, on_esc_pressed);

  kb->add_listener(kb, 'n', __add_new_blocks);
  kb->add_listener(kb, 'N', __add_new_blocks);
  kb->add_listener(kb, 'x', __add_exp);
  kb->add_listener(kb, 'X', __add_exp);
}

void header_draw_handler(Layout *self) {
  GameInfo_t model = updateCurrentState();

  wclear(self->window);
  wbkgd(self->window, COLOR_PAIR(THEME_SURFACE_PAIR));

  if (model.pause == 0) {
    insert_tetris_logo(self->window, (getmaxx(self->window) - 30) / 2, 0);
  } else if (model.pause == 1) {
    insert_pause_logo(self->window, (getmaxx(self->window) - 30) / 2, 0);
  } else if (model.pause == -1) {
    insert_gameover_logo(self->window, (getmaxx(self->window) - 54) / 2, 0);
  }
  wnoutrefresh(self->window);
}

void content_draw_handler(Layout *self) {
  if (!self) return;

  GameInfo_t model = updateCurrentState();

  wclear(self->window);
  wbkgd(self->window, COLOR_PAIR(THEME_SURFACE_PAIR));

  if (model.level >= 8) {
    fire_component(self->window,
                   (FireComponentProps){.x = 0,
                                        .y = getmaxy(self->window) - 20,
                                        .height = 20,
                                        .width = getmaxx(self->window)});
  }

  // board
  BoardComponentProps board = {
      .attrs = COLOR_PAIR(THEME_SURFACE_PAIR),
      .render_type = (model.pause == 1) ? BOARD_RENDER_TYPE_COLORLESS
                                        : BOARD_RENDER_TYPE_DEFAULT,
      .pos =
          {
              .x = (getmaxx(self->window) - 20 - 12) / 2,
              .y = (getmaxy(self->window) - 20) / 2,
          },
      .data = {
          .matrix = model.field,
      }};

  board_component(self->window, board);

  int stat_offset_x = board.pos.x + (BOARD_COMPONENT_WIDTH * 2) + 3;
  int stat_offset_y = board.pos.y + 3;
  int stat_width = 12;

  // stat
  if (TRUE) {
    // score
    counter_component(self->window,
                      (CounterComponentProps){
                          .attrs = COLOR_PAIR(THEME_SURFACE_PAIR),
                          .height = 3,
                          .width = stat_width,
                          .pos = {.x = stat_offset_x, .y = stat_offset_y},
                          .data = {.title = "score", .value = model.score},
                      });

    // highscore
    counter_component(
        self->window,
        (CounterComponentProps){
            .attrs = COLOR_PAIR(THEME_SURFACE_PAIR),
            .height = 3,
            .width = stat_width,
            .pos = {.x = stat_offset_x, .y = stat_offset_y + 4},
            .data = {.title = "high score", .value = model.high_score},
        });

    // level
    counter_component(self->window,
                      (CounterComponentProps){
                          .attrs = COLOR_PAIR(THEME_SURFACE_PAIR),
                          .height = 3,
                          .width = stat_width,
                          .pos = {.x = stat_offset_x, .y = stat_offset_y + 8},
                          .data = {.title = "level", .value = model.level},
                      });

    // next brick
    brick_component(
        self->window,
        (BrickComponentProps){
            .attrs = COLOR_PAIR(THEME_SURFACE_PAIR),
            .render_type = (model.pause == 1) ? BRICK_RENDER_TYPE_COLORLESS
                                              : BRICK_RENDER_TYPE_DEFAULT,
            .width = stat_width,
            .height = 6,
            .data = {.title = "next",
                     .width = 4,
                     .height = 4,
                     .matrix = model.next},
            .pos = {.x = stat_offset_x, .y = stat_offset_y + 12}});
  }

  insert_s21_logo(self->window,
                  getmaxx(self->window) - self->config.padding.right - 6,
                  getmaxy(self->window) - self->config.padding.bottom - 4);

  wnoutrefresh(self->window);
}

void content_motd_draw_handler(Layout *self) {
  if (!self) return;
  wclear(self->window);
  wbkgd(self->window, COLOR_PAIR(THEME_SURFACE_PAIR));

  motd_component(self->window,
                 (MotdComponentProps){
                     .attrs = COLOR_PAIR(THEME_SURFACE_PAIR),
                     .height = 21,
                     .width = 60,
                     .pos = {.x = (getmaxx(self->window) - 60) / 2, .y = 2}});

  wnoutrefresh(self->window);
}

void screen_initialize() {
  initscr();
  noecho();
  cbreak();
  nodelay(stdscr, TRUE);
  curs_set(0);
  keypad(stdscr, TRUE);
  wbkgd(stdscr, COLOR_PAIR(THEME_BACKGROUND_PAIR));
  refresh();
}

int main() {
  srand(time(NULL));
  setlocale(LC_ALL, "");

  screen_initialize();

  Pallete *pallete = provide_pallete();
  pallete->change_theme(pallete, DARK_THEME);
  KeyboardController *kb = provide_keyboard();

  RootView *root_view = provide_root_view();
  root_view->header->draw = header_draw_handler;

  // MOTD
  timeout(1000);
  configure_common_keyboard();
  root_view->content->draw = content_motd_draw_handler;
  do {
    wclear(stdscr);
    wbkgd(stdscr, COLOR_PAIR(THEME_BACKGROUND_PAIR));
    root_view->update(root_view);
  } while (kb->listen(kb).key != '\n');

  // GAME
  configure_game_keyboard();
  root_view->content->draw = content_draw_handler;
  Tetris *tetris = provide_tetris();

  timeout(1000 / 20);
  while (tetris->state != TETRIS_TERMINATED_STATE) {
    kb->listen(kb);

    wclear(stdscr);
    wbkgd(stdscr, COLOR_PAIR(THEME_BACKGROUND_PAIR));
    root_view->update(root_view);

    doupdate();
  }

  kb->destroy(kb);
  tetris->destroy(tetris);
  pallete->destroy(pallete);
  root_view->destroy(root_view);

  terminated_screen(stdscr, 4000);
  endwin();
  return 0;
}