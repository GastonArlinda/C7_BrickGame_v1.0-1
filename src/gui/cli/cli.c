#include "cli.h"

void draw_figure(Figure_t *figure) {
  attron(COLOR_PAIR(figure->color));
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (figure->shape[y][x] && figure->y + y >= 0 && figure->x + x >= 0 &&
          figure->x + x < FIELD_WIDTH) {
        mvaddch(figure->y + y + 1, (figure->x + x) * 2 + 1, ' ' | A_REVERSE);
        mvaddch(figure->y + y + 1, (figure->x + x) * 2 + 2, ' ' | A_REVERSE);
      }
    }
  }
  attroff(COLOR_PAIR(figure->color));
}

void draw_field(GameInfo_t *game_info) {
  for (int y = 0; y < FIELD_HEIGHT; y++) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
      if (game_info->field[y][x]) {
        attron(COLOR_PAIR(game_info->field[y][x]));
        mvaddch(y + 1, x * 2 + 1, ' ' | A_REVERSE);
        mvaddch(y + 1, x * 2 + 2, ' ' | A_REVERSE);
        attroff(COLOR_PAIR(game_info->field[y][x]));
      } else {
        attron(A_DIM);
        mvprintw(y + 1, x * 2 + 1, ".");
        mvprintw(y + 1, x * 2 + 2, ".");
        attroff(A_DIM);
      }
    }
  }
}

void draw_info_panel(GameInfo_t *game_info, Figure_t *next_figure) {
  init_pair(8, COLOR_WHITE, COLOR_BLACK);
  mvprintw(1, FIELD_WIDTH * 2 + 4, "SCORE:");
  mvprintw(2, FIELD_WIDTH * 2 + 4, "%d", game_info->score);

  mvprintw(4, FIELD_WIDTH * 2 + 4, "HIGH SCORE:");
  mvprintw(5, FIELD_WIDTH * 2 + 4, "%d", game_info->high_score);

  mvprintw(7, FIELD_WIDTH * 2 + 4, "LEVEL:");
  mvprintw(8, FIELD_WIDTH * 2 + 4, "%d", game_info->level);

  mvprintw(10, FIELD_WIDTH * 2 + 4, "NEXT:");
  draw_next_figure(next_figure);

  mvprintw(16, FIELD_WIDTH * 2 + 4, "SPACE - Rotate");
  mvprintw(18, FIELD_WIDTH * 2 + 4, "  P   - Pause");
  mvprintw(20, FIELD_WIDTH * 2 + 4, "  Q   - Exit");
}

void draw_next_figure(Figure_t *next_figure) {
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (next_figure->shape[y][x]) {
        attron(COLOR_PAIR(next_figure->color));
        mvaddch(11 + y, (FIELD_WIDTH + x) * 2 + 8, ' ' | A_REVERSE);
        mvaddch(11 + y, (FIELD_WIDTH + x) * 2 + 9, ' ' | A_REVERSE);
        attroff(COLOR_PAIR(next_figure->color));
      }
    }
  }
}

void draw_border() {
  print_rectangle(0, 21, 0, 21);  // Прямоугольник поля (стакана)
  print_rectangle(0, 9, 22, 40);  // Прямоугольник игровой статистики
  print_rectangle(10, 14, 22, 40);  // Пярмоугольник следующей фигуры
  print_rectangle(15, 21, 22, 40);  // Прямоугольник с информацией об управлении
}

void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  for (int y = top_y; y < bottom_y; y++) {  // Вертикальные линнии
    mvaddch(y, left_x, ACS_VLINE);
    mvaddch(y, right_x, ACS_VLINE);
  }
  for (int x = left_x; x < right_x; x++) {  // Горизонтальные линии
    mvaddch(top_y, x, ACS_HLINE);
    mvaddch(bottom_y, x, ACS_HLINE);
  }

  mvaddch(top_y, left_x, ACS_ULCORNER);   // Верхний левый угол
  mvaddch(top_y, right_x, ACS_URCORNER);  // Верхний правый угол
  mvaddch(bottom_y, left_x, ACS_LLCORNER);  // Нижний левый угол
  mvaddch(bottom_y, right_x, ACS_LRCORNER);  // Нижний правый угол
}

void initialize_game(GameInfo_t *game_info) {
  srand(time(NULL));
  InfoInit(game_info);

  initscr();
  start_color();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  timeout(60);

  init_color(8, 1000, 0, 0);       // Красный
  init_color(9, 1000, 500, 0);     // Оранжевый
  init_color(10, 1000, 1000, 0);   // Желтый
  init_color(11, 1000, 592, 733);  // Розовый
  init_color(12, 298, 730, 90);    // Зелёный
  init_color(13, 0, 0, 1000);      // Синий
  init_color(14, 690, 407, 930);   // Фиолетовый

  // Создаем цветовые пары для фигур
  for (int i = 1; i <= 7; i++) {
    init_pair(i, 7 + i, COLOR_BLACK);
  }
}

void render_screen(GameState_t state, GameInfo_t *game_info,
                   Figure_t *current_figure, Figure_t *next_figure) {
  clear();
  if (state == State_Start) {
    mvprintw(FIELD_HEIGHT / 2 - 3, 6, "WELCOME TO");
    mvprintw(FIELD_HEIGHT / 2 - 2, 6, "THE TETRIS");
    mvprintw(FIELD_HEIGHT / 2 + 2, 2, "TYPE [S], to START");
    mvprintw(FIELD_HEIGHT / 2 + 4, 5, "[Q], to QUIT");
    draw_border();
  } else if (state == State_Playing) {
    draw_border();
    draw_field(game_info);
    draw_figure(current_figure);
    draw_info_panel(game_info, next_figure);
  } else if (state == State_GameOver) {
    draw_border();
    draw_field(game_info);
    draw_info_panel(game_info, next_figure);
    mvprintw(FIELD_HEIGHT / 2, 6, "GAME OVER!");
    mvprintw(FIELD_HEIGHT / 2 + 1, 1, "TYPE [S], to RESTART");
    mvprintw(FIELD_HEIGHT / 2 + 2, 5, "[Q], to QUIT");
  } else if (state == State_Pause) {
    draw_border();
    draw_field(game_info);
    draw_figure(current_figure);
    draw_info_panel(game_info, next_figure);
    mvprintw(FIELD_HEIGHT / 2, 5, "GAME");
    mvprintw(FIELD_HEIGHT / 2, 11, "PAUSED");
    mvprintw(FIELD_HEIGHT / 2 + 1, 1, "PRESS [P], to RESUME");
  }
  refresh();
}

void game_loop(GameInfo_t *game_info, GameState_t *current_state) {
  Figure_t current_figure = generate_figure();
  Figure_t next_figure = generate_figure();
  clock_t clock1 = clock();

  while (*current_state != State_Terminate) {
    render_screen(*current_state, game_info, &current_figure, &next_figure);

    int ch = getch();
    UserAction_t action = get_action(ch, FALSE);
    *current_state =
        update_game_state(*current_state, action, game_info, &current_figure);

    clock_t clock2 = clock();
    if (*current_state == State_Playing &&
        (clock2 - clock1 >= 10000 - (game_info->speed * 600))) {
      if (figure_attaching(game_info, &current_figure)) {
        if (remove_full_lines(game_info)) {
          speed_change(game_info);
          hs_recorder(game_info);
        }
        current_figure = next_figure;
        next_figure = generate_figure();
      }
      current_figure.y++;
      clock1 = clock2;
    }
  }
}