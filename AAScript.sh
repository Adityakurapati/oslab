# echo Hello World!

# name="aditya"
# if [ $name = "aditya" ]; then
#   echo "Hello $name"
# else
#   echo "Wrong  person"
# fi

# for i in 1 2 3 4 5; do
#   echo $i
# done

# count=1
# while [ $count -le 10 ]; do
#   echo $i
#   count=$((count+1))
# done

# for i in {1..10}; do
#   echo $i
# done
#!/bin/bash
# Script to print various date and time formats

# echo "Full date and time (default format): $(date)"
# echo "Year (4 digits): $(date +"%Y")"
# echo "Year (2 digits): $(date +"%y")"
# echo "Month (2 digits): $(date +"%m")"
# echo "Month (short name): $(date +"%b")"
# echo "Month (full name): $(date +"%B")"
# echo "Day of month (2 digits): $(date +"%d")"
# echo "Day of week (short name): $(date +"%a")"
# echo "Day of week (full name): $(date +"%A")"
# echo "Week number of year (Sunday as first day): $(date +"%U")"
# echo "Week number of year (Monday as first day): $(date +"%W")"
# echo "Day of year: $(date +"%j")"
# echo "Time (24-hour format): $(date +"%H:%M:%S")"
# echo "Time (12-hour format): $(date +"%I:%M:%S %p")"
# echo "Timezone: $(date +"%Z")"
# echo "Unix timestamp: $(date +"%s")"
# echo "Full date (MM/DD/YY): $(date +"%m/%d/%y")"
# echo "Custom format (e.g., YYYY-MM-DD HH:MM:SS): $(date +"%Y-%m-%d %H:%M:%S")"

echo $(date +"%Y-%m-%d %H:%M:%S")\

#!/bin/bash

# date_diff() {
#   # Get today's date in YYYY-MM-DD format
#   today=$(date +"%Y-%m-%d")

#   # Prompt the user to enter a date
#   read -p "Enter Date (YYYY-MM-DD): " userdate

#   # Validate the date format
#   if [[ ! $userdate =~ ^[0-9]{4}-[0-9]{2}-[0-9]{2}$ ]]; then
#     echo "Invalid Date"
#     return 1
#   fi

#   # Calculate the difference in days
#   date_df=$(( ( $(date -d "$userdate" +%s) - $(date -d "$today" +%s) ) / 86400 ))

#   # Display the result
#   echo "Difference in days: $date_df"
# }

# # Call the function
# date_diff


fabo(){
  n=$1
  a=0
  b=1
  if [[ "$n" -lt 0 ]]; then
    echo "Invalid input"
    return 1
  fi

  for (( i=0; i<n; i++ )); do
    echo "$a "
    fn=$(( $a + $b ))
    a=$b
    b=$fn
  done 
}
fabo 5


fact(){
  fact=1
  n=$1
  if [[ $n -lt 0 ]]; then
    echo "Invalid Input"
    return 1
  fi
  while ((n > 0 )); do
    fact=$((fact*n))
    n=$((n-1))
  done
  echo "Fact : $fact"
}
fact 5

prime(){
  n=$1
  if (( n<= 1 )); then
    echo "Not a prime number"
    return
  fi
  for (( i=2; i*i<=n; i++ )); do
      if (( n%i == 0 )); then
        echo "Not a prime number"
        return
      fi
  done
  echo "Number iS Prime Number"
}
prime 7

sum_of_digits(){
  n=$1
  sum=0
  while (( n > 0 )); do
    digit=$(( n%10 ))
    sum=$(( sum + digit ))
    n=$(( n/10))
  done
  echo "sum of digit : $sum "
}
sum_of_digits 19

reverse(){
  n=$1
  reverse=0

  while (( n > 0 )); do
    digit=$(( n % 10 ))
    reverse=$((reverse *10 + digit ))
    n=$(( n/10 ))
  done
  echo "Reverse IS : $reverse"
}
reverse 122

find_largest(){
  a=$1
  b=$2
  c=$3

  if(( a>b && a>c )); then
    echo "$a is largest"
  elif (( b>a && b>c )); then
    echo "$b is largest"
  else
    echo "$c is largest"
  fi
}
read -p "Enter 3 Numbers " a b c

if ! [[ $a =~ [0-9]+$ ]] || ! [[ $b =~ [0-9]+$ ]] || ! [[ $c =~ [0-9]+$ ]]; then
  echo "Invalid input. Please enter valid numbers."  
fi

find_largest $a $b $c


is_palindrome(){
  str=$1
  len=${#str}

  for (( i=0; i<len/2; i++ )); do
    if (( "${str:i:1}" != "${str:len-i-1:1}" )); then
      echo "Number IS Not Palindrome"
    fi
  done
  echo "Number IS Palindrome"
}
read -p "Check Palindrome " pnum

is_palindrome pnum