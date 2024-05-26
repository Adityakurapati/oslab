#!bin/bash
myArray=($1 $2 $3 $4 $5 $6 $7 $8)
for element in "${myArray[@]}"; do
  if [[ ! " ${uniqueArray[@]} " =~ "$element" ]]; then
    uniqueArray+=("$element")
  fi
done

echo "original Array : ${myArray[@]}"
echo "Unique Array : ${uniqueArray[@]}"