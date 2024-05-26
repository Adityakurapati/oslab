#!/bin/bash

reverseString() {
    read -p "Enter a string: " inputString

    reversedString=""
    length=${#inputString}

    for ((i = length - 1; i >= 0; i--)); do
        reversedString="${reversedString}${inputString:$i:1}"
    done

    echo "Original String: $inputString"
    echo "Reversed String: $reversedString"
}

reverseString
