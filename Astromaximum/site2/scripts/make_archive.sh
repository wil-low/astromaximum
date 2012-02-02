#!/bin/bash
set -x
DATE=`date +%Y%m%d`
TMPDIR=tmp_deploy/
FILES='*.py 3d_party/dateutil/*.py amax/*.py amax/fixtures/ data/*.py desktop/*.py i/ m/*.py m/templatetags/*.py mobi/*.py public/ templates/'
PROJECT_ROOT=$1

rm -rf $TMPDIR
mkdir -p $TMPDIR deploy/
cp -r --parents $FILES $TMPDIR
cd $TMPDIR

# change files
mkdir -p data/{commons,locations}
chmod +x public/django.fcgi


perl -lape "s%^PROJECT_ROOT = .+%PROJECT_ROOT = '$PROJECT_ROOT'%; s%^DEBUG = .+%DEBUG = False%; s%^USE_PANEL = .+%USE_PANEL = False%;" \
	settings.py > settings.tmp
mv settings.tmp settings.py

TARGETFILE=../deploy/amdj_${DATE}.tgz
rm -f $TARGETFILE
tar czfv $TARGETFILE .
