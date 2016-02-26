# ------------------------------------------------
# DreamSceneGraph Makefile
# ------------------------------------------------

# Project Name
TARGET   = libDreamScene 
TEST_BIN = Test

# Compiler & Flags
CC        = gcc
CFLAGS   := -std=c99 \
            -I./unit/src \
            -fPIC

# Linker & Flags
LFLAGS   := -L./unit/bin \
            -Wall -lUnit

# Directories
SRC_DIR   = src
TEST_DIR := $(SRC_DIR)/test
OBJ_DIR   = obj
BIN_DIR   = bin

# Target Variables
SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TEST_SOURCES := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJECTS := $(TEST_SOURCES:$(TEST_DIR)/%.c=$(OBJ_DIR)/%.o)

# Utils
RM    = rm -rf
MKDIR = mkdir -p
ECHO  = echo
CP    = cp

# Link target binary
$(BIN_DIR)/$(TARGET).so: $(OBJECTS)
	$(MKDIR) bin
	$(CC) -shared $(OBJECTS) $(LFLAGS) -o $@

# Tests directive
.PHONY: test
test: $(BIN_DIR)/$(TEST_BIN)
	LD_LIBRARY_PATH=unit/bin $(BIN_DIR)/$(TEST_BIN) 

.PHONY: gdb_test
gdb_test: $(BIN_DIR)/$(TEST_BIN)
	LD_LIBRARY_PATH=unit/bin gdb $(BIN_DIR)/$(TEST_BIN) 


# Link test binary
$(BIN_DIR)/$(TEST_BIN): $(OBJECTS) $(TEST_OBJECTS)
	$(MKDIR) bin
	$(CC) $(OBJECTS) $(TEST_OBJECTS) $(LFLAGS) -o $@

# Build target objects
$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(MKDIR) obj
	$(CC) $(CFLAGS) -c $< -g -o $@

# Build test objects
$(TEST_OBJECTS): $(OBJ_DIR)/%.o : $(TEST_DIR)/%.c
	$(MKDIR) obj
	$(CC) $(CFLAGS) -c $< -o $@

# Clean environment
.PHONY: clean
clean:
	$(RM) $(OBJ_DIR)
	$(RM) $(BIN_DIR)
