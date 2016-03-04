################################################################################
#                                                                              #
# def.mk                                                                       #
#                                                                              #
# Makefile Definitions for DreamEngine                                         #
#                                                                              #
################################################################################

# Commands
RM      := rm -rf
CP      := cp -rf
CD      := cd
MAKE    := make
MKDIR   := mkdir -p
GDB     := gdb
SUDO    := sudo
DOXYGEN := doxygen
VIM     := vim

_BUILD_DIR := build
_LIB_DIR   := lib
_INC_DIR   := include
_TEST_DIR  := test
_CONF_DIR  := conf
DOCS       := docs
SRC        := src
TEST       := $(SRC)/test
GL_TEST    := $(SRC)/gl_test

BUILD_DIR     := $(_BUILD_DIR)/$(TARGET)
LIB_OUT_DIR   := $(BUILD_DIR)/$(_LIB_DIR)
INC_OUT_DIR   := $(BUILD_DIR)/$(_INC_DIR)
TEST_OUT_DIR  := $(BUILD_DIR)/$(_TEST_DIR)
CONF_OUT_DIR  := $(BUILD_DIR)/$(_CONF_DIR)
DOCS_OUT_DIR  := $(BUILD_DIR)/$(DOCS)
