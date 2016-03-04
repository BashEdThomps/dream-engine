################################################################################
#                                                                              #
# Makefile                                                                     #
#                                                                              #
# Makefile for DreamEngine                                                     #
#                                                                              #
################################################################################

include def.mk

# Submodules
UNIT_DIR := unit
SCENEGRAPH_DIR := dream-scenegraph
COLLISION_WORLD_DIR := dream-collision-world
# Select target

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
	$(MAKE) -C $(UNIT_DIR)

.PHONY: make_scenegraph
make_scenegraph:
	$(MAKE) -C $(SCENEGRAPH_DIR)

.PHONY: make_collision_world
make_collision_world:
	$(MAKE) -C $(COLLISION_WORLD_DIR)

# Documents ####################################################################


# Clean ########################################################################

clean: clean_build clean_unit clean_dream_collision_world clean_dream_scenegraph

.PHONY: clean_build
clean_build:
	$(RM) build

.PHONY: clean_unit
clean_unit: 
	$(MAKE) -C $(UNIT_DIR) clean

.PHONY: clean_dream_collision_world
clean_dream_collision_world:
	$(MAKE) -C $(COLLISION_WORLD_DIR) clean

.PHONY: clean_dream_scenegraph
clean_dream_scenegraph:
	$(MAKE) -C $(SCENEGRAPH_DIR) clean
