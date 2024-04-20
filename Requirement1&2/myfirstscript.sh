#!/bin/bash
echo "Ahmed Kamal Abdelrasol"
if test -d Lab1; then
    rm -r Lab1
fi
mkdir Lab1
cp numbers.txt Lab1/numbers.txt
cp words.txt Lab1/words.txt
cd Lab1
touch MergedContent.txt
paste words.txt numbers.txt > MergedContent.txt
echo " ------------------ "
head -n 3 MergedContent.txt
echo " ------------------ "
touch SortedMergedContent.txt
sort MergedContent.txt > SortedMergedContent.txt
echo "The sorted file is :"
cat SortedMergedContent.txt
echo " ------------------ "
chmod a-r SortedMergedContent.txt
sort MergedContent.txt | uniq
touch CapitalSortedMergedContent.txt
tr '[:lower:]' '[:upper:]' < SortedMergedContent.txt > CapitalSortedMergedContent.txt
echo "Permission to access SortedMergedContent.txt is denied because I removed from all users the ability to read this file"
chmod u+r SortedMergedContent.txt
tr '[:lower:]' '[:upper:]' < SortedMergedContent.txt > CapitalSortedMergedContent.txt
echo " ------------------ "
cat MergedContent.txt|grep -n "^w.*[0-9]$"
echo " ------------------ "
touch NewMergedContent.txt
cat MergedContent.txt | tr "i" "o" > NewMergedContent.txt
paste MergedContent.txt NewMergedContent.txt
echo " ------------------ "

