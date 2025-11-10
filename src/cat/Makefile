CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

all: build

build: s21_cat

s21_cat: s21_cat.o
	${CC} ${CFLAGS} s21_cat.c -o s21_cat

s21_cat.o: s21_cat.c
	$(CC) $(CFLAGS) -c s21_cat.c

.PHONY: test cppcheck valgrind clang clean rebuild

test: clean s21_cat
	chmod +x tests.sh
	./tests.sh

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem --suppress=constParameter --suppress=constParameterPointer --suppress=checkersReport *.h *.c

valgrind: s21_cat
	bash ./tests.sh valgrind
	
clang:
	clang-format -style=Google -i *.h *.c

clean:
	rm -rf *.o s21_cat

rebuild: clean all