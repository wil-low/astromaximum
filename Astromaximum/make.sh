#!/bin/sh
DEST=../amax-mobi/dl
#PERL="/opt/lampp/bin/perl"
PERL="/usr/bin/perl"
SOURCE=$DEST/source
#rm -r $DEST
mkdir $DEST $SOURCE
echo "#!$PERL" | cat - tools.pm genconst.pm gen_amax.pl > $DEST/gen_amax.cgi
chmod +x $DEST/gen_amax.cgi
exit
cp templates/AstromaximumDemo.jar $SOURCE
cp templates/Astromaximum-tb.jar $SOURCE
cp templates/MANIFEST.MF $SOURCE
svn export --force interpret $SOURCE/interpret
#svn export --force images/icons $SOURCE/icons
cp htaccess $SOURCE/interpret/.htaccess

#exit

echo "Size_join"
perl size_join.pl
echo "."

echo "Phase_join"
perl ph_join.pl
echo "."

echo "Swiss ephemeris"
cd swe
make $1
make
cd ..
echo "."

echo "Mutter2"
cd mutter2
make CONF=Release $1 build
cd ..
echo "."

echo "Relgui"
cd relgui
make CONF=Release $1 build
cd ..
echo "."

echo "gen_amax"
perl gen_amax.pl rebuild
echo "."

