################################################################################
# DreamAnimation Makefile                                                      #
################################################################################
include ../Build.mk
include ../Test.mk
include ../Headers.mk
all: $(LIB_OUT_DIR)/$(OUTPUT)

# Build Objects ################################################################

$(OBJECTS): $(OBJ)/%.o : $(SRC)/%.c
	@$(MKDIR) $(OBJ)
	$(CC) $(CFLAGS) -c $< -g -o $@

$(LIB_OUT_DIR)/$(OUTPUT): $(OBJECTS)
	$(CC) -shared $(OBJECTS) $(LDFLAGS) -o $@
