#ifndef S21_TETRIS_TEST_H_
#define S21_TETRIS_TEST_H_

#include <check.h>
#include <stdio.h>

#include "../brick_game/tetris/tetris.h"
#include "../gui/cli/cli.h"

Suite *suite_tetris_base(void);

void run_tests(void);
void run_testcase(Suite *testcase);

#endif