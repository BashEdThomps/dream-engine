################################################################################
# Unit Makefile                                                                #
################################################################################

# Project Name
NAME     := Unit
OUTPUT   := lib$(NAME).so
TEST_BIN := $(NAME)Test


TEST_SOURCES  := $(wildcard $(SRC)/$(TEST)/*.c)
TEST_INCLUDES := $(wildcard $(SRC)/$(TEST)/*.h)
TEST_OBJECTS  := $(TEST_SOURCES:$(SRC)/$(TEST)/%.c=$(OBJ)/%.o)

FILTERED_TEST_OBJECTS := $(filter-out obj/main.o, $(OBJECTS))
FILTERED_TEST_OBJECTS := $(FILTERED_TEST_OBJECTS) $(TEST_OBJECTS)

all: $(LIB_OUT_DIR)/$(OUTPUT)

# Target #######################################################################

$(OBJECTS): $(OBJ)/%.o : $(SRC)/%.c
	@$(MKDIR) $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_OUT_DIR)/$(OUTPUT): $(OBJECTS)
	$(CC) -shared $(OBJECTS) $(CFLAGS) $(LFLAGS) -o $@

# Tests directive ##############################################################

# Run Test
.PHONY: run_test
run_test: $(TEST_OUT_DIR)/$(TEST_BIN)
	$(TEST_OUT_DIR)/$(TEST_BIN)

# Build Test Binary
.PHONY: build_test
build_test: $(TEST_OBJECTS) $(TEST_OUT_DIR)/$(TEST_BIN)

# Link Test binary
.PHONY: $(TEST_OUT_DIR)/$(TEST_BIN)
$(TEST_OUT_DIR)/$(TEST_BIN): $(FILTERED_TEST_OBJECTS)
	$(CC) $(FILTERED_TEST_OBJECTS) $(LDFLAGS) -o $@

# Build test objects
.PHONY: $(TEST_OBJECTS)
$(TEST_OBJECTS): $(OBJ)/%.o : $(SRC)/$(TEST)/%.c
	@$(MKDIR) $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean ########################################################################

.PHONY: clean
clean:
	$(RM) $(OBJ)
	$(RM) $(DOC)

# Documentation ################################################################
.PHONY: $(DOC)
$(DOC): Doxyfile
	$(DOXYGEN)

Doxyfile:
	$(DOXYGEN) -g
	$(VIM) Doxyfile
	$(RM) Doxyfile.bak

# Install Headers ##############################################################
.PHONY: install_headers
install_headers:
	$(CP) $(HEADERS) $(INC_OUT_DIR)
