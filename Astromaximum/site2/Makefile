all: deploy

deploy_prod:
	bash scripts/make_archive.sh '/home/amdj/site2/'

deploy_dev:
	bash scripts/make_archive.sh '/home/willow/amax/site_amdj/'

art: i/a0.png

i/a0.png:
	bash scripts/svg2png.sh

clean:
	rm -f i/*.png

.PHONY: deploy_prod deploy_dev art all
