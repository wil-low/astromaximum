#!/bin/bash
#set -x
WORKDIR=`pwd`
mkdir -p i/
for i in *.svg; do 
    fname=${i%.svg};
    perl $WORKDIR/strip_flow_root.pl < $i | convert - $WORKDIR/i/$fname.png;
    echo $fname; 
done
cd i
optipng -o7 *.png
pngcrush -d ../../ -rem alla *.png
cd ..
rm -r i/
