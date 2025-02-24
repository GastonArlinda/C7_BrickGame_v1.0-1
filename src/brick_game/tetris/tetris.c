#include "tetris.h"

void figure_move(GameInfo_t *game_info, int key, Figure_t *figure) {
  if (key == KEY_LEFT && check_collision(game_info, figure, 0, -1)) {
    figure->x--;
  } else if (key == KEY_RIGHT && check_collision(game_info, figure, 0, 1)) {
    figure->x++;
  } else if (key == KEY_DOWN && check_collision(game_info, figure, 1, 0)) {
    figure->y++;
  } else if (key == ' ') {
    rotate_figure(game_info, figure);
  }
}

void rotate_figure(GameInfo_t *game_info, Figure_t *figure) {
  int rotated_figure[4][4];

  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      rotated_figure[y][x] = figure->shape[3 - x][y];
    }
  }

  Figure_t test_figure = *figure;
  memcpy(test_figure.shape, rotated_figure, sizeof(rotated_figure));

  if (check_collision(game_info, &test_figure, 0, 0)) {
    memcpy(figure->shape, rotated_figure, sizeof(rotated_figure));

    // Система Wall Kicks
  } else if (check_collision(game_info, &test_figure, 0, 1)) {
    figure->x += 1;
    memcpy(figure->shape, rotated_figure, sizeof(rotated_figure));
  } else if (check_collision(game_info, &test_figure, 0, -1)) {
    figure->x -= 1;
    memcpy(figure->shape, rotated_figure, sizeof(rotated_figure));
  }
}

int check_collision(GameInfo_t *game_info, Figure_t *figure, int deltaY,
                    int deltaX) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (figure->shape[y][x]) {
        int newX = figure->x + x + deltaX;
        int newY = figure->y + y + deltaY;

        if (newX < 0 || newX >= FIELD_WIDTH || newY >= FIELD_HEIGHT) {
          return 0;  // Столкновение с границами поля
        }
        if (newY >= 0 && game_info->field[newY][newX]) {
          return 0;  // Столкновение с другими блоками
        }
      }
    }
  }
  return 1;
}

int figure_attaching(GameInfo_t *game_info, Figure_t *figure) {
  if (!check_collision(game_info, figure, 1, 0)) {
    for (int y = 0; y < 4; y++) {
      for (int x = 0; x < 4; x++) {
        if (figure->shape[y][x]) {
          game_info->field[figure->y + y][figure->x + x] = figure->color;
        }
      }
    }
    return 1;
  }
  return 0;
}

bool remove_full_lines(GameInfo_t *game_info) {
  bool is_removed = 0;
  int removed_cnt = 0;
  for (int y = 0; y < FIELD_HEIGHT; y++) {
    int full_line = 1;
    for (int x = 0; x < FIELD_WIDTH; x++) {
      if (!game_info->field[y][x]) {
        full_line = 0;
        break;
      }
    }
    if (full_line) {
      is_removed = 1;
      removed_cnt++;
      for (int row = y; row > 0; row--) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
          game_info->field[row][x] = game_info->field[row - 1][x];
        }
      }
      for (int x = 0; x < FIELD_WIDTH; x++) {
        game_info->field[0][x] = 0;
      }
    }
  }
  scoring(game_info, removed_cnt);
  return is_removed;
}

void scoring(GameInfo_t *game_info, int removed_cnt) {
  if (removed_cnt == 1) {
    game_info->score += 100;
  } else if (removed_cnt == 2) {
    game_info->score += 300;
  } else if (removed_cnt == 3) {
    game_info->score += 700;
  } else if (removed_cnt == 4) {
    game_info->score += 1500;
  }
}

void hs_recorder(GameInfo_t *game_info) {
  if (game_info->score > game_info->high_score) {
    game_info->high_score = game_info->score;

    FILE *high_score = fopen("high_score.txt", "w");
    if (high_score != NULL) {
      fprintf(high_score, "%d", game_info->high_score);
      fclose(high_score);
    }
  }
}

