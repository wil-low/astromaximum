#!/bin/bash
set -x

# parameters: year, calculation dir
if [ "x$2" == "x" ]; then
	echo "Usage: sh compress_year.sh <year> <calculations dir>"
	exit 1
fi
year=$1
CALCULATIONS_DIR=$2
cd $CALCULATIONS_DIR/archive/$year
find . -name '*.txt' -o -name '*.dat' > $year.lst
arch=$CALCULATIONS_DIR/compressed/$year.tbz
tar cjf $arch -T $year.lst
rm $year.lst
echo "Written $arch"
ls -l $arch

