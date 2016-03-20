# Docs.mk ######################################################################
DOCS    := docs

DOXYGEN      := doxygen
DOXYFILE     := Doxyfile
DOXYFILE_BAK := $(DOXYFILE).bak

DOCS_OUT_DIR  := $(BUILD_OUT_DIR)/$(DOCS)

.PHONY: $(DOCS)
$(DOCS): $(DOXYFILE)
	$(DOXYGEN)

$(DOXYFILE):
	$(DOXYGEN) -g
	$(VIM) $(DOXYFILE)
	$(RM) $(DOXYFILE_BACKUP)
