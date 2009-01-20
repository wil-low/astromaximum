#!/bin/sh
export IMG_W=$1
export IMG_H=$2

export OPTIONS="-colorize 28,18,8"
export IMG_PATH=/home/willow/wtk251/appdb/ReducedColorPhone/filesystem/root1

export DEST_PATH=../site/i/daily

mkdir -p $DEST_PATH/raw

mogrify $OPTIONS -format png -size ${IMG_W}x${IMG_H}+1 -depth 8 \
	rgba:${IMG_PATH}/*.raw

cd $DEST_PATH

mv ${IMG_PATH}/*.raw raw

mv ${IMG_PATH}/*.png .

#rm ${IMG_PATH}/*.raw

# list \d{6}\-\d\.raw w/o duplicates
# ls -1 *.raw | grep -Po '\d{6}' | sort -u

