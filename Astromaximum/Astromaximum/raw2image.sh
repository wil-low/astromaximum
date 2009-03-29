#!/bin/sh
export IMG_W=$1
export IMG_H=$2

export OPTIONS="-colorize 28,18,8"
export IMG_PATH=/home/willow/wtk251/appdb/ReducedColorPhone/filesystem/root1

export DEST_PATH=../site/i/daily/ru

mkdir -p $DEST_PATH/raw
cd $DEST_PATH

#convert 090101-4.png -fill "#8699ac" -draw 'rectangle 1,226,140,238' -fill white -draw 'gravity SouthWest text 5,0 Kiev' 090101-4-1.png
#display 090101-4-1.png
#exit

mogrify $OPTIONS -format png -size ${IMG_W}x${IMG_H}+1 -depth 8 \
	rgba:${IMG_PATH}/*.raw

mv ${IMG_PATH}/*.raw raw

mv ${IMG_PATH}/*.png .

#rm ${IMG_PATH}/*.raw

# list \d{6}\-\d\.raw w/o duplicates
# ls -1 *.raw | grep -Po '\d{6}' | sort -u

