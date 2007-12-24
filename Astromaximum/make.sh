#!/bin/sh
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

