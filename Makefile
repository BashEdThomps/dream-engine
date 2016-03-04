################################################################################
#                                                                              #
# Makefile                                                                     #
#                                                                              #
# Makefile for DreamEngine                                                     #
#                                                                              #
################################################################################

# Submodules
UNIT := unit
SCENEGRAPH := dream-scenegraph
COLLISION_WORLD := dream-collision-world

all: setup make_build_dirs make_unit make_scenegraph make_collision_world

# Setup Make ###################################################################

.PHONY: setup
setup:
# Directories
ifeq '$(TARGET)' 'linux'
include linux.mk
endif
ifeq '$(TARGET)' 'psp'
include psp.mk
endif
ifeq '$(TARGET)' 'dreamcast'
include dreamcast.mk
endif
include def.mk
$(info Executing Makefile for DreamEngine with target: $(TARGET))
$(info CC: $(CC))
$(info CFLAGS: $(CFLAS))
$(info LD: $(LD))
$(info LFLAGS: $(LFLAGS))


# Make Rules ##################################################################

.PHONY: make_build_dirs
make_build_dirs:
	$(MKDIR) $(LIB_OUT_DIR)
	$(MKDIR) $(CONF_OUT_DIR)
	$(MKDIR) $(INC_OUT_DIR)
	$(MKDIR) $(TEST_OUT_DIR)

.PHONY: make_unit
make_unit:
	$(MAKE) -C $(UNIT)

.PHONY: make_scenegraph
make_scenegraph:
	$(MAKE) -C $(SCENEGRAPH)

.PHONY: make_collision_world
make_collision_world:
	$(MAKE) -C $(COLLISION_WORLD)

# Documents ####################################################################


# Clean ########################################################################

clean: setup clean_build clean_unit clean_dream_collision_world clean_dream_scenegraph

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
