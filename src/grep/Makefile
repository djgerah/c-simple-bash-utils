CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

all: build

build: s21_grep

s21_grep: s21_grep.o
	${CC} ${CFLAGS} s21_grep.c -o s21_grep

s21_grep.o: s21_grep.c
	$(CC) $(CFLAGS) -c s21_grep.c

.PHONY: test cppcheck valgrind clang clean rebuild

test: clean s21_grep
	chmod +x tests.sh
	./tests.sh

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem --suppress=constParameterPointer --suppress=checkersReport *.h *.c

valgrind: s21_grep
	bash ./tests.sh valgrind

clang:
	clang-format -style=Google -i *.h *.c

clean:
	rm -rf *.o s21_grep

rebuild: clean all