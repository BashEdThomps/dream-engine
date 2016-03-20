################################################################################
# DreamAnimation Makefile                                                      #
################################################################################

NAME := DreamAnimation

include ../mk/Build.mk
include ../mk/Test.mk
include ../mk/Headers.mk
all: $(LIB_OUT_DIR)/$(OUTPUT)

# Build Objects ################################################################

$(OBJECTS): $(OBJ)/%.o : $(SRC)/%.c
	@$(MKDIR) $(OBJ)
	$(CC) $(CFLAGS) -c $< -g -o $@

$(LIB_OUT_DIR)/$(OUTPUT): $(OBJECTS)
	$(CC) -shared $(OBJECTS) $(LDFLAGS) -o $@
