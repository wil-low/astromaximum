#!/bin/sh

# parameters: year
year=$1
cd data/archive/$year
find . -name *.txt -o -name *.dat > $year.lst
tar cjf ../../$year.tbz -T $year.lst
rm $year.lst
echo "Written data/$year.tbz"

