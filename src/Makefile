CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

SRCS = brick_game/tetris/tetris.c gui/cli/cli.c main.c
OBJS = $(SRCS:.c=.o)
TESTS = $(wildcard ./unit_tests/*.c)

all: clean tetris

install: clean tetris
	mkdir ./bin
	cp tetris ./bin

uninstall:
	rm -rf ./bin

dvi: clean doxy
	@echo "Documentation generated in HTML format. Open ./doxygen/html/index.html in your browser."

doxy: clean
	sudo doxygen

tetris: tetris.a $(OBJS)
	$(CC) $(CFLAGS) -o $@ tetris.a -lncurses

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -lncurses

tetris.a: $(OBJS)
	ar rc tetris.a $(OBJS)
	ranlib tetris.a

test: clean
	> high_score.txt
	$(CC) $(CFLAGS) $(TESTS) -c
	$(CC) $(CFLAGS) -o unit_test *.o ./brick_game/tetris/*.c --coverage -lcheck -lsubunit -lm -lncurses
	chmod +x unit_test
	./unit_test
	> high_score.txt

gcov_report: clean test
	gcov *.gcda
	gcovr
	gcovr --html-details -o report.html -g

clean:
	rm -rf *.a *.o *.gcno *.gcda unit_test *.css *.html *.gcov tetris ./doxygen
	rm -rf gui/cli/*.o brick_game/tetris/*.o

format:
	clang-format -n ./brick_game/tetris/*.h ./brick_game/tetris/*.c ./gui/cli/*.c ./gui/cli/*.h ./unit_tests/*.h ./unit_tests/*.c main.c

formati:
	clang-format -i ./brick_game/tetris/*.h ./brick_game/tetris/*.c ./gui/cli/*.c ./gui/cli/*.h ./unit_tests/*.h ./unit_tests/*.c main.c

valgrind:
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all -s  ./unit_test