#include "test_tetris.h"

START_TEST(repository_default_lifecicle) {
  TetrisBrickRepository *repo = new_brick_repository();

  ck_assert_ptr_nonnull(repo);
  ck_assert_ptr_nonnull(repo->create);
  ck_assert_ptr_nonnull(repo->destroy);
  ck_assert_ptr_nonnull(repo->get);
  ck_assert_ptr_nonnull(repo->get_random);

  ck_assert_ptr_null(repo->items);
  ck_assert_int_eq(repo->items_count, 0);

  ck_assert_ptr_null(repo->get(NULL, 0));
  ck_assert_ptr_null(repo->get_random(NULL));

  repo->create(NULL, (Brick){});
  repo->get(NULL, 0);
  repo->get_random(NULL);
  repo->destroy(NULL);

  repo->destroy(repo);
  repo = NULL;
}
END_TEST

START_TEST(repository_provider) {
  TetrisBrickRepository *repo = provide_brick_repository();
  TetrisBrickRepository *provided = provide_brick_repository();

  ck_assert_ptr_eq(repo, provided);

  repo->destroy(repo);
  repo = NULL;
  provided = NULL;
}
END_TEST

START_TEST(repository_brick) {
  TetrisBrickRepository *repo = provide_brick_repository();
  ck_assert_int_ge(repo->items_count, 0);

  Brick *brick = repo->get_random(repo);
  brick->next_state(NULL);
  brick->prev_state(NULL);

  for (size_t i = 0; i < 25; i++) {
    brick = repo->get_random(repo);
  }

  ck_assert_ptr_nonnull(brick);
  for (int _next = 0; _next < brick->total_states; _next++) {
    brick->next_state(brick);
  }
  ck_assert_int_eq(brick->state, 0);

  for (int _prev = 0; _prev < brick->total_states; _prev++) {
    brick->prev_state(brick);
  }
  ck_assert_int_eq(brick->state, 0);

  repo->destroy(repo);
  repo = NULL;
}
END_TEST

Suite *suite_tetris__repository(void) {
  Suite *s = suite_create("tetris__repository");
  TCase *tc_core = tcase_create("default");
  suite_add_tcase(s, tc_core);

  tcase_add_test(tc_core, repository_default_lifecicle);
  tcase_add_test(tc_core, repository_provider);
  tcase_add_test(tc_core, repository_brick);

  return s;
}
