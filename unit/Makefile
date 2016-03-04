################################################################################
# Unit Makefile                                                                #
################################################################################ 

include ../def.mk

# Project Name
OUTPUT   = libUnit
TEST_BIN = $(OUTPUT)Test

# Directories
SRC_DIR   = src
TEST_DIR  = test
OBJ_DIR   = obj

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

# Link target binary ###########################################################

$(BUILD_DIR)/$(LIB_DIR)/$(OUTPUT).so: $(OBJECTS)
	$(CC) -shared $(OBJECTS) $(LFLAGS) -o $@

# Tests directive ##############################################################

.PHONY: test
test: $(BUILD_DIR)/$(TEST_DIR)/$(TEST_BIN)
	$(BUILD_DIR)/$(TEST_DIR)/$(TEST_BIN) 

# Link test binary
$(BUILD_DIR)/$(TEST_DIR)/$(TEST_BIN): $(FILTERED_TEST_OBJECTS)
	$(CC) $(FILTERED_TEST_OBJECTS) $(LFLAGS) -o $@

# Build target objects
$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(MKDIR) obj
	$(CC) $(CFLAGS) -c $< -o $@

# Build test objects
$(TEST_OBJECTS): $(OBJ_DIR)/%.o : $(TEST_DIR)/%.c
	$(MKDIR) obj
	$(CC) $(CFLAGS) -c $< -o $@

# Clean ########################################################################

.PHONY: clean
clean:
	$(RM) $(OBJ_DIR)
