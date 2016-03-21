# Test #########################################################################
TEST      := test
TEST_BIN  := $(NAME)Test

TEST_SOURCES := $(wildcard $(SRC)/$(TEST)/*.c)
TEST_OBJECTS := $(TEST_SOURCES:$(SRC)/$(TEST)/%.c=$(OBJ)/%.o)

TEST_CFLAGS := $(CFLAGS) -I../dream-unit/src

.PHONY: build_test
build_test: $(TEST_OUT_DIR)/$(TEST_BIN)

.PHONY: $(TEST_OBJECTS)
$(TEST_OBJECTS): $(OBJ)/%.o : $(SRC)/$(TEST)/%.c
	@$(MKDIR) $(OBJ)
	$(CC) $(TEST_CFLAGS) -c $< -o $@

.PHONY: $(TEST_OUT_DIR)/$(TEST_BIN)
$(TEST_OUT_DIR)/$(TEST_BIN): $(OBJECTS) $(TEST_OBJECTS)
	$(CC) $(OBJECTS) $(TEST_OBJECTS) $(LDFLAGS) -o $@
