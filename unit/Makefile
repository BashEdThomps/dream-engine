################################################################################
# Unit Makefile                                                                #
################################################################################ 

# Project Name
NAME     := libUnit
OUTPUT   := $(NAME).so
TEST_BIN := $(OUTPUT)Test

# Directories
SRC  := src
TEST := test
OBJ  := obj

# Target Variables
SOURCES  := $(wildcard $(SRC)/*.c)
INCLUDES := $(wildcard $(SRC)/*.h)
OBJECTS  := $(SOURCES:$(SRC)/%.c=$(OBJ)/%.o)

# Test Variables
TEST_SOURCES  := $(wildcard $(TEST)/*.c)
TEST_INCLUDES := $(wildcard $(TEST)/*.h)
TEST_OBJECTS  := $(TEST_SOURCES:$(TEST)/%.c=$(OBJ)/%.o)

FILTERED_TEST_OBJECTS := $(filter-out obj/main.o, $(OBJECTS))
FILTERED_TEST_OBJECTS := $(FILTERED_TEST_OBJECTS) $(TEST_OBJECTS)

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
$(info Executing Makefile for Unit with target: $(TARGET))

all: setup ../$(LIB_OUT_DIR)/$(OUTPUT)

# Target Lib ###################################################################

$(OBJECTS): $(OBJ)/%.o : $(SRC)/%.c
	$(MKDIR) $(OBJ) 
	$(CC) $(CFLAGS) -c $< -o $@


../$(LIB_OUT_DIR)/$(OUTPUT): $(OBJECTS)
	$(CC) -shared $(OBJECTS) $(CFLAGS) $(LFLAGS) -o $@

# Tests directive ##############################################################

.PHONY: test
test: ../$(TEST_OUT_DIR)/$(TEST_BIN)
	../$(TEST_OUT_DIR)/$(TEST_BIN) 

# Link test binary
../$(TEST_OUT_DIR)/$(TEST_BIN): $(FILTERED_TEST_OBJECTS)
	$(CC) $(FILTERED_TEST_OBJECTS) $(LDFLAGS) -o $@

# Build test objects
$(TEST_OBJECTS): $(OBJ)/%.o : $(TEST)/%.c
	$(MKDIR) obj
	$(CC) $(CFLAGS) -c $< -o $@

# Clean ########################################################################

.PHONY: clean
clean:
	$(RM) $(OBJ)
