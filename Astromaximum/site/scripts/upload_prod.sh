#!/bin/bash
set -x

HOST=w_astromaximum-com_79c70237@astromaximum.com

DATE=`date +%Y%m%d`
ARCFILE=amaxsite_${DATE}.tgz

scp deploy/$ARCFILE $HOST:arc/
ssh $HOST tar xzfv arc/$ARCFILE -C http
