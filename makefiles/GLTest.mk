# GL Test ######################################################################

GL_TEST := gl_test

GL_TEST_SOURCES := $(wildcard $(SRC)/$(GL_TEST)/*.c)
GL_TEST_OBJECTS := $(GL_TEST_SOURCES:$(SRC)/$(GL_TEST)/%.c=$(OBJ)/%.o)

.PHONY: build_gl_test
build_gl_test: $(TEST_OUT_DIR)/$(GL_TEST_BIN)

$(GL_TEST_OBJECTS): $(OBJ)/%.o : $(SRC)/$(GL_TEST)/%.c
	@$(MKDIR) $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OUT_DIR)/$(GL_TEST_BIN): $(OBJECTS) $(GL_TEST_OBJECTS)
	$(CC) $(OBJECTS) $(GL_TEST_OBJECTS) $(LDFLAGS) $(GL_TEST_LDFLAGS) -o $@
