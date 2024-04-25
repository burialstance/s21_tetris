#ifndef TESTS_TETRIS_TEST_TETRIS_H
#define TESTS_TETRIS_TEST_TETRIS_H

#include <check.h>
#include <stdio.h>
#include <unistd.h>

#include "../../src/brick_game/tetris/tetris.h"

Suite *suite_tetris(void);
Suite *suite_tetris__fsm(void);
Suite *suite_tetris__repository(void);

#endif // !TESTS_TETRIS_TEST_TETRIS_H