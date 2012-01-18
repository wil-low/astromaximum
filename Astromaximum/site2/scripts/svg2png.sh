#!/bin/bash
#set -x
WORKDIR=`pwd`
mkdir -p i/
cd src/art
for i in *.svg; do 
    fname=${i%.svg};
    perl $WORKDIR/scripts/strip_flow_root.pl < $i | convert - $WORKDIR/i/$fname.png;
    echo $fname; 
done

