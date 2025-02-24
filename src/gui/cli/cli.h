#ifndef GUI_CLI_H
#define GUI_CLI_H

#include "../../brick_game/tetris/tetris.h"

/**
 * @brief Отображает фигуру на игровом поле.
 *
 * @param figure Указатель на текущую фигуру.
 */
void draw_figure(Figure_t *figure);

/**
 * @brief Отображает игровое поле.
 *
 * @param game_info Указатель на структуру с информацией об игре.
 */
void draw_field(GameInfo_t *game_info);

/**
 * @brief Отображает панель информации, включая счёт и следующую фигуру.
 *
 * @param game_info Указатель на структуру с информацией об игре.
 * @param next_figure Указатель на следующую фигуру.
 */
void draw_info_panel(GameInfo_t *game_info, Figure_t *next_figure);

/**
 * @brief Отображает следующую фигуру на панели информации.
 *
 * @param next_figure Указатель на следующую фигуру.
 */
void draw_next_figure(Figure_t *next_figure);

/**
 * @brief Отображает границы игрового интерфейса.
 */
void draw_border();

/**
 * @brief Рисует прямоугольник в указанных координатах.
 *
 * @param top_y Верхняя граница.
 * @param bottom_y Нижняя граница.
 * @param left_x Левая граница.
 * @param right_x Правая граница.
 */
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);

/**
 * @brief Инициализирует параметры игры и ncurses.
 *
 * @param game_info Указатель на структуру с информацией об игре.
 */
void initialize_game(GameInfo_t *game_info);

/**
 * @brief Отображает текущий экран в зависимости от состояния игры.
 *
 * @param state Текущее состояние игры.
 * @param game_info Указатель на структуру с информацией об игре.
 * @param current_figure Указатель на текущую фигуру.
 * @param next_figure Указатель на следующую фигуру.
 */
void render_screen(GameState_t state, GameInfo_t *game_info,
                   Figure_t *current_figure, Figure_t *next_figure);

/**
 * @brief Основной игровой цикл.
 *
 * @param game_info Указатель на структуру с информацией об игре.
 * @param current_state Указатель на текущее состояние игры.
 */
void game_loop(GameInfo_t *game_info, GameState_t *current_state);

#endif