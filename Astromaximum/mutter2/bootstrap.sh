#!/usr/bin/env bash
rm -rf *.cache
mkdir -p m4
echo aclocal...
aclocal
echo libtoolize...
`which glibtoolize`
if [ $? = 0 ]; then
    glibtoolize --force --copy --automake
else
    libtoolize --force --copy --automake
fi
echo automake...
automake --foreign --add-missing --copy
echo autoconf...
autoconf
echo "Now run bash build.sh"
