#set -x
mkdir -p ../i/
cd art
for i in *.svg; do 
    fname=${i%.svg};
    perl ../strip_flow_root.pl < $i | convert - ../../i/$fname.png;
    echo $fname; 
done

