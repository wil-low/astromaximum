.PHONY: clean All

All:
	@echo ----------Building project:[ Astronom - Debug ]----------
	@"$(MAKE)" -f "Astronom.mk"
clean:
	@echo ----------Cleaning project:[ Astronom - Debug ]----------
	@"$(MAKE)" -f "Astronom.mk" clean
