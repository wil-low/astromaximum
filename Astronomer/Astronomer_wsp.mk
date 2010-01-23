.PHONY: clean All

All:
	@echo ----------Building project:[ Astronomer - Debug ]----------
	@cd "Astronomer" && "$(MAKE)" -f "Astronomer.mk"
clean:
	@echo ----------Cleaning project:[ Astronomer - Debug ]----------
	@cd "Astronomer" && "$(MAKE)" -f "Astronomer.mk" clean
