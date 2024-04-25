#include "tests.h"

int main(void) {
  Suite *cases[] = {
      suite_tetris(),
      suite_tetris__fsm(),
      suite_tetris__repository(),
  };

  for (size_t i = 0; i < (sizeof(cases) / sizeof(Suite *)); i++) {
    SRunner *runner = srunner_create(cases[i]);
    srunner_run_all(runner, CK_NORMAL);
    srunner_free(runner);
  }

  return 0;
}
