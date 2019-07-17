HOST_PROJ_ROOT ?= $(shell pwd)/../../..
UTILS_DIR = $(HOST_PROJ_ROOT)/utils
UTILS_BIN = $(UTILS_DIR)/bin
UTILS_DAT = $(UTILS_BIN)/example

TEST_AL_PATH = ../TestAL
TEST_AL_GIT = ssh://gerrit.oss.arm.com/ssgcc/TestAL
TEST_AL_GIT_TAG = TAG_TESTAL_VER_3.03

include $(HOST_PROJ_ROOT)/Makefile.defs
copy_integ_cfg := $(shell test -f $(HOST_PROJ_ROOT)/src/tests/integration_cc7x3/proj_integration_tests.cfg || cp $(HOST_PROJ_ROOT)/proj.cfg  $(HOST_PROJ_ROOT)/src/tests/integration_cc7x3/proj_integration_tests.cfg)

ifneq ($(wildcard $(HOST_PROJ_ROOT)/../configs/proj-*),)
$(info Compiling for Internal ARM use)
ARM_INTERNAL=1
else
ARM_INTERNAL=0
endif

unexport CFLAGS

# dont remove this echo line: will skip dependecies if ARM_INTERNAL=0
all: build_test_al create_doxygen
	echo "nop" > /dev/null
ifeq ($(ARM_INTERNAL),1)
	make install_scripts
	make install_data
else 
ifeq ($(TEE_OS),linux64)
	echo "compiling linux64 driver."
	make -C ../../pal/linux64/driver -f Makefile.setup
endif 
endif

build_test_al:
	if [ ! -d $(TEST_AL_PATH) ]; then git clone  $(TEST_AL_GIT) $(TEST_AL_PATH); cd $(TEST_AL_PATH) && git checkout $(TEST_AL_GIT_TAG) && ./TestAL-Lite/testal_lite.sh; fi
	cd $(TEST_AL_PATH) && ./build_config.sh $(TEST_AL_CONFIG_NUM)
	mv $(TEST_AL_PATH)/libHAL* $(HOST_PROJ_ROOT)/lib/libtests_hal_lite.a
	mv $(TEST_AL_PATH)/libPAL* $(HOST_PROJ_ROOT)/lib/libtests_pal_lite.a

install_scripts: $(UTILS_BIN)
	if [ -d "$(FLAVOUR_SCRIPTS_DIR)" ]; then \
		echo "[CP] $(FLAVOUR_SCRIPTS_DIR)/* -> $(UTILS_BIN)"; \
		cp -a $(FLAVOUR_SCRIPTS_DIR)/* $(UTILS_BIN); \
	fi
	
install_data: $(UTILS_DAT)
	if [ -d "$(FLAVOUR_DATA_DIR)" ]; then \
		echo "[CP] $(FLAVOUR_DATA_DIR)/* -> $(UTILS_DAT)"; \
		cp -a $(FLAVOUR_DATA_DIR)/* $(UTILS_DAT); \
	fi

$(UTILS_DAT) $(UTILS_BIN):
	@$(MKDIR) $@

clean: clean_doxygen
	echo "nop" > /dev/null
    
create_doxygen:
	echo "Copying $(HOST_PROJ_ROOT)/../doxygen to doxygen-it directory"
	rm -rf $(HOST_PROJ_ROOT)/../doxygen-it && cp -R $(HOST_PROJ_ROOT)/../doxygen $(HOST_PROJ_ROOT)/../doxygen-it

clean_doxygen:
	echo "Cleaning $(HOST_PROJ_ROOT)/../doxygen-it directory"
	rm -rf $(HOST_PROJ_ROOT)/../doxygen-it
    
.PHONY: default clean build_test_al install_data install_scripts create_doxygen clean_doxygen

include integration_cc7x3.mk
include $(HOST_PROJ_ROOT)/src/tests/integration_cc7x3/proj_integration_tests.cfg

include $(HOST_PROJ_ROOT)/Makefile.rules

.NOTPARALLEL: all
