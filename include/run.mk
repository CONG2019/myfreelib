# An Makefile to Generate so run rule for my 
define run
$(1)/run:
	$(MAKE) -C $(BUILD_DIR)/$(1) run
endef