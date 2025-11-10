#!/bin/bash

set -e

green="\033[0;32m"
reset="\033[0m"

VG=""

if [ "$1" = "valgrind" ]; then
    VG="valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes"
fi

run_test() {
    echo "$3"
    $VG ./s21_grep $1 $2 > s21_grep.txt
    grep $1 $2 > grep.txt
    diff -s s21_grep.txt grep.txt
    echo
    rm s21_grep.txt grep.txt
}

run_test "main" "s21_grep.c" "no flags test"
run_test "-e main -e void" "s21_grep.c" "-e test"
run_test "-c main" "s21_grep.c" "-c test"
run_test "-l main" "s21_grep.c s21_grep.c s21_grep.c" "-l test"
run_test "-n main" "s21_grep.c" "-n test"
run_test "-nh void" "s21_grep.c s21_grep.h" "-h test"
run_test "-v main" "s21_grep.c" "-v test"
run_test "-i MAIN" "s21_grep.c" "-i test"
run_test "-o main" "s21_grep.c" "-o test"
run_test "-n -c main" "s21_grep.c" "-n -c test"
run_test "-in MAIN" "s21_grep.c" "-in test"
run_test "-iv MAIN" "s21_grep.c" "-iv test"
run_test "main" "s21_grep.c s21_grep.h" "multiple files test"
run_test "-oi MAIN" "s21_grep.c" "-o -i test"
run_test "-vc main" "s21_grep.c" "-v -c test"

echo main > _f_test.txt
run_test "-f _f_test.txt" "s21_grep.c" "-f test"
rm _f_test.txt

echo "abc123" > temp.txt
run_test "a.*3" "temp.txt" "regex special chars test"
rm temp.txt

echo -e "${green}All tests completed!${reset}"