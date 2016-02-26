################################################################################
# DreamSceneGraph Makefile                                                     #
################################################################################

# Variables ####################################################################

# Project Name
TARGET = libDreamSceneGraph.so
TEST_BIN = Test
GLTEST_BIN = GLTest

# Compiler Flags
CC = gcc
CFLAGS := -std=c99 -I./unit/src -fPIC

# Linker Flags
LFLAGS := -L./unit/bin -L./bin -Wall -lUnit
GLTEST_LFLAGS := -lglfw 

# Directories
SRC_DIR = src
TEST_DIR := $(SRC_DIR)/test
GLTEST_DIR := $(SRC_DIR)/gltest
OBJ_DIR = obj
BIN_DIR = bin

# Inputs and Outputs 
SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TEST_SOURCES := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS := $(TEST_SOURCES:$(TEST_DIR)/%.c=$(OBJ_DIR)/%.o)

GLTEST_SOURCES := $(wildcard $(GLTEST_DIR)/*.c)
GLTEST_OBJECTS := $(GLTEST_SOURCES:$(GLTEST_DIR)/%.c=$(OBJ_DIR)/%.o)

# Commands
RM    = rm -rf
MKDIR = mkdir -p
ECHO  = echo
CP    = cp
GDB   = gdb

all: $(BIN_DIR)/$(TARGET)

# Tests irectives ##############################################################

.PHONY: test
test: $(BIN_DIR)/$(TEST_BIN)
	LD_LIBRARY_PATH=unit/bin $(BIN_DIR)/$(TEST_BIN) 

.PHONY: gdb_test
gdb_test: $(BIN_DIR)/$(TEST_BIN)
	LD_LIBRARY_PATH=unit/bin $(GDB) $(BIN_DIR)/$(TEST_BIN) 

.PHONY: gl_test
gl_test: $(BIN_DIR)/$(GLTEST_BIN)
	$(BIN_DIR)/$(GLTEST_BIN) 

# Build Objects ################################################################

$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(MKDIR) obj
	$(CC) $(CFLAGS) -c $< -g -o $@

$(TEST_OBJECTS): $(OBJ_DIR)/%.o : $(TEST_DIR)/%.c
	$(MKDIR) obj
	$(CC) $(CFLAGS) -c $< -o $@

$(GLTEST_OBJECTS): $(OBJ_DIR)/%.o : $(GLTEST_DIR)/%.c
	$(MKDIR) obj
	$(CC) $(CFLAGS) -c $< -o $@

# Build Binaries ###############################################################

$(BIN_DIR)/$(TARGET): $(OBJECTS)
	$(MKDIR) bin
	$(CC) -shared $(OBJECTS) $(LFLAGS) -o $@

$(BIN_DIR)/$(TEST_BIN): $(OBJECTS) $(TEST_OBJECTS)
	$(MKDIR) bin
	$(CC) $(OBJECTS) $(TEST_OBJECTS) $(LFLAGS) -o $@

$(BIN_DIR)/$(GLTEST_BIN): $(OBJECTS) $(GLTEST_OBJECTS)
	$(MKDIR) bin
	$(CC) $(OBJECTS) $(GLTEST_OBJECTS) $(LFLAGS) $(GLTEST_LFLAGS) -o $@

# Clean ######################################################################## 

.PHONY: clean
clean:
	$(RM) $(OBJ_DIR)
	$(RM) $(BIN_DIR)
