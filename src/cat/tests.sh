#!/bin/bash

set -e

green="\033[0;32m"
reset="\033[0m"

VG=""

if [ "$1" = "valgrind" ]; then
    VG="valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes"
fi

run_test() {
    echo "$2"
    $VG ./s21_cat $1 > s21_cat.txt
    cat $1 > cat.txt
    diff -s s21_cat.txt cat.txt
    echo
    rm s21_cat.txt cat.txt
}

echo -e "Hello\n\nWorld !\nTest\r\n" > test.txt

run_test "test.txt" "no flags test"
run_test "-b test.txt" "-b test"
run_test "-e test.txt" "-e test"
run_test "-n test.txt" "-n test"
run_test "-s test.txt" "-s test"
run_test "-t test.txt" "-t test"
run_test "-v test.txt" "-v test"

rm test.txt

echo -e "${green}All tests completed!${reset}"
