# Build.mk #####################################################################

OUTPUT  := lib$(NAME).so

BUILD   := build
LIB     := lib
INC     := include
OBJ     := obj
SRC     := src

SOURCES  := $(wildcard $(SRC)/*.c)
HEADERS  := $(wildcard $(SRC)/*.h)
OBJECTS  := $(SOURCES:$(SRC)/%.c=$(OBJ)/%.o)

CFLAGS  := -Wall -std=c99 -fPIC
BUILD_OUT_DIR := $(BUILD)
LIB_OUT_DIR := $(BUILD_OUT_DIR)/$(LIB)
