#!/bin/bash
set -x

HOST=amdj@ssh.alwaysdata.com

DATE=`date +%Y%m%d`
ARCFILE=amdj_${DATE}.tgz

scp deploy/$ARCFILE $HOST:arc/
ssh $HOST tar xzfv arc/$ARCFILE -C site2
