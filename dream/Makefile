################################################################################
# Dream Makefile                                                               #
################################################################################

# Project Name
NAME         := Dream

BUILD_OUT_DIR := ../$(BUILD)/$(TARGET)
LIB_OUT_DIR   := $(BUILD_OUT_DIR)/$(LIB)
INC_OUT_DIR   := $(BUILD_OUT_DIR)/$(INC)
TEST_OUT_DIR  := $(BUILD_OUT_DIR)/$(TEST)
CONF_OUT_DIR  := $(BUILD_OUT_DIR)/$(CONF)
DOC_OUT_DIR   := $(BUILD_OUT_DIR)/$(DOC)

LDFLAGS := -fPIC -L$(LIB_OUT_DIR) -Wall -lUnit -lm
GL_TEST_LDFLAGS := $(LDFLAGS) -lGL -lGLU  -lglut

# Inputs
SOURCES      := $(wildcard $(SRC)/*.c)
HEADERS      := $(wildcard $(SRC)/*.h)
OBJECTS      := $(SOURCES:$(SRC)/%.c=$(OBJ)/%.o)

TEST_SOURCES := $(wildcard $(SRC)/$(TEST)/*.c)
TEST_OBJECTS := $(TEST_SOURCES:$(SRC)/$(TEST)/%.c=$(OBJ)/%.o)

GL_TEST_SOURCES := $(wildcard $(SRC)/$(GL_TEST)/*.c)
GL_TEST_OBJECTS := $(GL_TEST_SOURCES:$(SRC)/$(GL_TEST)/%.c=$(OBJ)/%.o)

all: $(LIB_OUT_DIR)/$(OUTPUT)

# Tests irectives ##############################################################

.PHONY: run_test
run_test: $(TEST_OUT_DIR)/$(TEST_BIN)
	LD_LIBRARY_PATH=$(LIB_OUT_DIR) $(TEST_OUT_DIR)/$(TEST_BIN)

.PHONY: run_gdb_test
run_gdb_test: $(TEST_OUT_DIR)/$(TEST_BIN)
	LD_LIBRARY_PATH=$(LIB_OUT_DIR) $(GDB) $(TEST_OUT_DIR)/$(TEST_BIN)

.PHONY: run_gl_test
run_gl_test: $(TEST_OUT_DIR)/$(GL_TEST_BIN)
	LD_LIBRARY_PATH=$(LIB_OUT_DIR) $(TEST_OUT_DIR)/$(GL_TEST_BIN)

.PHONY: run_gl_test_gdb
run_gl_test_gdb: $(TEST_OUT_DIR)/$(GL_TEST_BIN)
	LD_LIBRARY_PATH=../$(LIB_OUT_DIR) $(GDB) $(BIN)/$(GL_TEST_BIN)

.PHONY: $(DOC)
$(DOC): $(DOXYFILE)
	$(DOXYGEN)

$(DOXYFILE):
	$(DOXYGEN) -g
	$(VIM) $(DOXYFILE)
	$(RM) $(DOXYFILE_BACKUP)

# Build Objects ################################################################

$(OBJECTS): $(OBJ)/%.o : $(SRC)/%.c
	@$(MKDIR) $(OBJ)
	$(CC) $(CFLAGS) -c $< -g -o $@

$(LIB_OUT_DIR)/$(OUTPUT): $(OBJECTS)
	$(CC) -shared $(OBJECTS) $(LDFLAGS) -o $@


# Test #########################################################################

.PHONY: build_test
build_test: $(TEST_OUT_DIR)/$(TEST_BIN)

$(TEST_OBJECTS): $(OBJ)/%.o : $(SRC)/$(TEST)/%.c
	@$(MKDIR) $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OUT_DIR)/$(TEST_BIN): $(OBJECTS) $(TEST_OBJECTS)
	$(CC) $(OBJECTS) $(TEST_OBJECTS) $(LDFLAGS) -o $@

# GL Test ######################################################################

.PHONY: build_gl_test
build_gl_test: $(TEST_OUT_DIR)/$(GL_TEST_BIN)

$(GL_TEST_OBJECTS): $(OBJ)/%.o : $(SRC)/$(GL_TEST)/%.c
	@$(MKDIR) $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OUT_DIR)/$(GL_TEST_BIN): $(OBJECTS) $(GL_TEST_OBJECTS)
	$(CC) $(OBJECTS) $(GL_TEST_OBJECTS) $(LDFLAGS) $(GL_TEST_LDFLAGS) -o $@

# Clean ########################################################################

.PHONY: clean
clean:
	$(RM) $(DOC)
	$(RM) $(OBJ)

# Install Headers ##############################################################
.PHONY: install_headers
install_headers:
	$(CP) $(HEADERS) $(INC_OUT_DIR)
