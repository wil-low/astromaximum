set -x
cp ../3d_party/sweph/src/libswe.so bin/Debug
cd bin/Debug
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
./Astronom $1 $2 $3 $4 $5 -tracelevel 90
#valgrind --leak-check=full ./Astronom $1 $2 $3 $4 $5

