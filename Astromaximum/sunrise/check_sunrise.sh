#!/bin/bash
set -x
rm 1.txt 2.txt 3.txt
find -L ../data/archive -name *.dat -exec ./sunrise {} >> 1.txt \;
grep -Po "\.\..*\.dat" 1.txt > 2.txt
echo "Errors found:"
cat 2.txt

