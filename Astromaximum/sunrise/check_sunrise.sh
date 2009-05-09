#!/bin/bash
rm 1.txt 2.txt 3.txt
find ../data/archive -name Data*.dat -exec sunrise {} >> 1.txt \;
grep -Po "\.\..*\.dat" 1.txt > 2.txt
echo "Errors found:"
cat 2.txt

