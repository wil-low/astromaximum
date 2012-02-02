all: deploy

upload: deploy_prod
	bash scripts/upload.sh

upload_db:
	bash scripts/upload_db.sh '/home/amdj/site2/'

deploy_prod:
	bash scripts/make_archive.sh '/home/amdj/site2/'

deploy_dev:
	bash scripts/make_archive.sh '/home/willow/amax/site_amdj/'

text:
	mkdir -p amax/fixtures/
	perl scripts/initial_data.pl > amax/fixtures/initial_data.json

art: i/a0.png

i/a0.png:
	bash scripts/svg2png.sh

clean:
	rm -f i/*.png

.PHONY: deploy_prod deploy_dev art text upload all
