# ************
# Project Name
# ************
PROJECT_NAME	= libselftest.a

# ***********
# Executables
# ***********
RM		= rm -f
ECHO		= echo
MKDIR		= mkdir -p
RMDIR		= rm -rf
CP		= cp -R
MV		= mv

# ****************
# Path Definitions
# ****************
PAL_DIR		= $(CURDIR)/../pal
HAL_DIR		= $(CURDIR)/../hal
TESTS_DIR	= $(CURDIR)
CONFIGS_DIR	= $(CURDIR)/../configs

# ****************
# Config Files
# ****************

PROJ_CFG_PATH = $(CURDIR)/../proj.cfg

ifeq ($(wildcard $(PROJ_CFG_PATH)),$(PROJ_CFG_PATH))
include $(PROJ_CFG_PATH)
endif

# ********
# Includes
# ********
INCLUDES_PAL	= $(PAL_DIR)/include
INCLUDES_HAL	= $(HAL_DIR)/include
INCLUDES_TESTS	= $(TESTS_DIR)/includes

INCLUDES 	= $(INCLUDES_TESTS)

ifeq ($(OS),linux)
INCLUDES	+= $(INCLUDES_PAL) $(if $(findstring target,$(DEVICE)), $(INCLUDES_HAL))

else ifeq ($(OS),freertos)
INCLUDES 	+= $(INCLUDES_PAL) $(INCLUDES_HAL)

COMPILER_TYPE_DIR = $(shell echo $(COMPILER_TYPE) | tr a-z A-Z)
INCLUDES_FREERTOS = $(KERNEL_DIR)/OS/FreeRTOS/Source/include
INCLUDES_FREERTOS += $(KERNEL_DIR)/OS/FreeRTOS/Source/portable/$(COMPILER_TYPE_DIR)/ARM_$(CORTEX_SHORT)
INCLUDES_FREERTOS += $(KERNEL_DIR)/OS/FreeRTOS-Plus-CLI
INCLUDES_FREERTOS += $(KERNEL_DIR)/OS/FreeRTOS-Plus-TCP/include
INCLUDES_FREERTOS += $(KERNEL_DIR)/OS/FreeRTOS-Plus-TCP/portable/Compiler/$(COMPILER_TYPE_DIR)
INCLUDES_FREERTOS += $(KERNEL_DIR)/boards/$(BOARD)
INCLUDES_FREERTOS += $(KERNEL_DIR)/boards/$(BOARD)/$(CMSIS_DRIVER)
INCLUDES_EXTRA	= $(INCLUDES_FREERTOS)

else ifeq ($(OS),no_os)
INCLUDES 	+= $(INCLUDES_PAL) $(INCLUDES_HAL)
INCLUDES_EXTRA 	= $(PAL_DIR)/$(OS)

endif

# **************
# Make variables
# **************
ifeq ($(COMPILER_TYPE), armcc)
CC 		= armcc
LD 		= armlink
AR		= armar
AS 		= armasm

else ifeq ($(COMPILER_TYPE), armclang)
CC 		= armclang
LD 		= armlink
AR		= armar
AS 		= armclang

else ifeq ($(COMPILER_TYPE), gcc)
CXX		= $(CROSS_COMPILE)g++
CC		= $(CROSS_COMPILE)gcc
LD		= $(CROSS_COMPILE)ld
AR		= $(CROSS_COMPILE)ar
AS		= $(CROSS_COMPILE)as
endif

# ****************
# Project Definitions
# ****************
SRC		= $(shell find $(TESTS_DIR)/src -name '*.c')
OBJ		= $(SRC:.c=.o)

BIN		= libselftest

LIBS	= $(shell find $(CURDIR)/.. -name '*.a')

# *****
# Flags
# *****

ifeq "$(BOARD)" "MPS2+"
CFLAGS		+= -DMPS2
else
CFLAGS		+= -D$(BOARD)
endif

ifeq ($(COMPILER_TYPE), armcc)
CFLAGS		+= --cpu=$(CORTEX)
CFLAGS		+= --thumb
CFLAGS		+= -DCMSDK_$(CORTEX_SHORT)
CFLAGS		+= -D$(OS)
CFLAGS		+= -D__CC_ARM -DFPGA_IMAGE
CFLAGS		+= -DportREMOVE_STATIC_QUALIFIER
CFLAGS		+= -DINCLUDE_xTaskGetIdleTaskHandle=1
CFLAGS		+= -DconfigQUEUE_REGISTRY_SIZE=8
CFLAGS		+= -DDX_PLAT_MPS2_PLUS
CFLAGS		+= -O0 -g
CFLAGS		+= -DCC_TEE -DCC_IOT -DCC_REE
CFLAGS		+= $(foreach incdir, $(INCLUDES), -I$(incdir))
CFLAGS		+= $(foreach incdir, $(INCLUDES_EXTRA), -I$(incdir))

CFLAGS		+= $(if $(findstring freertos,$(OS)), -D$(CMSIS_DRIVER))

ARFLAGS		= -rcs

else ifeq ($(COMPILER_TYPE), armclang)
LDFLAGS		+= --cpu=$(CORTEX)
LDFLAGS		+= --map --verbose
LDFLAGS		+= --list=$(PROJ_NAME).map
LDFLAGS		+= --entry=Reset_Handler
LDFLAGS		+= --dangling_debug_address 0xF0000000 # Moves unneeded debug sections

