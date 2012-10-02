#!/bin/bash
set -x

HOST=amdj@ssh.alwaysdata.com

DATE=`date +%Y%m%d`
ARCFILE=amaxsite_${DATE}.tgz

sudo tar xzf deploy/$ARCFILE -C /var/www/

