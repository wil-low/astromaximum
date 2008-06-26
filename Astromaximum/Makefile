DEST=site/mobi/dl
SOURCE=$(DEST)/source

.PHONY: gen_amax config template interp mutter relgui test

test:	
	#make mutter; \
	#rm -f data/archive/2008/test/Data0000.dat; \
	perl geoCoord.pl 2008 test; \
	mutter2/mutter2 2008 dump test 0 -2

all:	gen_amax config template interp mutter relgui

gen_amax:
	mkdir $(DEST) $(SOURCE); \
	cat tools.pm genconst.pm gen_amax.pl > $(DEST)/gen_amax.cgi

template:
	cp templates/AstromaximumDemo.jar $(SOURCE); \
	cp templates/GeoAM.jar $(SOURCE); \
	cp templates/Astromaximum-tb.jar $(SOURCE); \
	cp templates/MANIFEST.MF $(SOURCE)

interp:
	svn export --force interpret $(SOURCE)/interpret; \
	#svn export --force images/icons $(SOURCE)/icons
	cp htaccess $(SOURCE)/interpret/.htaccess

config:
	mkdir Astromaximum/deploy;perl size_join.pl;perl ph_join.pl; \
	cd swe && make && cd ..; \
	perl gen_amax.pl rebuild

mutter:
	cd mutter2;  make CONF=Release build; cd ..

relgui:
	cd relgui && make CONF=Release $1 build && cd ..

help:
	@echo "Usage: make [gen_amax|template|interp|config|mutter|relgui]"
