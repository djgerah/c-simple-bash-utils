CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

.PHONY: all build s21_cat test check valgrind clang clean rebuild

all: build

build: s21_cat

s21_cat: s21_cat.o
	${CC} ${CFLAGS} s21_cat.c -o s21_cat

s21_cat.o: s21_cat.c
	$(CC) $(CFLAGS) -c s21_cat.c

test:
	chmod +x cat_tests/tests.sh
	./cat_tests/tests.sh

check:
	cppcheck --enable=all --suppress=missingIncludeSystem --suppress=constParameterPointer --suppress=checkersReport *.h *.c

valgrind:
	valgrind --tool=memcheck --leak-check=yes test out
	
clang:
	clang-format -style=Google -i *.h *.c

clean:
	rm -rf s21_cat *.o

rebuild: all