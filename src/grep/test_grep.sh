#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a basicTests=(
"reflect test_0_grep.txt test_2_grep.txt"
"-i trulyalya test_3_grep.txt "
"-v s test_4_grep.txt "
"-c s test_3_grep.txt "
"-l s test_2_grep.txt test_6_grep.txt test_5_grep.txt "
"-n s test_0_grep.txt "
"-o 123 test_1_grep.txt "
"-h reflecttest_1_grep.txt test_5_grep.txt "
"grep -s 123123 "
"-f test_ptrn_grep.txt test_5_grep.txt "
"-in trulyalya test_2_grep.txt"
"-cv s test_3_grep.txt"
"-iv s test_4_grep.txt"
"-lv s test_6_grep.txt test_0_grep.txt test_4_grep.txt"
"-ho reflect test_3_grep.txt test_6_grep.txt"
"-nf test_ptrn_grep.txt test_2_grep.txt"
)

declare -a extra=(
"-n str1 test_1_grep.txt test_2_grep.txt"
"-n for test_1_grep.txt"
"-n -e ^\} test_1_grep.txt"
"-c -e 345 test_1_grep.txt"
"-ce ^int test_1_grep.txt test_2_grep.txt"
"-e ^int test_1_grep.txt"
"-nivh = test_1_grep.txt test_2_grep.txt"
"-e"
"-ie INT test_5_grep.txt"
"-echar test_1_grep.txt test_2_grep.txt"
"-ne = -e out test_5_grep.txt"
"-iv int test_5_grep.txt"
"-in int test_5_grep.txt"
"-c -l aboba test_1_grep.txt test_5_grep.txt"
"-v test_1_grep.txt -e ank"
"-noe \\ test_5_grep.txt"
"-l for test_1_grep.txt test_2_grep.txt"
"-o -e int test_4_grep.txt"
"-e = -e out test_5_grep.txt"
"-e ing -e as -e the -e not -e is test_6_grep.txt"
"-c -e . test_1_grep.txt -e '.'"
"-l for no_file.txt test_2_grep.txt"
"-f test_3_grep.txt test_5_grep.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_grep $t > test_s21_grep.log
    grep $t > test_sys_grep.log
    DIFF_RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
    then
      (( SUCCESS++ ))
      echo "SUCCESS: grep $t"
    else
      (( FAIL++ ))
      echo "FAIL: grep $t"
    fi
    rm test_s21_grep.log test_sys_grep.log
}

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

# 1 параметр
for var1 in v c l n h o
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# 2 параметра base
for var1 in v c l n h o
do
   for var2 in v c l n h o
   do
       if [ $var1 != $var2 ]
       then
           for i in "${basicTests[@]}"
           do
               var="-$var1 -$var2"
               testing $i
           done
       fi
   done
done

# 2 сдвоенных параметра
for var1 in v c l n h o
do
   for var2 in v c l n h o
   do
       if [ $var1 != $var2 ]
       then
           for i in "${tests[@]}"
           do
               var="-$var1$var2"
               testing $i
           done
       fi
   done
done

echo "FAIL: $FAIL"
echo "SUCCESS: $SUCCESS"
echo "ALL: $COUNTER"
