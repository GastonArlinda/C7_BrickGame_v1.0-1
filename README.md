# C7_BrickGame_v1.0-1

## Описание проекта
**C7_BrickGame_v1.0-1** – консольная версия классической игры «Тетрис», разработанная на языке C. Проект реализует игровой процесс с использованием конечного автомата и библиотеки `ncurses` для терминального интерфейса.  

## Функциональные возможности
- Вращение фигур  
- Перемещение фигур влево и вправо  
- Ускорение падения фигуры  
- Уничтожение заполненных линий  
- Показ следующей фигуры  
- Подсчет очков  
- Система уровней с увеличением скорости игры  
- Сохранение и отображение рекорда  

## Управление
- `S` – начало игры  
- `P` – пауза  
- `Q` – завершение игры  
- `←` – перемещение фигуры влево  
- `→` – перемещение фигуры вправо  
- `↓` – ускоренное падение фигуры  
- `Space` – вращение фигуры  

## Сборка и запуск
### Сборка проекта:
```sh
make
./tetris
```
### Тесты
```sh
make test
```
### Проверить покрытие
```sh
make gcov_report
```
### Сформировать doxygen документацию
```sh
make dvi
```
