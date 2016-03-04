################################################################################
# DreamSceneGraph Makefile                                                     #
################################################################################

# Variables ####################################################################

# Project Name
NAME = DreamSceneGraph
OUTPUT = lib$(NAME).so
TEST_BIN = Test
GL_TEST_BIN = GLTest

LDFLAGS := $(LDFLAGS) -L../$(LIB_OUT_DIR) -Wall -lUnit -lm
GL_TEST_LDFLAGS := $(LDFLAGS) -lGL -lGLU  -lglut

# Inputs and Outputs 
SOURCES := $(wildcard $(SRC)/*.c)
OBJECTS := $(SOURCES:$(SRC)/%.c=$(OBJ)/%.o)

TEST_SOURCES := $(wildcard $(TEST)/*.c)
TEST_OBJECTS := $(TEST_SOURCES:$(TEST)/%.c=$(OBJ)/%.o)

GL_TEST_SOURCES := $(wildcard $(GL_TEST)/*.c)
GL_TEST_OBJECTS := $(GL_TEST_SOURCES:$(GL_TEST)/%.c=$(OBJ)/%.o)

# Setup Make ###################################################################

.PHONY: setup
setup:
# Directories
ifeq '$(TARGET)' 'linux'
include ../linux.mk
endif
ifeq '$(TARGET)' 'psp'
include ../psp.mk
endif
ifeq '$(TARGET)' 'dreamcast'
include ../dreamcast.mk
endif
include ../def.mk
$(info Executing Makefile for DreamSceneGraph with target: $(TARGET))

all: setup ../$(LIB_OUT_DIR)/$(OUTPUT)

# Tests irectives ##############################################################

.PHONY: test
test: ../$(TEST_OUT_DIR)/$(TEST_BIN)
	LD_LIBRARY_PATH=../$(LIB_OUT_DIR) $(TEST_OUT_DIR)/$(TEST_BIN) 

.PHONY: gdb_test
gdb_test: ../$(TEST_OUT_DIR)/$(TEST_BIN)
	LD_LIBRARY_PATH=../$(LIB_OUT_DIR) $(GDB) $(TEST_OUT_DIR)/$(TEST_BIN) 

.PHONY: gl_test
gl_test: ../$(TEST_OUT_DIR)/$(GL_TEST_BIN)
	LD_LIBRARY_PATH=../$(LIB_OUT_DIR) ../$(TEST_OUT_DIR)/$(GL_TEST_BIN) 

.PHONY: gl_test_gdb
gl_test_gdb: ../$(TEST_OUT_DIR)/$(GL_TEST_BIN)
	LD_LIBRARY_PATH=../$(LIB_OUT_DIR) $(GDB) $(BIN)/$(GL_TEST_BIN) 

.PHONY: $(DOCS)
$(DOCS): Doxyfile
	$(DOXYGEN)

Doxyfile: 
	$(DOXYGEN) -g
	$(VIM) Doxyfile
	$(RM) Doxyfile.bak

# Build Objects ################################################################

$(OBJECTS): $(OBJ)/%.o : $(SRC)/%.c
	$(MKDIR) $(OBJ) 
	$(CC) $(CFLAGS) -c $< -g -o $@

$(TEST_OBJECTS): $(OBJ)/%.o : $(TEST)/%.c
	$(MKDIR) $(OBJ) 
	$(CC) $(CFLAGS) -c $< -o $@

$(GL_TEST_OBJECTS): $(OBJ)/%.o : $(GL_TEST)/%.c
	$(MKDIR) $(OBJ) 
	$(CC) $(CFLAGS) -c $< -o $@

# Build Binaries ###############################################################

../$(LIB_OUT_DIR)/$(OUTPUT): $(OBJECTS)
	$(CC) -shared $(OBJECTS) $(LDFLAGS) -o $@

../$(TEST_OUT_DIR)/$(TEST_BIN): $(OBJECTS) $(TEST_OBJECTS)
	$(CC) $(OBJECTS) $(TEST_OBJECTS) $(LDFLAGS) -o $@

../$(TEST_OUT_DIR)/$(GL_TEST_BIN): $(OBJECTS) $(GL_TEST_OBJECTS)
	$(CC) $(OBJECTS) $(GL_TEST_OBJECTS) $(LDFLAGS) $(GL_TEST_LDFLAGS) -o $@

# Clean ######################################################################## 

.PHONY: clean
clean:
	$(RM) $(DOCS) 
	$(RM) $(OBJ)
