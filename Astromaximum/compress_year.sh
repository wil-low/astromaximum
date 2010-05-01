#!/bin/sh

# parameters: year
#if [ "$1" == "" ]; then
#	echo "Please specify year"
#	exit 1
#fi
year=$1
cd data/archive/$year
find . -name '*.txt' -o -name '*.dat' > $year.lst
arch=../../$year.tbz
tar cjf $arch -T $year.lst
rm $year.lst
echo "Written ./data/$year.tbz"
ls -l $arch

