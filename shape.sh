#!/bin/bash

printStar() {
    size=5

    for ((i = 1; i <= size; i++)); do
        for ((j = 1; j <= size; j++)); do 
            if [ $i -eq $((size / 2 + 1)) ] || [ $j -eq $((size / 2 + 1)) ]; then
                echo -n "xx"
            else
                echo -n "  "
            fi
        done
        echo ""
    done
}

printStar
