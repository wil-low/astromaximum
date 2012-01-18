#!/bin/bash
set -x
DATE=`date +%Y%m%d`
TMPDIR=tmp_deploy/
FILES='*.py amax/*.py desktop/*.py i/ m/*.py m/templatetags/*.py mobi/*.py public/ templates/'
PROJECT_ROOT=$1

rm -rf $TMPDIR
mkdir -p $TMPDIR deploy/
cp -r --parents $FILES $TMPDIR
cd $TMPDIR

# change files
mkdir -p data/{commons,locations}
chmod +x public/django.fcgi

perl -lape "s%PROJECT_ROOT = .+%PROJECT_ROOT = '$PROJECT_ROOT'%" settings.py > settings.tmp
mv settings.tmp settings.py

TARGETFILE=../deploy/amdj_${DATE}.tgz
rm -f $TARGETFILE
tar czfv $TARGETFILE .
