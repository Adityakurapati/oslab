#!/bin/bash

date_diff() {
    today=$(date +"%Y-%m-%d")
    read -p "Enter a date (YYYY-mm-dd): " userDate

    if [[ ! $userDate =~ ^[0-9]{4}-[0-9]{2}-[0-9]{2}$ ]]; then
      echo "Enter a valid date (YYYY-mm-dd)"
      exit 1
    fi

    dateDiff=$(( ($(date -d "$userDate" +%s) - $(date -d "$today" +%s)) / 86400 ))
    echo "The difference between $userDate and today ($today) is $dateDiff days."
}

date_diff
