all: deploy

upload:
	bash scripts/upload.sh

deploy_prod:
	bash scripts/make_archive.sh

deploy_dev:
	bash scripts/make_archive.sh .dev

.PHONY: deploy_prod deploy_dev upload all
