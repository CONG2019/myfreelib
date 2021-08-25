include rules.mk
include include/subdir.mk
export BUILD_DIR PKG_INSTALL_ROOT_DIR

PACKAGES_LIST=$(shell ls $(CURRENT_DIR)/packages)
TARGETS_TMP=$(foreach package, $(PACKAGES_LIST), packages/$(package))
TARGETS=$(foreach targets_tmp, $(TARGETS_TMP), $(targets_tmp)/compile)

world:init prepare $(TARGETS)

$(foreach target, $(TARGETS_TMP), $(eval $(call submake,$(target))))

prepare:.prepare

# The copy must be executed every time
init:FORCE
	$(INSTALL_DIR) $(BUILD_DIR)
	$(CP) -p $(CURRENT_DIR)/packages $(BUILD_DIR)	# also copy mode, ownership and timestamps for make to determine whether the files is updated

.prepare:
	$(TOUCH) .prepare

FORCE:

clean:
	$(RM) .prepare

.PHONY : world clean prepare init FORCE