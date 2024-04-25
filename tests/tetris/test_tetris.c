#include "test_tetris.h"

START_TEST(tetris_default_case) {
  Tetris *tetris = new_tetris(new_brick_repository());
  tetris->repository->populate_custom(tetris->repository);

  ck_assert_ptr_nonnull(tetris);
  ck_assert_ptr_null(tetris->data.current_brick);
  ck_assert_ptr_null(tetris->data.next_brick);
  ck_assert_ptr_nonnull(tetris->data.info.field);
  ck_assert_ptr_nonnull(tetris->data.info.next);
  ck_assert_int_eq(tetris->data.info.pause, 0);
  ck_assert_int_eq(tetris->data.info.level, 1);
  ck_assert_int_eq(tetris->state, TETRIS_READY_STATE);

  tetris->down(NULL, false);
  tetris->up(NULL, false);
  tetris->left(NULL, false);
  tetris->right(NULL, false);
  tetris->action(NULL, false);

  tetris->start(tetris);
  ck_assert_int_eq(tetris->state, TETRIS_MOVING_STATE);
  ck_assert_ptr_nonnull(tetris->data.current_brick);
  ck_assert_ptr_nonnull(tetris->data.next_brick);

  tetris->pause(tetris);
  ck_assert_int_eq(tetris->data.info.pause, 1);
  tetris->pause(tetris);
  ck_assert_int_eq(tetris->data.info.pause, 0);

  tetris->down(tetris, true);
  ck_assert_int_eq(tetris->state, TETRIS_ATTACH_STATE);

  tetris->_tick(tetris);
  ck_assert_int_eq(tetris->state, TETRIS_MOVING_STATE);

  for (size_t i = 0; i < 10; i++) {
    for (size_t j = 0; j < 10; j++) {
      tetris->down(tetris, false);
    }
    tetris->_tick(tetris);
  }
  ck_assert_int_eq(tetris->state, TETRIS_GAMEOVER_STATE);

  tetris->start(tetris);
  ck_assert_int_eq(tetris->state, TETRIS_MOVING_STATE);

  tetris->destroy(tetris);
  tetris = NULL;
}
END_TEST

START_TEST(tetris_timer_is_ticked) {
  Tetris *tetris = new_tetris(new_brick_repository());
  tetris->repository->populate_custom(tetris->repository);

  tetris->start(tetris);
  int last_y = tetris->data.current_brick->pos.y;
  ck_assert_int_eq(tetris->state, TETRIS_MOVING_STATE);

  sleep(1);
  tetris->_tick(tetris);
  ck_assert_int_eq(tetris->data.current_brick->pos.y, last_y + 1);

  tetris->destroy(tetris);
  tetris = NULL;
}
END_TEST

START_TEST(tetris_movement) {
  Tetris *tetris = new_tetris(new_brick_repository());
  tetris->repository->populate_custom(tetris->repository);

  tetris->start(tetris);
  ck_assert_int_eq(tetris->state, TETRIS_MOVING_STATE);
  for (size_t left_btn_presser = 0; left_btn_presser < 10; left_btn_presser++) {
    tetris->left(tetris, false);
  }
  for (size_t right_btn_presser = 0; right_btn_presser < 10;
       right_btn_presser++) {
    tetris->right(tetris, false);
  }

  for (size_t down_btn_presser = 0; down_btn_presser < 10; down_btn_presser++) {
    tetris->down(tetris, false);
  }

  tetris->terminate(tetris);
  ck_assert_int_eq(tetris->state, TETRIS_TERMINATED_STATE);

  tetris->destroy(tetris);
}
END_TEST

START_TEST(tetris_action) {
  Tetris *tetris = new_tetris(new_brick_repository());
  tetris->repository->populate_custom(tetris->repository);

  tetris->start(tetris);
  ck_assert_int_eq(tetris->state, TETRIS_MOVING_STATE);

  tetris->_tick(tetris);
  ck_assert_int_gt(tetris->data.current_brick->total_states,
                   1);  // otherwise test not have statement

  int last_brick_state = tetris->data.current_brick->state;
  tetris->action(tetris, false);
  ck_assert_int_ne(tetris->data.current_brick->state, last_brick_state);

  tetris->destroy(tetris);
  tetris = NULL;
}
END_TEST

START_TEST(tetris_erease_lines) {
  Tetris *tetris = new_tetris(new_brick_repository());
  tetris->repository->populate_custom(tetris->repository);

  tetris->start(tetris);
  int score = tetris->data.info.score;
  ck_assert_int_eq(score, 0);

  for (size_t i = 0; i < TETRIS_FIELD_WIDTH; i++) {
    tetris->data.info.field[TETRIS_FIELD_HEIGHT - 1][i] = 1;
  }
  tetris->down(tetris, true);
  tetris->_tick(tetris);
  ck_assert_int_gt(tetris->data.info.score, score);

  tetris->destroy(tetris);
  tetris = NULL;
}
END_TEST

START_TEST(tetris_current_state) {
  Tetris *tetris = new_tetris(new_brick_repository());
  tetris->repository->populate_custom(tetris->repository);

  tetris->start(tetris);
  GameInfo_t info = updateCurrentState();

  // for (size_t row = 0; row < TETRIS_FIELD_HEIGHT; row++){
  //   for (size_t col = 0; col < TETRIS_FIELD_WIDTH; col++){
  //     ck_assert_int_eq(info.field[row][col],
  //     tetris->data.info.field[row][col]);
  //   }
  // }

  // assert dto have colorized matrix
  for (size_t row = 0; row < BRICK_HEIGHT; row++) {
    for (size_t col = 0; col < BRICK_WIDTH; col++) {
      if (info.next[row][col])
        ck_assert_int_eq(tetris->data.next_brick
                             ->states[tetris->data.next_brick->state][row][col],
                         tetris->data.next_brick->color);
    }
  }

  tetris->destroy(tetris);
}
END_TEST

START_TEST(tetris_reward) {
  ck_assert_int_eq(get_reward_count(1), 100);
  ck_assert_int_eq(get_reward_count(2), 300);
  ck_assert_int_eq(get_reward_count(3), 700);
  ck_assert_int_eq(get_reward_count(4), 1500);
  ck_assert_int_eq(get_reward_count(5), 1500);
  ck_assert_int_eq(get_reward_count(100), 1500);
}
END_TEST

START_TEST(tetris_leveling) {
  ck_assert_int_eq(get_level_by_score(600), 1);
  ck_assert_int_eq(get_level_by_score(1200), 2);
  ck_assert_int_eq(get_level_by_score(2400), 4);
  ck_assert_int_eq(get_level_by_score(4800), 8);
  ck_assert_int_eq(get_level_by_score(6000), 10);

  ck_assert_int_eq(get_level_by_score(12000), 10);
}
END_TEST

Suite *suite_tetris(void) {
  Suite *s = suite_create("tetris");
  TCase *tc_core = tcase_create("default");
  suite_add_tcase(s, tc_core);

  tcase_add_test(tc_core, tetris_default_case);
  tcase_add_test(tc_core, tetris_timer_is_ticked);
  tcase_add_test(tc_core, tetris_movement);
  tcase_add_test(tc_core, tetris_action);
  tcase_add_test(tc_core, tetris_erease_lines);
  tcase_add_test(tc_core, tetris_current_state);

  tcase_add_test(tc_core, tetris_reward);
  tcase_add_test(tc_core, tetris_leveling);

  return s;
}
