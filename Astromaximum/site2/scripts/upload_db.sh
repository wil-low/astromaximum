#!/bin/bash
set -x
DIR=$1
HOST=amdj@ssh.alwaysdata.com

scp amax_dj.sdb $HOST:$DIR