Figure_t generate_figure() {
  Figure_t figures[] = {
      {.shape = {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
       .x = 3,
       .y = -1,
       .color = 1},
      {.shape = {{0, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
       .x = 3,
       .y = -1,
       .color = 2},
      {.shape = {{0, 0, 0, 0}, {0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
       .x = 3,
       .y = -1,
       .color = 3},
      {.shape = {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
       .x = 3,
       .y = -1,
       .color = 4},
      {.shape = {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
       .x = 3,
       .y = -1,
       .color = 5},
      {.shape = {{0, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
       .x = 4,
       .y = -1,
       .color = 6},
      {.shape = {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
       .x = 4,
       .y = -1,
       .color = 7}};
  int rand_index = rand() % 7;
  return figures[rand_index];
}

int is_game_over(GameInfo_t *game_info, Figure_t *figure) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (figure->shape[y][x] &&
          game_info->field[figure->y + y][figure->x + x]) {
        return 1;
      }
    }
  }
  return 0;
}

void InfoInit(GameInfo_t *game_info) {
  game_info->score = 0;
  game_info->high_score = 0;
  game_info->level = 1;
  game_info->speed = 1;
  game_info->pause = 0;

  game_info->field = (int **)malloc(sizeof(int *) * FIELD_HEIGHT);

  for (int i = 0; i < FIELD_HEIGHT; i++) {
    game_info->field[i] = (int *)calloc(FIELD_WIDTH, sizeof(int));
  }

  FILE *high_score = fopen("high_score.txt", "r");
  if (high_score == NULL ||
      fscanf(high_score, "%d", &game_info->high_score) == 0) {
    game_info->high_score = 0;
  }
  fclose(high_score);
}

void InfoFree(GameInfo_t *game_info) {
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    free(game_info->field[i]);
  }
  free(game_info->field);
  game_info->field = NULL;
}

GameState_t update_game_state(GameState_t current_state, UserAction_t action,
                              GameInfo_t *game_info, Figure_t *current_figure) {
  switch (current_state) {
    case State_Start:
      if (action == Start) {
        return State_Playing;
      } else if (action == Terminate) {
        return State_Terminate;
      }
      break;

    case State_Pause:
      if (action == Pause) {
        return State_Playing;
      }
      break;

    case State_Playing:
      switch (action) {
        case Left:
          figure_move(game_info, KEY_LEFT, current_figure);
          break;
        case Right:
          figure_move(game_info, KEY_RIGHT, current_figure);
          break;
        case Down:
          figure_move(game_info, KEY_DOWN, current_figure);
          break;
        case Action:
          rotate_figure(game_info, current_figure);
          break;
        case Pause:
          return State_Pause;
        case Terminate:
          return State_Terminate;
        default:
          break;
      }
      if (is_game_over(game_info, current_figure)) {
        return State_GameOver;
      }
      break;

    case State_Terminate:
      break;

    case State_GameOver:  // Перезапуск игры
      if (action == Start) {
        InfoFree(game_info);
        InfoInit(game_info);
        return State_Start;
      } else if (action == Terminate) {
        return State_Terminate;
      }
      break;
  }

  return current_state;
}

UserAction_t get_action(int ch, bool hold) {
  UserAction_t action;
  if (hold) {
  } else {
    switch (ch) {
      case KEY_LEFT:
        action = Left;
        break;
      case KEY_RIGHT:
        action = Right;
        break;
      case KEY_DOWN:
        action = Down;
        break;
      case ' ':
        action = Action;
        break;
      case 'p':
        action = Pause;
        break;
      case 'q':
        action = Terminate;
        break;
      case 's':
        action = Start;
        break;
      default:
        action = Up;
    }
  }
  return action;
}

void speed_change(GameInfo_t *game_info) {
  if (game_info->score / 600 > 9) {
    game_info->level = 10;
  } else {
    game_info->level = game_info->score / 600 + 1;
  }
  game_info->speed = game_info->level;
}