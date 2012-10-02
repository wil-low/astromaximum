#!/bin/bash
set -x
DATE=`date +%Y%m%d`
TMPDIR=tmp_deploy/
FILE_SUFFIX=$1

TARGETFILE=../deploy/amaxsite_${DATE}.tgz
rm -f $TARGETFILE

FILES=".htaccess$FILE_SUFFIX favicon.ico *.php *.css robots.txt *.js i/*.png i/*.jpg mobi/html/ mobi/*.php mobi/*.css mobi/dl/ mobi/i/ mobi/kcaptcha/ mobi/mobile_device_detect/ mobi/paypal/ mobi/phpmailer/ mobi/fastjar mobi/sunrise "

rm -rf $TMPDIR
mkdir -p $TMPDIR deploy/
cp -r --parents $FILES $TMPDIR
cd $TMPDIR

# change files
rm pwdgen_local.php mobi/dl/gen_amax.log
mv .htaccess$FILE_SUFFIX .htaccess
chmod a+w mobi/dl/{files,inbox,source}
chmod a+w mobi/dl/source/restore

CONF=mobi/config.php
perl -lpe 'require "../../genconst.pm"; s/<<VERSION>>/$const::VERSION/' $CONF > $CONF.new
mv -f $CONF.new $CONF

tar czf $TARGETFILE .
