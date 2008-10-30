DEST=site/mobi/dl
SOURCE=$(DEST)/source

.PHONY: all gen_amax config template interp mutter relgui test sunrise

all:	gen_amax config template interp mutter relgui sunrise

test:	
	#make mutter; \
	#rm -f data/archive/2008/test/Data0000.dat; \
	perl geoCoord.pl 2008 test; \
	mutter2/mutter2 2008 dump test 0 -2

gen_amax:
	mkdir -p $(DEST) $(SOURCE); \
	cat tools.pm genconst.pm gen_amax.pl > $(DEST)/gen_amax.cgi

template:
	cp templates/AstromaximumDemo.jar $(SOURCE); \
	cp templates/GeoAM.jar $(SOURCE); \
	cp templates/Astromaximum-tb.jar $(SOURCE); \
	cp templates/MANIFEST.MF $(SOURCE)

interp:
	cp -Rf interpret $(SOURCE)/interpret; \
	cp htaccess $(SOURCE)/interpret/.htaccess; \
	if ls *.comm 2> /dev/null; then cp *.comm $(SOURCE); fi; \
	mkdir -p $(DEST)/files $(DEST)/inbox $(SOURCE)/restore; \
	touch $(DEST)/gen_amax.log; \
	chmod a+w  $(DEST)/files $(DEST)/inbox $(SOURCE) $(SOURCE)/restore \
		$(DEST)/gen_amax.log

config:
	mkdir -p Astromaximum/deploy;perl size_join.pl;perl ph_join.pl; \
	cd swe && make && cd ..; \
	mkdir -p Astromaximum/deploy; \
	perl gen_amax.pl rebuild

mutter:
	cd mutter2 && make CONF=Release $1 build && cd ..

relgui:
	cd relgui && make CONF=Release $1 build && cd ..

sunrise:
	cd sunrise && make CONF=Release $1 build && cd ..

help:
	@echo "Usage: make [all|gen_amax|template|interp|config|mutter|relgui|sunrise]"
