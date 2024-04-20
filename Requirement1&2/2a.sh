#!/bin/bash
function convert {
    mode=1
    if [ $2 ];then
        mode=$1
        number=$2
    else
        number=$1
    fi
    if [ $mode -eq 1 ];then
        sum=0
        for i in $(seq 1 ${#number}); do
            let "sum*=2"
            let "sum+=${number:i-1:1}"
        done
        echo "$sum"
    else
        result=""
        remain=0
        while [ $number -gt 0 ];
        do
            let "remain=$number%2"
            result+=$remain
            let "number/=2"
        done
        echo "$result"
    fi
}