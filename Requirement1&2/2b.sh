#!/bin/bash
function checkPal {
    string_word=$1
    string_word=${string_word,,}
    j=0
    let "j=${#string_word}-j-1"
    i=0
    flag=1
    while [ $i -lt $j ];do
        if [ ${string_word:i:1} = ${string_word:j:1} ];then
            let "i+=1"
            let "j-=1"
        else
            flag=0
            break
        fi
    done
    echo "$flag"
}

