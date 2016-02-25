# ------------------------------------------------
# DreamScene Makefile
# ------------------------------------------------

# Project Name
TARGET   = libDreamScene 
TEST_BIN = $(TARGET)_test

# Compiler & Flags
CC       = gcc
CFLAGS   := -std=c99 \
            -Iunit/src \
            -fPIC

# Linker & Flags
LINKER   = gcc
LFLAGS   := -Lunit/bin \
            -Wall 

# Directories
SRC_DIR   = src
H_DIR     = headers
TEST_DIR  = test
OBJ_DIR   = obj
BIN_DIR   = bin

# Target Variables
SOURCES  := $(wildcard $(SRC_DIR)/*.c)
INCLUDES := $(wildcard $(SRC_DIR)/*.h)
OBJECTS  := $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Test Variables
TEST_SOURCES   := $(wildcard $(TEST_DIR)/*.c)
TEST_INCLUDES  := $(wildcard $(TEST_DIR)/*.h)
TEST_OBJECTS   := $(TEST_SOURCES:$(TEST_DIR)/%.c=$(OBJ_DIR)/%.o)
FILTERED_TEST_OBJECTS  := $(filter-out obj/main.o, $(OBJECTS))
FILTERED_TEST_OBJECTS  := $(FILTERED_TEST_OBJECTS) $(TEST_OBJECTS)

# Utils
rm       = rm -rf
mkdir    = mkdir -p
echo     = echo
cp       = cp

# Link target binary
$(BIN_DIR)/$(TARGET).so: $(OBJECTS)
	$(mkdir) bin
	$(LINKER) -shared $(OBJECTS) $(LFLAGS) -o $@

# Tests directive
.PHONY: test
test: $(BIN_DIR)/$(TEST_BIN)
	$(BIN_DIR)/$(TEST_BIN) $(TEST_CONF_FILE)

# Link test binary
$(BIN_DIR)/$(TEST_BIN): $(FILTERED_TEST_OBJECTS)
	$(mkdir) bin
	$(LINKER) $(FILTERED_TEST_OBJECTS) $(LFLAGS) -o $@

# Build target objects
$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(mkdir) obj
	$(CC) $(CFLAGS) -c $< -o $@

# Build test objects
$(TEST_OBJECTS): $(OBJ_DIR)/%.o : $(TEST_DIR)/%.c
	$(mkdir) obj
	$(CC) $(CFLAGS) -c $< -o $@

# Clean environment
.PHONY: clean
clean:
	$(rm) $(OBJ_DIR)
	$(rm) $(BIN_DIR)
