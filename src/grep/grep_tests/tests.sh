#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""
DIR="test_files/"
start_time=$(date +%s)

declare -a tests=(
"s ${DIR}test_0_grep.txt VAR"
"'for' s21_grep.c ${DIR}test_1_grep.txt ${DIR}test_6_grep.txt VAR"
"'for' s21_grep.c VAR"
"-e 'for' -e ^int s21_grep.c ${DIR}test_1_grep.txt ${DIR}test_6_grep.txt VAR"
"-e 'for' -e ^int s21_grep.c VAR"
""-e" 'regex' -e ^print s21_grep.c VAR -f ${DIR}test_ptrn_grep.txt -f ${DIR}test_4_grep.txt"
"-e 'while' -e void s21_grep.c ${DIR}test_6_grep.txt VAR -f ${DIR}test_ptrn_grep.txt"
)

declare -a extra=(
    "-f ${DIR}test_empty_grep.txt s21_grep.c"
    "-c -e END ${DIR}test_empty_grep.txt"
    "-c -e 10 ${DIR}test_long_grep.txt"
    "${DIR}test_long_grep.txt -f ${DIR}test_long_grep.txt"
    "-c -e . ${DIR}test_1_grep.txt -e '.'"
    "-c -e .$ ${DIR}test_3_grep.txt"
    "-c -e *ring ${DIR}test_1_grep.txt"
    "-c -e std+lib ${DIR}test_1_grep.txt"
    "-c -e ? ${DIR}test_1_grep.txt"
    "-c -e '(int)+' ${DIR}test_1_grep.txt"
    "-c -e 'int|str' ${DIR}test_1_grep.txt"
    "-c -e [isp] ${DIR}test_1_grep.txt"
    "-c -e '){1,3}' ${DIR}test_1_grep.txt"
    "-n for ${DIR}test_1_grep.txt ${DIR}test_2_grep.txt"
    "-n for ${DIR}test_1_grep.txt"
    "-n -e ^\} ${DIR}test_1_grep.txt"
    "-c -e \/ ${DIR}test_1_grep.txt"
    "-ce ^int ${DIR}test_1_grep.txt ${DIR}test_2_grep.txt"
    "-e ^int ${DIR}test_1_grep.txt"
    "-nivh = ${DIR}test_1_grep.txt ${DIR}test_2_grep.txt"
    "-e"
    "-ie INT ${DIR}test_5_grep.txt"
    "-echar ${DIR}test_1_grep.txt ${DIR}test_2_grep.txt"
    "-ne = -e out ${DIR}test_5_grep.txt"
    "-iv int ${DIR}test_5_grep.txt"
    "-in int ${DIR}test_5_grep.txt"
    "-c -l aboba ${DIR}test_1_grep.txt ${DIR}test_5_grep.txt"
    "-v ${DIR}test_1_grep.txt -e ank"
    "-noe ) ${DIR}test_5_grep.txt"
    "-l for ${DIR}test_1_grep.txt ${DIR}test_2_grep.txt"
    "-o -e int ${DIR}test_4_grep.txt"
    "-e = -e out ${DIR}test_5_grep.txt"
    "-noe ing -e as -e the -e not -e is ${DIR}test_6_grep.txt"
    "-e ing -e as -e the -e not -e is ${DIR}test_6_grep.txt"
    "-l for no_file.txt ${DIR}test_2_grep.txt"
    "-f ${DIR}test_3_grep.txt ${DIR}test_5_grep.txt"
)

testing()
{
    # echo "testing аргумент получен : "${1:0:1}" : "${1:1:1}" "
    if [[ ("${1:0:1}" == "-" && "${1:1:1}" == "e") || ("${1:0:1}" == "-" && "${1:1:1}" == "n") ]]; then
     t="-"${1:1:1}""
      temp=${#1}
     t+=$(echo "${1:2:${#1}}" | sed "s/VAR/$var/")
     t+=" "
     t+=$(echo "${@:2:$#}" | sed "s/VAR/$var/")

    else
     t=$(echo "$@" | sed "s/VAR/$var/")
    fi
    printf "\033[K\033[0m\r\033[1;32mТекущий шаблон поиска\033[0m \033[1;33m./s21_grep $t\033[0m"
    
    # echo ""
    # ./s21_grep $t | tee test_s21_grep.log
    printf "\033[K\033[0m"
    ./s21_grep $t > test_s21_grep.log 2> >(sed '1s/^/\n/;' >&2)
    # echo "Сформированная команда grep $t"
    # echo ""
    # grep $t | tee test_sys_grep.log
    printf "\033[K"
    grep $t > test_sys_grep.log

    DIFF_RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
    then
      (( SUCCESS++ ))
      printf "\033[K\033[0m\r"
      printf "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m grep $t";
    else
      (( FAIL++ ))
      printf "\033[K\033[0m"
      echo -e "\n\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m grep $t"
    fi
    # exit
    rm test_s21_grep.log test_sys_grep.log
}

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

flags=("i" "v" "c" "l" "n" "h" "s" "o")

# 1 параметр
j=0
k=0
n=${#flags[@]}
for ((i = 0; i < n; i++)); do
    for o in "${tests[@]}"; do
        var="-${flags[i]}"
        testing $o
    done
done

# # 2 параметра
j=0
k=0
for ((i = 0; i < (n - 1); i++)); do
    for ((j = (i + 1); j < n; j++)); do
        if ((j <= i)); then
            j=$((i + 1))
        fi
        for o in "${tests[@]}"; do
            var="-${flags[i]} -${flags[j]}"
            testing $o
        done
    done
done

# # 3 параметра
j=0
k=0
for ((i = 0; i < (n - 2); i++)); do
    for ((j = (i + 1); j < (n - 1); j++)); do
        for ((k = (i + 2); k < n; k++)); do
            if ((k <= j)); then
                k=$((j + 1))
            fi
            for o in "${tests[@]}"; do
                var="-${flags[i]} -${flags[j]} -${flags[k]}"
                testing $o
            done
        done
    done
done
# # 2 сдвоенных параметра
j=0
k=0
for ((i = 0; i < (n - 1); i++)); do
    for ((j = (i + 1); j < n; j++)); do
        if ((j <= i)); then
            j=$((i + 1))
        fi
        for o in "${tests[@]}"; do
            var="-${flags[i]}${flags[j]}"
            testing $o
        done
    done
done

# 3 строенных параметра
# flags=(" 0 " " 1 " " 2 " " 3 " " 4 " " 5 " " 6 "  " 7 ")
# flags=(" 0 " " 1 " " 2 " " 3 " " 4 " " 5 " " 6 " " 7 " " 8 " " 9 " " 10 " " 11 ")
# flags=("i" "v" "c" "l" "n" "h" "s" "o")
# m=0
j=0
k=0
n=${#flags[@]}
for ((i = 0; i < (n - 2); i++)); do
    for ((j = (i + 1); j < (n - 1); j++)); do
        for ((k = (i + 2); k < n; k++)); do
        if ((k <= j)); then
            k=$((j + 1))
        fi
        for o in "${tests[@]}"; do
            var="-${flags[i]}${flags[j]}${flags[k]}"
            # m=$((m + 1))
            # echo "$m: $var"
            testing $o
        done
        done
    done
done



echo "\033[K"
echo -e "\033[31mFAIL: $FAIL\033[0m"
echo -e "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"

end_time=$(date +%s)
execution_time=$((end_time - start_time))

echo -e "Время выполнения:\033[1;32m $execution_time \033[1;30mсек."