# Docs.mk ######################################################################
DOCS         := docs
DOXYFILE     := Doxyfile
DOXYFILE_BAK := $(DOXYFILE).bak
DOCS_OUT_DIR := $(BUILD_OUT_DIR)/$(DOCS)

.PHONY: $(DOCS)
$(DOCS): $(DOXYFILE)
	$(DOXYGEN)

.PHONY: $(DOXYFILE)
$(DOXYFILE):
	$(DOXYGEN) -g
	$(VIM) $(DOXYFILE)
	$(RM)  $(DOXYFILE_BAK)
