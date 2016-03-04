################################################################################
#                                                                              #
# Makefile                                                                     #
#                                                                              #
# Makefile for DreamEngine                                                     #
#                                                                              #
################################################################################

TARGET := linux

# Submodules
UNIT := unit
SCENEGRAPH := dream-scenegraph
COLLISION_WORLD := dream-collision-world

# Commands
RM      := rm -rf
CP      := cp -rf
MAKE    := make
MKDIR   := mkdir -p

ALL   := all
BUILD := build
LIB   := lib
INC   := include
TEST  := test
CONF  := conf
DOCS  := docs

BUILD_OUT_DIR := $(BUILD)/$(TARGET)
LIB_OUT_DIR := $(BUILD_OUT_DIR)/$(LIB)
INC_OUT_DIR := $(BUILD_OUT_DIR)/$(INC)
TEST_OUT_DIR := $(BUILD_OUT_DIR)/$(TEST)
CONF_OUT_DIR := $(BUILD_OUT_DIR)/$(CONF)
DOCS_OUT_DIR := $(BUILD_OUT_DIR)/$(DOCS)

all: make_build_dirs make_unit make_scenegraph make_collision_world

# Make Rules ##################################################################

.PHONY: make_build_dirs
make_build_dirs:
	$(MKDIR) $(LIB_OUT_DIR)
	$(MKDIR) $(CONF_OUT_DIR)
	$(MKDIR) $(INC_OUT_DIR)
	$(MKDIR) $(TEST_OUT_DIR)

.PHONY: make_unit
make_unit:
	$(MAKE) -C $(UNIT) $(ALL) $(TEST)

.PHONY: make_scenegraph
make_scenegraph:
	$(MAKE) -C $(SCENEGRAPH) $(ALL) $(TEST)

.PHONY: make_collision_world
make_collision_world:
	$(MAKE) -C $(COLLISION_WORLD) $(ALL) $(TEST)

# Documents ####################################################################

.PHONY: docs
docs: docs_unit docs_dream_collision_world docs_dream_scenegraph

# Clean ########################################################################

clean: clean_build clean_unit clean_dream_collision_world clean_dream_scenegraph

.PHONY: clean_build
clean_build:
	$(RM) build

.PHONY: clean_unit
clean_unit: 
	$(MAKE) -C $(UNIT) clean

.PHONY: clean_dream_collision_world
clean_dream_collision_world:
	$(MAKE) -C $(COLLISION_WORLD) clean

.PHONY: clean_dream_scenegraph
clean_dream_scenegraph:
	$(MAKE) -C $(SCENEGRAPH) clean
