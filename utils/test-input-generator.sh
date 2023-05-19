#!/bin/bash

function randrange {
    local min=$1
    local max=$2

    # use shuf for greater range
    echo "$(shuf -i $min-$max -n 1)"

    # echo "$(( $min + $RANDOM % ($max + 1 - $min) ))"
}

for arg in "$@"; do
    # split argument
    readarray -td ' ' formats <<< "$arg"

    lines="${formats[0]}"
    ranges=("${formats[@]:1}")

    for ((i=0; i < lines; i++)); do
        line=""
        for range in "${ranges[@]}"; do
            min=${range%-*}
            max=${range#*-}

            line+="$(randrange $min $max) "
        done
        # trim last space in line
        echo "${line::-1}"
    done
done

exit 0