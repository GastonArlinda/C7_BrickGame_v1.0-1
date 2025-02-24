#include "tetris_test.h"

START_TEST(test_figure_move) {
  GameInfo_t game_info;
  InfoInit(&game_info);
  Figure_t figure = {
      .x = 5,
      .y = 5,
      .shape = {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}};

  figure_move(&game_info, KEY_LEFT, &figure);
  ck_assert_int_eq(figure.x, 4);

  figure_move(&game_info, KEY_RIGHT, &figure);
  ck_assert_int_eq(figure.x, 5);

  figure_move(&game_info, KEY_DOWN, &figure);
  ck_assert_int_eq(figure.y, 6);

  figure_move(&game_info, ' ', &figure);
  ck_assert_int_eq(figure.shape[0][2], 1);
  ck_assert_int_eq(figure.shape[2][2], 1);

  InfoFree(&game_info);
}
END_TEST

START_TEST(test_remove_full_lines) {
  GameInfo_t game_info;
  InfoInit(&game_info);

  for (int y = 16; y < FIELD_HEIGHT; y++) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
      game_info.field[y][x] = 1;
    }
  }

  bool removed = remove_full_lines(&game_info);
  ck_assert(removed);
  for (int x = 0; x < FIELD_WIDTH; x++) {
    ck_assert_int_eq(game_info.field[0][x], 0);
  }

  InfoFree(&game_info);
}
END_TEST

START_TEST(test_check_collision) {
  GameInfo_t game_info;
  InfoInit(&game_info);

  Figure_t figure = {
      .x = 0,
      .y = 0,
      .shape = {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}};
  ck_assert(check_collision(&game_info, &figure, 0, 0));

  figure.x = -1;
  ck_assert(!check_collision(&game_info, &figure, 0, 0));

  InfoFree(&game_info);
}
END_TEST

START_TEST(test_figure_attaching) {
  GameInfo_t game_info;
  InfoInit(&game_info);

  Figure_t figure = {
      .x = 3,
      .y = 4,
      .color = 1,
      .shape = {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}};

  figure_attaching(&game_info, &figure);

  for (int x = 0; x < FIELD_WIDTH; x++) {
    game_info.field[6][x] = 1;
  }

  figure_attaching(&game_info, &figure);
  ck_assert_int_eq(game_info.field[5][3], 1);

  InfoFree(&game_info);
}
END_TEST

START_TEST(test_wall_kick_rotation) {
  GameInfo_t game_info;
  InfoInit(&game_info);

  Figure_t figure_left = {
      .x = -1,
      .y = 5,
      .shape = {{0, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}};
  rotate_figure(&game_info, &figure_left);
  rotate_figure(&game_info, &figure_left);
  rotate_figure(&game_info, &figure_left);

  Figure_t figure_right = {
      .x = 7,
      .y = -1,
      .shape = {{0, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}}};
  rotate_figure(&game_info, &figure_right);
  rotate_figure(&game_info, &figure_right);

  InfoFree(&game_info);
}
END_TEST

START_TEST(test_is_game_over) {
  GameInfo_t game_info;
  InfoInit(&game_info);

  GameState_t state = State_Playing;

  Figure_t figure = {
      .x = 4,
      .y = -1,
      .shape = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}};

  for (int x = 0; x < FIELD_WIDTH; x++) {
    game_info.field[0][x] = 1;
  }

  ck_assert_int_eq(is_game_over(&game_info, &figure), 1);

  state = update_game_state(state, Up, &game_info, &figure);
  ck_assert_int_eq(state, State_GameOver);

  InfoFree(&game_info);
}
END_TEST

START_TEST(test_scoring) {
  GameInfo_t game_info;
  InfoInit(&game_info);

  scoring(&game_info, 1);
  ck_assert_int_eq(game_info.score, 100);

  scoring(&game_info, 2);
  ck_assert_int_eq(game_info.score, 400);

  scoring(&game_info, 3);
  ck_assert_int_eq(game_info.score, 1100);

  scoring(&game_info, 4);
  ck_assert_int_eq(game_info.score, 2600);

  InfoFree(&game_info);
}
END_TEST

