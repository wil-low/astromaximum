DEST=../amax-mobi/dl
SOURCE=$(DEST)/source

all:	config gen_amax template interpret

gen_amax:
	#rm -r $(DEST)
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
	echo "Size_join"; \
	perl size_join.pl; \
	echo "."; \
	echo "Phase_join"; \
	perl ph_join.pl; \
	echo "."; \
	echo "Swiss ephemeris"; \
	cd swe && make; \
       	cd ..; \
	echo "."; \
	echo "Mutter2"; \
	cd mutter2 && make CONF=Release $1 build; \
	cd ..; \
	echo "."; \
	echo "Relgui"; \
	cd relgui && make CONF=Release $1 build; \
	cd ..; \
	echo "."; \
	echo "gen_amax"; \
	perl gen_amax.pl rebuild; \
	echo "."

help:
	echo "Usage: make config|gen_amax|template|interpret"