CFLAGS		+= --target=arm-arm-none-eabi
CFLAGS		+= $(if $(strip $(M_ARCH)),-march=$(M_ARCH),-mcpu=$(CORTEX))
CFLAGS		+= -mlittle-endian
CFLAGS		+= -xc -O0 -g -Wno-pragma-pack
CFLAGS		+= -mthumb
CFLAGS		+= $(if $(findstring 1,$(TZM)), -mcmse -DTZM) # Supports CMSE
CFLAGS		+= -DCORTEX_$(CORTEX_SHORT)
CFLAGS		+= -D$(CMSIS_DRIVER)
CFLAGS		+= -D$(OS)
CFLAGS		+= -DARM$(CORTEX_SHORT)
CFLAGS		+= -DconfigQUEUE_REGISTRY_SIZE=8U
CFLAGS		+= -DCC_TEE -DDX_PLAT_MPS2_PLUS
CFLAGS		+= $(if $(findstring SSE_200,$(CMSIS_DRIVER)), -mfpu=none)
CFLAGS		+= $(if $(findstring SSE_200,$(CMSIS_DRIVER)), -mcmse)
CFLAGS		+= $(foreach incdir, $(INCLUDES), -I$(incdir))
CFLAGS		+= $(foreach incdir, $(INCLUDES_EXTRA), -I$(incdir))

ARFLAGS		= -rcs

else # aarch64-linux-gnu- , arm-xilinx-linux-gnueabi- , arm-none-eabi- , arm-br-7.3 , and native
CFLAGS		+= $(if $(findstring target,$(DEVICE)), $(if $(strip $(M_ARCH)),-march=$(M_ARCH),-mcpu=$(CORTEX)),)
CFLAGS		+= -DLITTLE__ENDIAN -DHASLONGLONG
CFLAGS		+= -Wall -Wsign-compare -Wextra
CFLAGS		+= -Wstrict-prototypes -Wwrite-strings
CFLAGS		+= -Wswitch-default -Wunreachable-code
CFLAGS		+= -Winit-self -Wjump-misses-init -Wlogical-op
CFLAGS		+= -o2 -g
CFLAGS		+= $(if $(findstring 1,$(TZM)), -mcmse -DTZM) # Supports CMSE
CFLAGS		+= $(foreach incdir, $(INCLUDES), -I$(incdir))
CFLAGS		+= $(foreach incdir, $(INCLUDES_EXTRA), -I$(incdir))
CFLAGS		+= -D$(OS)
CFLAGS		+= $(if $(findstring freertos,$(OS)), -D$(CMSIS_DRIVER))

LIBS_GCC	+= -L$(CURDIR)/..
LIBS_GCC	+= -Wl,--start-group
LIBS_GCC	+= $(foreach lib, $(notdir $(LIBS)), -l$(strip $(patsubst lib%.a, %, $(lib))))
LIBS_GCC	+= -Wl,--end-group
LDFLAGS		+= -Xlinker -Map=$(BIN).map -lpthread -lm -Wl,-rpath=.

ARFLAGS		= -rcs
endif

# ****************
# Build Operations
# ****************
############ Special rules for configuration selection ##############
ifneq ($(wildcard $(PROJ_CFG_PATH)),$(PROJ_CFG_PATH))

all: # default in case there is no proj.cfg and setconfig_ was not used
	$(info Invoke 'make setconfig_<config. name>' to select project configuration )

setconfig_%: $(CONFIGS_DIR)/proj-%.cfg
	$(info [CFG] $< --> $(PROJ_CFG_PATH))
	@ln -s $< $(PROJ_CFG_PATH)

$(CONFIGS_DIR)/proj-%.cfg:
	@$(info Unknown project configuration. $@ does not exist.)
	@exit 1;

clrconfig:
	$(info No active configuration )

############ Special rules for project configuration selection ##############
else

all: $(BIN) passed

$(BIN): $(OBJ)
ifeq ($(CROSS_COMPILE),$(filter $(CROSS_COMPILE),armcc armclang)) 
#ifeq ($(CROSS_COMPILE),armcc)
	$(AR) $(ARFLAGS) $@ $^
	$(MV) $(BIN) $(BIN).a
	$(CP) $(BIN).a $(KERNEL_DIR)/lib/tests/.
#else ifeq ($(CROSS_COMPILE),armclang)	
else
	$(CC) $^ $(LIBS_GCC) $(LDFLAGS) -o $@
	$(MV) $(BIN) $(BIN).bin
endif

passed:
	@$(ECHO) $(CURDIR)/$(BIN) "Executable was Created."
	@$(ECHO) "\033[42m***************\033[0m"
	@$(ECHO) "\033[42m* P A S S E D *\033[0m"
	@$(ECHO) "\033[42m***************\033[0m"

$(SRC:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

endif

clean:
	$(RM) $(OBJ) $(notdir $(SRC:.c=.d)) $(BIN).map $(BIN).a $(BIN).bin

help:
	@echo all:	build $(BIN)
	@echo clean:	clean $(BIN) and intermediates
	@echo test:	run unit tests
	@echo help:	this text

PHONY: all clean help
