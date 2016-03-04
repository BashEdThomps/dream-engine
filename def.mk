################################################################################
#                                                                              #
# def.mk                                                                       #
#                                                                              #
# Makefile Definitions for DreamEngine                                         #
#                                                                              #
################################################################################

# Commands
RM := rm -rf
CP := cp -rf
CD := cd
MAKE := make
ECHO := echo
MKDIR := mkdir -p
GDB := gdb
SUDO := sudo
DOXYGEN := doxygen

# Directories
ifeq ($(TARGET),'linux')
 $(ECHO) "Target is linux"
 include ./linux.mk
endif

ifeq ($(TARGET),'psp')
 $(ECHO) "Target is psp"
 include ./psp.mk)
endif

ifeq ($(TARGET),'dreamcast')
 $(ECHO) "Target is dreamcast"
 include ./dreamcast.mk
endif

_BUILD_DIR := build
_LIB_DIR   := lib
_INC_DIR   := include
_TEST_DIR  := test
_CONF_DIR  := conf

BUILD_DIR := $(_BUILD_DIR)/$(TARGET)
LIB_OUT_DIR   := $(BUILD_DIR)/$(_LIB_DIR)
INC_OUT_DIR   := $(BUILD_DIR)/$(_INC_DIR)
TEST_OUT_DIR  := $(BUILD_DIR)/$(_TEST_DIR)
CONF_OUT_DIR  := $(BUILD_DIR)/$(_CONF_DIR)
