################################################################################
# DreamUnit Makefile                                                           #
################################################################################

NAME := DreamUnit


include ../mk/common.mk


all: $(LIB_OUT_DIR)/$(OUTPUT)
	
$(OBJECTS): $(OBJ)/%.o : $(SRC)/%.c
	@$(MKDIR) $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_OUT_DIR)/$(OUTPUT): $(OBJECTS)
	$(CC) -shared $(OBJECTS) $(CFLAGS) $(LFLAGS) -o $@
