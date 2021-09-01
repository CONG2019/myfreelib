include rules.mk
include $(INCLUDE_DIR)/subdir.mk
export BUILD_DIR PKG_INSTALL_ROOT_DIR INCLUDE_DIR

PACKAGES_LIST=$(shell ls $(CURRENT_DIR)/packages)
TARGETS_TMP=$(foreach package, $(PACKAGES_LIST), packages/$(package))
TARGETS=$(foreach targets_tmp, $(TARGETS_TMP), $(targets_tmp)/compile)



world:init prepare $(TARGETS)

prepare:.prepare

# Generating dependency: packages/package_name/compile:init
$(foreach target, $(TARGETS), $(eval $(call dependency,$(target),init)))
# Generating rule: packages/package_name/{clean,compile}
$(foreach target, $(TARGETS_TMP), $(eval $(call submake,$(target))))

# Generating runnung rule: packages/sys_call_demo/package_name/run
include $(INCLUDE_DIR)/run.mk
SYS_CALL_DEMO_PACKAGE=$(shell cat $(CURRENT_DIR)/packages/sys_call_demo/packages.list)
$(foreach package,$(SYS_CALL_DEMO_PACKAGE),$(eval $(call run,packages/sys_call_demo/$(package))))

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