################################################################################
# DreamSceneGraph Makefile                                                     #
################################################################################

# Variables ####################################################################

# Project Name
TARGET = libDreamSceneGraph.so
TEST_BIN = Test
GL_TEST_BIN = GLTest

# Compiler Flags
CC = gcc
CFLAGS := -std=c99 -I./unit/src -fPIC

# Linker Flags
LFLAGS := -L./unit/bin -Wall -lUnit -lm
GL_TEST_LFLAGS := -lGL -lGLU  -lglut

# Directories
SRC_DIR = src
TEST_DIR := $(SRC_DIR)/test
GL_TEST_DIR := $(SRC_DIR)/gl_test
OBJ_DIR = obj
BIN_DIR = bin

# Inputs and Outputs 
SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TEST_SOURCES := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS := $(TEST_SOURCES:$(TEST_DIR)/%.c=$(OBJ_DIR)/%.o)

GL_TEST_SOURCES := $(wildcard $(GL_TEST_DIR)/*.c)
GL_TEST_OBJECTS := $(GL_TEST_SOURCES:$(GL_TEST_DIR)/%.c=$(OBJ_DIR)/%.o)

HEADERS := $(wildcard $(SRC_DIR)/*.h)

PREFIX=/usr/local

# Commands
RM      = rm -rf
MKDIR   = mkdir -p
ECHO    = echo
CP      = cp
GDB     = gdb
SUDO    = sudo
DOXYGEN = doxygen

all: $(BIN_DIR)/$(TARGET)

# Tests irectives ##############################################################

.PHONY: test
test: $(BIN_DIR)/$(TEST_BIN)
	LD_LIBRARY_PATH=unit/bin $(BIN_DIR)/$(TEST_BIN) 

.PHONY: gdb_test
gdb_test: $(BIN_DIR)/$(TEST_BIN)
	LD_LIBRARY_PATH=unit/bin $(GDB) $(BIN_DIR)/$(TEST_BIN) 

.PHONY: gl_test
gl_test: $(BIN_DIR)/$(GL_TEST_BIN)
	$(BIN_DIR)/$(GL_TEST_BIN) 

.PHONY: gl_test_gdb
gl_test_gdb: $(BIN_DIR)/$(GL_TEST_BIN)
	$(GDB) $(BIN_DIR)/$(GL_TEST_BIN) 

.PHONY: docs
docs: Doxyfile
	$(DOXYGEN)

Doxyfile: 
	$(DOXYGEN) -g
	vim Doxyfile
	$(RM) Doxyfile.bak

# Build Objects ################################################################

$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(MKDIR) obj
	$(CC) $(CFLAGS) -c $< -g -o $@

$(TEST_OBJECTS): $(OBJ_DIR)/%.o : $(TEST_DIR)/%.c
	$(MKDIR) obj
	$(CC) $(CFLAGS) -c $< -o $@

$(GL_TEST_OBJECTS): $(OBJ_DIR)/%.o : $(GL_TEST_DIR)/%.c
	$(MKDIR) obj
	$(CC) $(CFLAGS) -c $< -o $@

# Build Binaries ###############################################################

$(BIN_DIR)/$(TARGET): $(OBJECTS)
	$(MKDIR) bin
	$(CC) -shared $(OBJECTS) $(LFLAGS) -o $@

$(BIN_DIR)/$(TEST_BIN): $(OBJECTS) $(TEST_OBJECTS)
	$(MKDIR) bin
	$(CC) $(OBJECTS) $(TEST_OBJECTS) $(LFLAGS) -o $@

$(BIN_DIR)/$(GL_TEST_BIN): $(OBJECTS) $(GL_TEST_OBJECTS)
	$(MKDIR) bin
	$(CC) $(OBJECTS) $(GL_TEST_OBJECTS) $(LFLAGS) $(GL_TEST_LFLAGS) -o $@

# Clean ######################################################################## 

.PHONY: clean
clean:
	$(RM) docs 
	$(RM) $(OBJ_DIR)
	$(RM) $(BIN_DIR)

# Install ######################################################################

install: install_lib

.PHONY: install_lib
install_lib: $(BIN_DIR)/$(TARGET) $(SRC)/$(HEADERS)
	$(SUDO) $(CP) $(BIN_DIR)/$(TARGET) $(PREFIX)/lib/$(TARGET)

.PHONY: install_headers
install_headers: $(SRC)/$(HEADERS) 
	$(SUDO) $(CP) $(SRC)/$@ $(PREFIX)/include/$@
