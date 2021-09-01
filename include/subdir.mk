define submake
$(1)/compile:
	@echo "\n\033[32;1m Building $(1)... ...\033[0m"
	$(MAKE) -C $(BUILD_DIR)/$(1)
$(1)/clean:
	@echo "\033[31;1m Clean $(1)... ...\033[0m"
	$(MAKE) -C $(BUILD_DIR)/$(1) clean
endef

define dependency
$(1):$(2)
endef