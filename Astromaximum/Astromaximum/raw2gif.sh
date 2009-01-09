#!/bin/sh
export IMG_W=$1
export IMG_H=$2
export IMG_PATH=/home/willow/wtk251/appdb/DefaultColorPhone/filesystem/root1

mogrify -format gif -size ${IMG_W}x${IMG_H}+1 -depth 8 \
	rgba:${IMG_PATH}/*.raw
rm ${IMG_PATH}/*.raw

# list \d{6}\-\d\.raw w/o duplicates
# ls -1 *.raw | grep -Po '\d{6}' | sort -u

