# Build.mk #####################################################################

BUILD   := build
LIB     := lib
INC     := include
OBJ     := obj
SRC     := src

SOURCES  := $(wildcard $(SRC)/*.c)
HEADERS  := $(wildcard $(SRC)/*.h)
OBJECTS  := $(SOURCES:$(SRC)/%.c=$(OBJ)/%.o)

CFLAGS  := -I../unit/src -Wall -std=c99 -fPIC

CC      := gcc
CPP     := g++
LD      := ld

BUILD_OUT_DIR := $(BUILD)/$(TARGET)
LIB_OUT_DIR := $(BUILD_OUT_DIR)/$(LIB)
