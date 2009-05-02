#!/bin/sh
IMG_W=$1
IMG_H=$2
LNG=$3
YEAR=$4

OPTIONS="-colorize 28,18,8"
IMG_PATH=/home/willow/wtk251/appdb/root/filesystem/root1

DEST_PATH=$PWD/../site/i/daily/$LNG
mkdir -p $DEST_PATH
cd $IMG_PATH

#convert 090101-4.png -fill "#8699ac" -draw 'rectangle 1,226,140,238' -fill white -draw 'gravity SouthWest text 5,0 Kiev' 090101-4-1.png
#display 090101-4-1.png
#exit
dirs=`find . -wholename './*' -type d -maxdepth 1`
for i in $dirs; do
    cd $i;
    echo "Converting $PWD";
    mogrify $OPTIONS -format png -size ${IMG_W}x${IMG_H}+1 -depth 8 rgba:*.raw
    cd ..;
done

find . -name *.raw -delete

dest_arc=$DEST_PATH/informers_${YEAR}_${LNG}.tgz
tar czf $dest_arc *
rm -r *
echo "$dest_arc written"

# list \d{6}\-\d\.raw w/o duplicates
# ls -1 *.raw | grep -Po '\d{6}' | sort -u

