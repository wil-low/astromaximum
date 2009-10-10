#!/bin/sh

# parameters: year
if [ "$1" == "" ]; then
	echo "Please specify year"
	exit 1
fi
year=$1
cd data/archive/$year
find . -name *.txt -o -name *.dat > $year.lst
tar cjf ../../$year.tbz -T $year.lst
rm $year.lst
echo "Written data/$year.tbz"

