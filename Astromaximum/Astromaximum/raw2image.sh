#!/bin/sh
IMG_W=$1
IMG_H=$2
LNG=$3
YEAR=$4

FIND=/d/prj/msys/1.0/bin/find
WTK=/d/wtk2.5.1

if ! [ -e $FIND ]; then
	FIND=find
fi
echo find is at $FIND
if ! [ -d $WTK ]; then
	WTK=$HOME/wtk251
fi
echo WTK  is at $WTK

exit
OPTIONS="-colorize 28,18,8"
IMG_PATH=$WTK/appdb/root/filesystem/root1

DEST_PATH=$PWD/../site/i/daily
mkdir -p $DEST_PATH
cd $IMG_PATH

#convert 090101-4.png -fill "#8699ac" -draw 'rectangle 1,226,140,238' -fill white -draw 'gravity SouthWest text 5,0 Kiev' 090101-4-1.png
#display 090101-4-1.png
#exit
dirs=`$FIND . -type d -maxdepth 1`
for i in $dirs; do
    cd $IMG_PATH/$i;
    echo "Converting $PWD";
    mogrify $OPTIONS -format png -size ${IMG_W}x${IMG_H}+1 -depth 8 rgba:*.raw
    cd ..;
done

$FIND . -name *.raw -exec rm '{}' \;

cd $IMG_PATH
dest_arc=$DEST_PATH/informers_${YEAR}_${LNG}.tgz
tar czf $dest_arc *
#rm -r *
echo "$dest_arc written"

# list \d{6}\-\d\.raw w/o duplicates
# ls -1 *.raw | grep -Po '\d{6}' | sort -u

