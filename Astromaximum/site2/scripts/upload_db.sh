#!/bin/bash
set -x
DIR=$1
HOST=amdj@ssh.alwaysdata.com
DBFILE=amax_dj.sdb

echo 'vacuum;' | sqlite3 $DBFILE
scp amax_dj.sdb $HOST:$DIR
