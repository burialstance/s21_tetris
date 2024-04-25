#include "test_tetris.h"


START_TEST(tetris_fsm__ready) {
  Tetris *tetris = provide_tetris(new_brick_repository());
  tetris->repository->populate_custom(tetris->repository);

  tetris->state = TETRIS_READY_STATE;
  userInput(Start, false);
  ck_assert_int_eq(tetris->state, TETRIS_MOVING_STATE);

  tetris->state = TETRIS_READY_STATE;
  userInput(Terminate, false);
  ck_assert_int_eq(tetris->state, TETRIS_TERMINATED_STATE);

  tetris->state = TETRIS_READY_STATE;
  userInput(Left, false);
  userInput(Right, false);
  ck_assert_int_eq(tetris->state, TETRIS_READY_STATE);

  tetris->destroy(tetris);
  tetris = NULL;
}
END_TEST

START_TEST(tetris_fsm__spawn) {
  Tetris *tetris = provide_tetris(new_brick_repository());
  tetris->repository->populate_custom(tetris->repository);

  tetris->state = TETRIS_SPAWN_STATE;
  userInput(Pause, false);
  ck_assert_int_eq(tetris->state, TETRIS_PAUSE_STATE);
  userInput(Pause, false);
  ck_assert_int_eq(tetris->state, TETRIS_MOVING_STATE);

  tetris->state = TETRIS_SPAWN_STATE;
  userInput(Left, false);
  userInput(Right, false);
  ck_assert_int_eq(tetris->state, TETRIS_SPAWN_STATE);

  tetris->state = TETRIS_SPAWN_STATE;
  userInput(Terminate, false);
  ck_assert_int_eq(tetris->state, TETRIS_TERMINATED_STATE);

  tetris->destroy(tetris);
  tetris = NULL;
}
END_TEST

START_TEST(tetris_fsm__movement) {
  Tetris *tetris = provide_tetris(new_brick_repository());
  tetris->repository->populate_custom(tetris->repository);

  tetris->state = TETRIS_MOVING_STATE;

  userInput(Start, false);
  ck_assert_int_eq(tetris->state, TETRIS_MOVING_STATE);

  userInput(Left, false);
  ck_assert_int_eq(tetris->state, TETRIS_MOVING_STATE);
  userInput(Right, false);
  ck_assert_int_eq(tetris->state, TETRIS_MOVING_STATE);
  userInput(Up, false);
  ck_assert_int_eq(tetris->state, TETRIS_MOVING_STATE);
  userInput(Down, false);
  ck_assert_int_eq(tetris->state, TETRIS_MOVING_STATE);
  userInput(Action, false);
  ck_assert_int_eq(tetris->state, TETRIS_MOVING_STATE);
  userInput(Pause, false);
  ck_assert_int_eq(tetris->state, TETRIS_PAUSE_STATE);
  userInput(Pause, false);
  ck_assert_int_eq(tetris->state, TETRIS_MOVING_STATE);
  userInput(Terminate, false);
  ck_assert_int_eq(tetris->state, TETRIS_TERMINATED_STATE);

  tetris->destroy(tetris);
  tetris = NULL;
}
END_TEST

START_TEST(tetris_fsm__pause) {
  Tetris *tetris = provide_tetris(new_brick_repository());
  tetris->repository->populate_custom(tetris->repository);

  tetris->state = TETRIS_PAUSE_STATE;
  userInput(Pause, false);
  ck_assert_int_eq(tetris->state, TETRIS_PAUSE_STATE);
  userInput(Pause, false);
  ck_assert_int_eq(tetris->state, TETRIS_MOVING_STATE);

  tetris->state = TETRIS_PAUSE_STATE;
  userInput(Left, false);
  ck_assert_int_eq(tetris->state, TETRIS_PAUSE_STATE);

  tetris->state = TETRIS_PAUSE_STATE;
  userInput(Terminate, false);
  ck_assert_int_eq(tetris->state, TETRIS_TERMINATED_STATE);

  tetris->destroy(tetris);
  tetris = NULL;
}
END_TEST

START_TEST(tetris_fsm__gameover) {
  Tetris *tetris = provide_tetris(new_brick_repository());
  tetris->repository->populate_custom(tetris->repository);

  tetris->state = TETRIS_GAMEOVER_STATE;
  userInput(Start, false);
  ck_assert_int_eq(tetris->state, TETRIS_MOVING_STATE);

  tetris->state = TETRIS_GAMEOVER_STATE;
  userInput(Left, false);
  userInput(Right, false);
  ck_assert_int_eq(tetris->state, TETRIS_GAMEOVER_STATE);

  tetris->state = TETRIS_GAMEOVER_STATE;
  userInput(Terminate, false);
  ck_assert_int_eq(tetris->state, TETRIS_TERMINATED_STATE);

  tetris->destroy(tetris);
  tetris = NULL;
}
END_TEST

Suite *suite_tetris__fsm(void) {
  Suite *s = suite_create("suite_tetris__fsm");
  TCase *tc_core = tcase_create("default");
  suite_add_tcase(s, tc_core);

  // tcase_add_test(tc_core, tetris_fsm_default);
  tcase_add_test(tc_core, tetris_fsm__ready);
  tcase_add_test(tc_core, tetris_fsm__spawn);
  tcase_add_test(tc_core, tetris_fsm__movement);
  tcase_add_test(tc_core, tetris_fsm__pause);
  tcase_add_test(tc_core, tetris_fsm__gameover);

  return s;
}
