CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

.PHONY: all build s21_grep test check valgrind clang clean rebuild

all: build

build: s21_grep

s21_grep: s21_grep.o
	${CC} ${CFLAGS} s21_grep.c -o s21_grep

s21_grep.o: s21_grep.c
	$(CC) $(CFLAGS) -c s21_grep.c

test:
	chmod +x ./grep_tests/tests.sh
	./grep_tests/tests.sh

check:
	cppcheck --enable=all --suppress=missingIncludeSystem --suppress=constParameterPointer --suppress=checkersReport *.h *.c

valgrind:
	valgrind --tool=memcheck --leak-check=yes ./s21_grep
	
clang:
	clang-format -style=Google -i *.h *.c

clean:
	rm -rf s21_grep *.o

rebuild: all