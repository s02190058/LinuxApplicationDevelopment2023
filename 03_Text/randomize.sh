#!/bin/bash

DELAY=${1:-0.05}

LINE=0
COL=0
POINTS=
while IFS= read -d "" -n 1 CHAR
do
    if [[ "$CHAR" = $'\n' ]] ; then
        COL=0
        LINE=$((LINE+1))
    else
        if [[ "$CHAR" != " " ]] ; then
            POINTS="${POINTS}$LINE $COL $CHAR\n"
        fi
        COL=$((COL+1))
    fi
done

tput clear

printf "$POINTS" | shuf | while read LINE COL CHAR
do
    tput cup $LINE $COL
    echo -n "$CHAR"
    sleep $DELAY
done

tput cup $LINE
