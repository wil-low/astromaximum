DEST=../amax-mobi/dl
SOURCE=$(DEST)/source

all:	config gen_amax template interpret

gen_amax:
	mkdir $(DEST) $(SOURCE); \
	cat tools.pm genconst.pm gen_amax.pl > $(DEST)/gen_amax.cgi; \

template:	
	cp templates/AstromaximumDemo.jar $(SOURCE); \
	cp templates/GeoAM.jar $(SOURCE); \
	cp templates/Astromaximum-tb.jar $(SOURCE); \
	cp templates/MANIFEST.MF $(SOURCE)

interpret:	
	svn export --force interpret $(SOURCE)/interpret; \
	#svn export --force images/icons $(SOURCE)/icons
	cp htaccess $(SOURCE)/interpret/.htaccess; \

config:
	mkdir Astromaximum/deploy;perl size_join.pl;perl ph_join.pl; \
	cd swe && make && cd ..; \
	cd mutter2 && make CONF=Release $1 build && cd ..; \
	cd relgui && make CONF=Release $1 build && cd ..; \
	perl gen_amax.pl rebuild

help:
	echo "Usage: make config|gen_amax|template|interpret"

