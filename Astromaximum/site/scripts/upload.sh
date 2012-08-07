#!/bin/bash
set -x

HOST=amdj@ssh.alwaysdata.com

DATE=`date +%Y%m%d`
ARCFILE=amaxsite_${DATE}.tgz

sudo tar xzfv deploy/$ARCFILE -C /var/www/
#scp deploy/$ARCFILE $HOST:arc/
#ssh $HOST tar xzfv arc/$ARCFILE -C site2
