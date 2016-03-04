################################################################################
# Unit Makefile                                                                #
################################################################################ 

# Project Name
NAME     := Unit
OUTPUT   := lib$(NAME).so
TEST_BIN := $(NAME)Test

# Target Info
TARGET := linux
CC := gcc
CFLAGS := -Wall -std=c99 -fPIC
LD := ld 
LFLAGS := -fPIC

# Commands
RM      := rm -rf
CP      := cp -rf
CD      := cd
MAKE    := make
MKDIR   := mkdir -p
GDB     := gdb
SUDO    := sudo
DOXYGEN := doxygen
VIM     := vim

# Directories
BUILD   := build
LIB     := lib
INC     := include
TEST    := test
CONF    := conf
DOCS    := docs
OBJ     := obj
SRC     := src
TEST    := test

# Output Paths
BUILD_OUT_DIR := ../$(BUILD)/$(TARGET)
LIB_OUT_DIR   := $(BUILD_OUT_DIR)/$(LIB)
INC_OUT_DIR   := $(BUILD_OUT_DIR)/$(INC)
TEST_OUT_DIR  := $(BUILD_OUT_DIR)/$(TEST)
CONF_OUT_DIR  := $(BUILD_OUT_DIR)/$(CONF)
DOCS_OUT_DIR  := $(BUILD_OUT_DIR)/$(DOCS)

# Target Variables
SOURCES  := $(wildcard $(SRC)/*.c)
INCLUDES := $(wildcard $(SRC)/*.h)
OBJECTS  := $(SOURCES:$(SRC)/%.c=$(OBJ)/%.o)
TEST_SOURCES  := $(wildcard $(SRC)/$(TEST)/*.c)
TEST_INCLUDES := $(wildcard $(SRC)/$(TEST)/*.h)
TEST_OBJECTS  := $(TEST_SOURCES:$(TEST)/%.c=$(OBJ)/%.o)
FILTERED_TEST_OBJECTS := $(filter-out obj/main.o, $(OBJECTS))
FILTERED_TEST_OBJECTS := $(FILTERED_TEST_OBJECTS) $(TEST_OBJECTS)

all: $(LIB_OUT_DIR)/$(OUTPUT)

# Target #######################################################################

$(OBJECTS): $(OBJ)/%.o : $(SRC)/%.c
	$(MKDIR) $(OBJ) 
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_OUT_DIR)/$(OUTPUT): $(OBJECTS)
	$(CC) -shared $(OBJECTS) $(CFLAGS) $(LFLAGS) -o $@

# Tests directive ##############################################################

.PHONY: test
test: $(TEST_OUT_DIR)/$(TEST_BIN)
	$(TEST_OUT_DIR)/$(TEST_BIN) 

# Link test binary
$(TEST_OUT_DIR)/$(TEST_BIN): $(FILTERED_TEST_OBJECTS)
	$(CC) $(FILTERED_TEST_OBJECTS) $(LDFLAGS) -o $@

# Build test objects
$(TEST_OBJECTS): $(OBJ)/%.o : $(TEST)/%.c
	$(MKDIR) $(OBJ) 
	$(CC) $(CFLAGS) -c $< -o $@

# Clean ########################################################################

.PHONY: clean
clean:
	$(RM) $(OBJ)
