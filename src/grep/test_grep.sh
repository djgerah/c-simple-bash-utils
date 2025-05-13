#!/bin/bash
	
./s21_grep main s21_grep.c > s21_grep.txt
grep main s21_grep.c > grep.txt
diff -s s21_grep.txt grep.txt
echo "no flags test"
rm s21_grep.txt grep.txt

./s21_grep -e main -e void s21_grep.c > s21_grep.txt
grep -e main -e void s21_grep.c > grep.txt
diff -s s21_grep.txt grep.txt
echo "-e test"
rm s21_grep.txt grep.txt

./s21_grep -c main s21_grep.c > s21_grep.txt
grep -c main s21_grep.c > grep.txt
diff -s s21_grep.txt grep.txt
echo "-c test"
rm s21_grep.txt grep.txt

./s21_grep -l main s21_grep.c s21_grep.c s21_grep.c > s21_grep.txt
grep -l main s21_grep.c s21_grep.c s21_grep.c > grep.txt
diff -s s21_grep.txt grep.txt
echo "-l test"
rm s21_grep.txt grep.txt

./s21_grep -n main s21_grep.c > s21_grep.txt
grep -n main s21_grep.c > grep.txt
diff -s s21_grep.txt grep.txt
echo "-n test"
rm s21_grep.txt grep.txt

./s21_grep -s e s21_grep.cpp > s21_grep.txt
grep -s e s21_grep.cpp > grep.txt
diff -s s21_grep.txt grep.txt
echo "-s test"
rm s21_grep.txt grep.txt

./s21_grep -nh void s21_grep.c s21_grep.h > s21_grep.txt
grep -nh void s21_grep.c s21_grep.h > grep.txt
diff -s s21_grep.txt grep.txt
echo "-h test"
rm s21_grep.txt grep.txt

./s21_grep -v main s21_grep.c > s21_grep.txt
grep -v main s21_grep.c > grep.txt
diff -s s21_grep.txt grep.txt
echo "-v test"
rm s21_grep.txt grep.txt

./s21_grep -i MAIN s21_grep.c > s21_grep.txt
grep -i MAIN s21_grep.c > grep.txt
diff -s s21_grep.txt grep.txt
echo "-i test"
rm s21_grep.txt grep.txt

echo main > _f_test.txt
./s21_grep -f _f_test.txt s21_grep.c > s21_grep.txt
grep -f _f_test.txt s21_grep.c > grep.txt
diff -s s21_grep.txt grep.txt
echo "-f test"
rm s21_grep.txt grep.txt _f_test.txt

./s21_grep -o main s21_grep.c > s21_grep.txt
grep -o main s21_grep.c > grep.txt
diff -s s21_grep.txt grep.txt
echo "-o test"
rm s21_grep.txt grep.txt

./s21_grep -n -c main s21_grep.c > s21_grep.txt
grep -n -c main s21_grep.c > grep.txt
diff -s s21_grep.txt grep.txt
echo "-n -c test"
rm s21_grep.txt grep.txt

./s21_grep -in MAIN s21_grep.c > s21_grep.txt
grep -in MAIN s21_grep.c > grep.txt
diff -s s21_grep.txt grep.txt
echo "-in test"
rm s21_grep.txt grep.txt

./s21_grep -iv MAIN s21_grep.c > s21_grep.txt
grep -iv MAIN s21_grep.c > grep.txt
diff -s s21_grep.txt grep.txt
echo "-iv test"
rm s21_grep.txt grep.txt

./s21_grep -iv MAIN s21_grp.c > s21_grep.txt
grep -iv MAIN s21_grp.c > grep.txt
diff -s s21_grep.txt grep.txt
echo "no file test"
rm s21_grep.txt grep.txt