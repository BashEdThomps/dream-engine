# Test #########################################################################

TEST_SOURCES := $(wildcard $(SRC)/$(TEST)/*.c)
TEST_OBJECTS := $(TEST_SOURCES:$(SRC)/$(TEST)/%.c=$(OBJ)/%.o)

CONF    := conf
TEST    := test
CFLAGS  := $(CFLAGS) -I../unit/src

.PHONY: build_test
build_test: $(TEST_OUT_DIR)/$(TEST_BIN)

$(TEST_OBJECTS): $(OBJ)/%.o : $(SRC)/$(TEST)/%.c
	@$(MKDIR) $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OUT_DIR)/$(TEST_BIN): $(OBJECTS) $(TEST_OBJECTS)
	$(CC) $(OBJECTS) $(TEST_OBJECTS) $(LDFLAGS) -o $@
