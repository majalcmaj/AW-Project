#!/bin/bash

[ $# -eq 4 ] || exit 1
algorithm=$1
input=$2
scale=$3
times_file=$4

printf "$algorithm|$input," >> $times_file
for i in {1..20}
do
    echo Iter $i
    printf "`build/program.out $algorithm $input $scale`," >> $times_file
done
printf "\n" >> $times_file


