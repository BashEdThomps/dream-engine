################################################################################
# Makefile for DreamEngine                                                     #
################################################################################

BUILD_BASE          := build
UNIT_DIR            := unit
SCENEGRAPH_DIR      := dream-scenegraph
COLLISION_WORLD_DIR := dream-collision-world

# Commands
RM   := rm -rf
CP   := cp -rf
MAKE := make

#include Makefile.psp
#include Makefile.dreamcast
include Makefile.linux

BUILD_DIR := $(BUILD_BASE)/$(BUILD_TARGET)
LIB_DIR   := $(BUILD_DIR)/lib

all: make_unit make_scenegraph make_collision_world

# Make Rules ##################################################################

.PHONY: make_unit
make_unit:
	$(CD) $(UNIT_DIR)
	$(MAKE)

.PHONY: make_scenegraph
make_scenegraph:
	$(CD) $(SCENEGRAPH_DIR)
	$(MAKE)

.PHONY: make_collision_world
make_collision_world:
	$(CD) $(COLLISION_WORLD_DIR)
	$(MAKE)

# Documents ####################################################################


# Clean ########################################################################

clean: clean_build clean_unit clean_dream_collision_world clean_dream_scenegraph

.PHONY: clean_build
clean_build:
	$(RM) build

.PHONY: clean_unit
clean_unit: 
	$(CD) $(UNIT_DIR)
	$(MAKE) clean

.PHONY: clean_dream_collision_world
clean_dream_collision_world:
	$(CD) $(COLLISION_WORLD_DIR)
	$(MAKE) clean

.PHONY: clean_dream_scenegraph
clean_dream_scenegraph:
	$(CD) $(SCENEGRAPH_DIR)
	$(MAKE) clean
