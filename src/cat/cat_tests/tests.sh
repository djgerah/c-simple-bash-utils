#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF=""
start_time=$(date +%s)

s21_command=("./s21_cat")
sys_command=("cat")

tests=(
"FLAGS test_files/test_case_cat.txt"
"FLAGS test_files/test_case_cat.txt test_files/test_1_cat.txt"
"FLAGS test_files/test_ascii_characters.txt"
"FLAGS test_files/test_empty_grep.txt s21_cat.c"
)

flags=("b" "e" "n" "s" "t" "v" "E" "T")

manual=(
"-s test_files/test_1_cat.txt"
"-b -e -n -s -t -v test_files/test_1_cat.txt"
"-b test_files/test_1_cat.txt nofile.txt"
"-t test_files/test_3_cat.txt"
"-n test_files/test_2_cat.txt"
"no_file.txt"
"-n -b test_files/test_1_cat.txt"
"-s -n -e test_files/test_4_cat.txt"
"test_files/test_1_cat.txt -n"
"-n test_files/test_1_cat.txt"
"-n test_files/test_1_cat.txt test_files/test_2_cat.txt"
"-v test_files/test_5_cat.txt"
"-- test_files/test_5_cat.txt"
"test_files/test_empty_grep.txt s21_cat.c -b -e -n -s -t -v"
)

gnu=(
"-T test_files/test_1_cat.txt"
"-E test_files/test_1_cat.txt"
"-vT test_files/test_3_cat.txt"
"--number test_files/test_2_cat.txt"
"--squeeze-blank test_files/test_1_cat.txt"
"--number-nonblank test_files/test_4_cat.txt"
"test_files/test_1_cat.txt --number --number"
"-bnvste test_files/test_6_cat.txt"
)
testing() {
    param=$(echo "$@" | sed "s/FLAGS/$var/")
    "${s21_command[@]}" $param > "${s21_command[@]}".log
    "${sys_command[@]}" $param > "${sys_command[@]}".log
    printf "\033[K\033[0m\r\033[1;32mТекущий шаблон поиска\033[0m \033[1;33m${s21_command[@]} $param\033[0m"
    DIFF="$(diff -s "${s21_command[@]}".log "${sys_command[@]}".log)"
    let "COUNTER++"
    if [ "$DIFF" == "Files "${s21_command[@]}".log and "${sys_command[@]}".log are identical" ]
    then
        let "SUCCESS++"
        printf "\033[K\033[0m\r\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m ${sys_command[@]} $param";
        printf "\033[K\033[0m"
    else
        let "FAIL++"
        echo -e "\033[K\033[0m\n\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m ${sys_command[@]} $param"
    fi
    rm -f "${s21_command[@]}".log "${sys_command[@]}".log
}

echo "TESTS WITH NORMAL FLAGS"
echo "^^^^^^^^^^^^^^^^^^^^^^^"
echo "MANUAL TESTS"
printf "\n"
for i in "${manual[@]}"
do
    var="-"
    testing "$i"
done
printf "\033[K\033[0m\n\n"
echo "AUTOTESTS"
echo "^^^^^^^^^"
echo "1 PARAMETER"
j=0
k=0
n=${#flags[@]}
for ((i = 0; i < n; i++)); do
    for o in "${tests[@]}"; do
        var="-${flags[i]}"
        testing $o
    done
done
printf "\n\n"
echo "2 PARAMETERS"
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
printf "\n\n"
echo "3 PARAMETERS"
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
printf "\n\n"
echo "4 PARAMETERS"
# flags=(" 0 " " 1 " " 2 " " 3 " " 4 " " 5 " " 6 "  " 7 ")
# n=${#flags[@]}
# m=0
j=0
k=0
for ((i = 0; i < (n - 3); i++)); do
    for ((j = (i + 1); j < (n - 2); j++)); do
        for ((k = (i + 2); k < (n - 1); k++)); do
            if ((k <= j)); then
                k=$((j + 1))
            fi
            for ((l = (i + 3); l < n; l++)); do
                
                if ((l <= k)); then
                    l=$((k + 1))
                fi
                if ((k == (n - 1))); then
                    j=$((j + 1))
                fi
                for o in "${tests[@]}"; do
                    var="-${flags[i]} -${flags[j]} -${flags[k]} -${flags[l]}"
                    # m=$((m + 1))
                    # echo "$m: $var"
                    testing $o
                done
            done
        done
    done
done

# 2 сдвоенных параметра
printf "\n\n"
echo "2 BUILT-IN PARAMETERS"
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

# # 3 строенных параметра
printf "\n\n"
echo "3 BUILT-IN PARAMETERS"

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
# # 4 счетверенных параметра
printf "\n\n"
echo "4 BUILT-IN PARAMETERS"
j=0
k=0
for ((i = 0; i < (n - 3); i++)); do
    for ((j = (i + 1); j < (n - 2); j++)); do
        for ((k = (i + 2); k < (n - 1); k++)); do
            if ((k <= j)); then
                k=$((j + 1))
            fi
            for ((l = (i + 3); l < n; l++)); do
                
                if ((l <= k)); then
                    l=$((k + 1))
                fi
                if ((k == (n - 1))); then
                    j=$((j + 1))
                fi
                for o in "${tests[@]}"; do
                    var="-${flags[i]}${flags[j]}${flags[k]}${flags[l]}"
                    # m=$((m + 1))
                    # echo "$m: $var"
                    testing $o
                done
            done
        done
    done
done

echo "\033[K"
echo -e "\033[31mFAIL: $FAIL\033[0m"
echo -e "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"
printf "\n"
##############################
FAIL=0
SUCCESS=0
COUNTER=0

echo "TESTS WITH GNU FLAGS"
echo "^^^^^^^^^^^^^^^^^^^^^^^"

echo "MANUAL TESTS"
for i in "${gnu[@]}"
do
    var="-"
    testing $i
done
echo "\033[K"
echo -e "\033[31mFAIL: $FAIL\033[0m"
echo -e "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"

end_time=$(date +%s)
execution_time=$((end_time - start_time))

echo -e "Время выполнения:\033[1;32m $execution_time \033[1;30mсек."