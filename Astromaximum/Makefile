DEST=site/mobi/dl
SOURCE=$(DEST)/source

.PHONY: all gen_amax config rebuild template interp mutter relgui test sunrise encode85 swe

all:	gen_amax config template interp mutter relgui sunrise encode85

test:	
	#make mutter; 
	#rm -f data/archive/2008/test/Data0000.dat; 
	perl geoCoord.pl 2008 test; \
	mutter2/mutter2 2008 dump test 0 -2

gen_amax:
	mkdir -p $(DEST) $(SOURCE) site/i/daily; \
	cat tools.pm genconst.pm gen_amax.pl > $(DEST)/gen_amax.cgi; \
	hg log  --template '{rev}\n' Astromaximum | head -n 1 > rev.txt; \
	hg log  --template '{rev}\n' GeoAM | head -n 1 >> rev.txt; \
	cp rev.txt $(DEST)

template:
	cp templates/AstromaximumDemo.jar $(SOURCE); \
	cp templates/GeoAM.jar $(SOURCE); \
	cp templates/Astromaximum-tb.jar $(SOURCE); \
	cp templates/MANIFEST.MF $(SOURCE)

interp:
	cp -Rf Astromaximum/interpret $(SOURCE); \
	cp htaccess $(SOURCE)/interpret/.htaccess; \
	cp htaccess site/i/daily/.htaccess; \
	if ls *.comm 2> /dev/null; then cp *.comm $(SOURCE); fi; \
	mkdir -p $(DEST)/files $(DEST)/inbox $(SOURCE)/restore; \
	touch $(DEST)/gen_amax.log; \
	chmod a+w  $(DEST)/files $(DEST)/inbox $(SOURCE) $(SOURCE)/restore \
		$(DEST)/gen_amax.log

swe:
	cd swe && make clean && make && cd ..

config:
	mkdir -p Astromaximum/deploy;perl size_join.pl;perl ph_join.pl

rebuild:
	mkdir -p Astromaximum/deploy; \
	perl gen_amax.pl rebuild

mutter:
	cd mutter2 && make CONF=Release clean build && cd ..

relgui:
	cd relgui && make CONF=Release clean build && cd ..

sunrise:
	cd sunrise && make CONF=Release clean build && cd ..

encode85:
	cd 3d_party && gcc -O2 -o encode85 encode85.c

help:
	@echo "Usage: make [all|gen_amax|template|interp|config|mutter|relgui|sunrise|encode85|rebuild]"

