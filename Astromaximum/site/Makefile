DEST=mobi/dl
SOURCE=$(DEST)/source

all:

upload_dev: deploy_dev
	bash scripts/upload_dev.sh

upload_prod: deploy_prod
	bash scripts/upload_prod.sh

deploy_prod: gen_amax templates
	bash scripts/make_archive.sh

deploy_dev: gen_amax templates
	bash scripts/make_archive.sh .dev

gen_amax:
	mkdir -p $(DEST) $(SOURCE) site/i/daily; \
	cat ../tools.pm ../genconst.pm ../gen_amax.pl > $(DEST)/gen_amax.cgi

templates:
	cp ../templates/AstromaximumDemo.jar $(SOURCE); \
	cp ../templates/GeoAM.jar $(SOURCE); \
	cp ../templates/Astromaximum-tb.jar $(SOURCE); \
	cp ../templates/MANIFEST.MF $(SOURCE)

.PHONY: deploy_prod deploy_dev upload_dev upload_prod gen_amax teplates all
