#!/bin/bash

NO_UPDATE=$1
TZDIR=/home/willow/amax/data/tz/src

mkdir -p $TZDIR
cd $TZDIR

if [ "$NO_UPDATE" == "" ]; then
	rm -rf $TZDIR/*
	apt-get source tzdata
fi

TZSUBDIR=`find -type d -name 'tzdata-*'`
echo $TZSUBDIR