START_TEST(test_speed_change) {
  GameInfo_t game_info;
  InfoInit(&game_info);

  game_info.score = 1200;
  speed_change(&game_info);
  ck_assert_int_eq(game_info.level, 3);
  ck_assert_int_eq(game_info.speed, 3);

  game_info.score = 10000;
  speed_change(&game_info);
  ck_assert_int_eq(game_info.level, 10);
  ck_assert_int_eq(game_info.speed, 10);

  InfoFree(&game_info);
}
END_TEST

START_TEST(test_hs_recorder) {
  GameInfo_t game_info;
  InfoInit(&game_info);

  game_info.score = 600;
  game_info.high_score = 100;
  hs_recorder(&game_info);

  InfoFree(&game_info);
}
END_TEST

START_TEST(test_update_game_state) {
  GameInfo_t game_info;
  InfoInit(&game_info);

  Figure_t figure = generate_figure();
  GameState_t state = State_Start;

  state = update_game_state(state, Up, &game_info, &figure);
  ck_assert_int_eq(state, State_Start);

  state = update_game_state(state, Start, &game_info, &figure);
  ck_assert_int_eq(state, State_Playing);

  state = State_Start;
  state = update_game_state(state, Terminate, &game_info, &figure);
  ck_assert_int_eq(state, State_Terminate);

  state = State_Playing;

  state = update_game_state(state, Left, &game_info, &figure);
  ck_assert_int_eq(state, State_Playing);

  state = update_game_state(state, Right, &game_info, &figure);
  ck_assert_int_eq(state, State_Playing);

  state = update_game_state(state, Down, &game_info, &figure);
  ck_assert_int_eq(state, State_Playing);

  state = update_game_state(state, Action, &game_info, &figure);
  ck_assert_int_eq(state, State_Playing);

  state = update_game_state(state, Up, &game_info, &figure);
  ck_assert_int_eq(state, State_Playing);

  state = update_game_state(state, Terminate, &game_info, &figure);
  ck_assert_int_eq(state, State_Terminate);

  state = update_game_state(state, Up, &game_info, &figure);
  ck_assert_int_eq(state, State_Terminate);

  state = State_Playing;
  state = update_game_state(state, Pause, &game_info, &figure);
  ck_assert_int_eq(state, State_Pause);

  state = update_game_state(state, Up, &game_info, &figure);
  ck_assert_int_eq(state, State_Pause);

  state = update_game_state(state, Pause, &game_info, &figure);
  ck_assert_int_eq(state, State_Playing);

  state = State_GameOver;

  state = update_game_state(state, Up, &game_info, &figure);
  ck_assert_int_eq(state, State_GameOver);

  state = update_game_state(state, Start, &game_info, &figure);
  ck_assert_int_eq(state, State_Start);

  state = State_GameOver;
  state = update_game_state(state, Terminate, &game_info, &figure);
  ck_assert_int_eq(state, State_Terminate);

  InfoFree(&game_info);
}
END_TEST

START_TEST(test_get_action) {
  ck_assert_int_eq(get_action(KEY_LEFT, false), Left);
  ck_assert_int_eq(get_action(KEY_RIGHT, false), Right);
  ck_assert_int_eq(get_action(KEY_DOWN, false), Down);
  ck_assert_int_eq(get_action(' ', false), Action);
  ck_assert_int_eq(get_action('p', false), Pause);
  ck_assert_int_eq(get_action('q', false), Terminate);
  ck_assert_int_eq(get_action('s', false), Start);
  ck_assert_int_eq(get_action('x', false), Up);  // Default case
}
END_TEST

Suite *suite_tetris_base(void) {
  Suite *s = suite_create("Tetris");

  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_figure_move);
  tcase_add_test(tc_core, test_remove_full_lines);
  tcase_add_test(tc_core, test_check_collision);
  tcase_add_test(tc_core, test_figure_attaching);
  tcase_add_test(tc_core, test_wall_kick_rotation);
  tcase_add_test(tc_core, test_is_game_over);
  tcase_add_test(tc_core, test_scoring);
  tcase_add_test(tc_core, test_speed_change);
  tcase_add_test(tc_core, test_hs_recorder);
  tcase_add_test(tc_core, test_update_game_state);
  tcase_add_test(tc_core, test_get_action);

  suite_add_tcase(s, tc_core);

  return s;
}