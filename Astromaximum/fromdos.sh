#!/bin/bash

for i in `ls -1 *.pl`; do
	echo "Converting $i";
	fromdos < $i > $i.new;
	if (! head -n 1 $i | grep -q "/bin/perl"); then
		echo "#!/usr/bin/perl"| cat > $i;
		cat < $i.new >> $i;
	fi
	rm $i.new;
done

