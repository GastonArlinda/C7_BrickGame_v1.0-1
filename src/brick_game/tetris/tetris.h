#ifndef S21_TETRIS_H
#define S21_TETRIS_H

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SCORE_FILE "./score"

#define FIELD_HEIGHT 20
#define FIELD_WIDTH 10

/**
 * @brief Перечисление, описывающее возможные состояния игры.
 */
typedef enum {
  State_Start,
  State_Pause,
  State_Playing,
  State_GameOver,
  State_Terminate
} GameState_t;

/**
 * @brief Перечисление действий пользователя.
 */
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

/**
 * @brief Структура для хранения информации об игровом состоянии.
 */
typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

/**
 * @brief Структура, описывающая игровую фигуру.
 */
typedef struct {
  int shape[4][4];
  int x, y;
  int color;
} Figure_t;

/**
 * @brief Управляет движением фигуры.
 *
 * @param game_info Указатель на структуру с информацией об игре.
 * @param key Код клавиши, управляющий движением.
 * @param figure Указатель на текущую фигуру.
 */
void figure_move(GameInfo_t *game_info, int key, Figure_t *figure);

/**
 * @brief Поворачивает текущую фигуру.
 *
 * @param game_info Указатель на структуру с информацией об игре.
 * @param figure Указатель на текущую фигуру.
 */
void rotate_figure(GameInfo_t *game_info, Figure_t *figure);

/**
 * @brief Проверяет столкновение фигуры с границами поля или другими блоками.
 *
 * @param game_info Указатель на структуру с информацией об игре.
 * @param figure Указатель на текущую фигуру.
 * @param deltaY Изменение по оси Y.
 * @param deltaX Изменение по оси X.
 * @return 1, если столкновения нет, 0 в противном случае.
 */
int check_collision(GameInfo_t *game_info, Figure_t *figure, int deltaY,
                    int deltaX);

/**
 * @brief Присоединяет фигуру к игровому полю, если она достигла нижней границы.
 *
 * @param game_info Указатель на структуру с информацией об игре.
 * @param figure Указатель на текущую фигуру.
 * @return 1, если фигура прикреплена, 0 в противном случае.
 */
int figure_attaching(GameInfo_t *game_info, Figure_t *figure);

/**
 * @brief Удаляет заполненные строки с поля.
 *
 * @param game_info Указатель на структуру с информацией об игре.
 * @return true, если строки были удалены, false в противном случае.
 */
bool remove_full_lines(GameInfo_t *game_info);

/**
 * @brief Рассчитывает количество очков на основе удалённых строк.
 *
 * @param game_info Указатель на структуру с информацией об игре.
 * @param removed_cnt Количество удалённых строк.
 */
void scoring(GameInfo_t *game_info, int removed_cnt);

/**
 * @brief Записывает рекордный счёт в файл.
 *
 * @param game_info Указатель на структуру с информацией об игре.
 */
void hs_recorder(GameInfo_t *game_info);

/**
 * @brief Генерирует новую фигуру.
 *
 * @return Сгенерированная фигура.
 */
Figure_t generate_figure();

/**
 * @brief Проверяет, окончена ли игра.
 *
 * @param game_info Указатель на структуру с информацией об игре.
 * @param figure Указатель на текущую фигуру.
 * @return 1, если игра окончена, 0 в противном случае.
 */
int is_game_over(GameInfo_t *game_info, Figure_t *figure);

/**
 * @brief Инициализирует структуру GameInfo_t.
 *
 * @param game_info Указатель на структуру с информацией об игре.
 */
void InfoInit(GameInfo_t *game_info);

/**
 * @brief Освобождает ресурсы, связанные с игровым состоянием.
 *
 * @param game_info Указатель на структуру с информацией об игре.
 */
void InfoFree(GameInfo_t *game_info);

/**
 * @brief Обновляет состояние игры в зависимости от действий пользователя.
 *
 * @param current_state Текущее состояние игры.
 * @param action Действие пользователя.
 * @param game_info Указатель на структуру с информацией об игре.
 * @param current_figure Указатель на текущую фигуру.
 * @return Обновлённое состояние игры.
 */
GameState_t update_game_state(GameState_t current_state, UserAction_t action,
                              GameInfo_t *game_info, Figure_t *current_figure);

/**
 * @brief Определяет действие пользователя на основе нажатой клавиши.
 *
 * @param ch Код нажатой клавиши.
 * @param hold Признак удержания клавиши.
 * @return Действие пользователя.
 */
UserAction_t get_action(int ch, bool hold);

/**
 * @brief Изменяет скорость игры в зависимости от текущего счёта.
 *
 * @param game_info Указатель на структуру с информацией об игре.
 */
void speed_change(GameInfo_t *game_info);

#endif