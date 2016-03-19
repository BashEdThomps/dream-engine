################################################################################
# Makefile for Dream                                                           #
################################################################################


# Submodules
UNIT            := unit
JSMN            := jsmn
SCENEGRAPH      := dream-scenegraph
COLLISION_WORLD := dream-collision-world
ANIMATION       := dream-animation

include makefiles/Commands.mk

ALL        := all
BUILD      := build
LIB        := lib
INC        := include
TEST       := test
BUILD_TEST := build_test
RUN_TEST   := run_test
CONF       := conf
DOC        := doc

INC_OUT_DIR := $(BUILD_OUT_DIR)/$(INC)
TEST_OUT_DIR := $(BUILD_OUT_DIR)/$(TEST)
CONF_OUT_DIR := $(BUILD_OUT_DIR)/$(CONF)

all: build \
	doc \
	install_headers \
	build_test \
	run_test

# Make Rules ###################################################################

build: build_build_dirs \
	build_unit \
	build_dream_collision_world \
	build_dream_animation \
	build_dream_scenegraph

.PHONY: build_build_dirs
build_build_dirs:
	$(MKDIR) $(LIB_OUT_DIR)
	$(MKDIR) $(CONF_OUT_DIR)
	$(MKDIR) $(INC_OUT_DIR)
	$(MKDIR) $(TEST_OUT_DIR)
	$(MKDIR) $(DOC_OUT_DIR)

.PHONY: build_unit
build_unit:
	$(MAKE) -C $(UNIT) $(ALL) -f Makefile

.PHONY: build_dream_scenegraph
build_dream_scenegraph:
	$(MAKE) -C $(SCENEGRAPH) $(ALL) -f Makefile

.PHONY: build_dream_collision_world
build_dream_collision_world:
	$(MAKE) -C $(COLLISION_WORLD) $(ALL) -f Makefile

.PHONY: build_dream_animation
build_dream_animation:
	$(MAKE) -C $(ANIMATION) $(ALL) -f Makefile

# Build Test ###################################################################

build_test: build_unit_test \
	build_dream_collision_world_test \
	build_dream_animation_test \
	build_dream_scenegraph_test

.PHONY: build_unit_test
build_unit_test:
	$(MAKE) -C $(UNIT) $(BUILD_TEST) -f Makefile

.PHONY: build_dream_scenegraph_test
build_dream_scenegraph_test:
	$(MAKE) -C $(SCENEGRAPH) $(BUILD_TEST) -f Makefile

.PHONY: build_dream_collision_world_test
build_dream_collision_world_test:
	$(MAKE) -C $(COLLISION_WORLD) $(BUILD_TEST) -f Makefile

.PHONY: build_dream_animation_test
build_dream_animation_test:
	$(MAKE) -C $(ANIMATION) $(BUILD_TEST) -f Makefile

# Run Test #####################################################################

run_test: run_unit_test \
	run_dream_animation_test \
	run_dream_collision_world_test \
	run_dream_scenegraph_test

.PHONY: run_test_unit
run_unit_test:
	$(MAKE) -C $(UNIT) $(RUN_TEST) -f Makefile

.PHONY: run_test_dream_scenegraph
run_dream_scenegraph_test:
	$(MAKE) -C $(SCENEGRAPH) $(RUN_TEST) -f Makefile

.PHONY: run_dream_collision_world_test
run_dream_collision_world_test:
	$(MAKE) -C $(COLLISION_WORLD) $(RUN_TEST) -f Makefile

.PHONY: run_dream_animation_test
run_dream_animation_test:
	$(MAKE) -C $(ANIMATION) $(RUN_TEST) -f Makefile

# Documents ####################################################################

doc: doc_unit \
	doc_dream_collision_world \
	doc_dream_animation \
	doc_dream_scenegraph

.PHONY: doc_unit
doc_unit:
	$(MAKE) -C $(UNIT) $(DOC) -f Makefile

.PHONY: doc_dream_scenegraph
doc_dream_scenegraph:
	$(MAKE) -C $(SCENEGRAPH) $(DOC) -f Makefile

.PHONY: doc_dream_collision_world
doc_dream_collision_world:
	$(MAKE) -C $(COLLISION_WORLD) $(DOC) -f Makefile

.PHONY: doc_dream_animation
doc_dream_animation:
	$(MAKE) -C $(ANIMATION) $(DOC) -f Makefile

# Clean ########################################################################

clean: clean_build \
	clean_unit \
	clean_dream_collision_world \
	clean_dream_animation \
	clean_dream_scenegraph

.PHONY: clean_build
clean_build:
	$(RM) build

.PHONY: clean_unit
clean_unit:
	$(MAKE) -C $(UNIT) clean -f Makefile

.PHONY: clean_dream_collision_world
clean_dream_collision_world:
	$(MAKE) -C $(COLLISION_WORLD) clean -f Makefile

.PHONY: clean_dream_scenegraph
clean_dream_scenegraph:
	$(MAKE) -C $(SCENEGRAPH) clean -f Makefile

.PHONY: clean_dream_animation
clean_dream_animation:
	$(MAKE) -C $(ANIMATION) clean -f Makefile

# Install Headers ##############################################################
.PHONY: install_headers
install_headers: install_headers_unit \
                 install_headers_dream_animation \
				 install_headers_dream_collision_world \
				 install_headers_dream_scenegraph

.PHONY: install_headers_unit
install_headers_unit:
	$(MAKE) -C $(UNIT) install_headers -f Makefile

.PHONY: install_headers_dream_animation
install_headers_dream_animation:
	$(MAKE) -C $(ANIMATION) install_headers -f Makefile

.PHONY: install_headers_dream_collision_world
install_headers_dream_collision_world:
	$(MAKE) -C $(COLLISION_WORLD) install_headers -f Makefile

.PHONY: install_headers_dream_scenegraph
install_headers_dream_scenegraph:
	$(MAKE) -C $(SCENEGRAPH) install_headers -f Makefile
