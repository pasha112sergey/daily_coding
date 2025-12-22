CC := gcc
SRC_DIR := src
HDR := include/*.h
SRC1 := $(SRC_DIR)/main.c
SRC2 := $(SRC_DIR)/stack.c
BIN_DIR := bin

TARGET := $(BIN_DIR)/calc

INCLUDES := -Iinclude
CFLAGS := -std=c11 $(INCLUDES)


.PHONY: all clean


all: $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TARGET) : $(SRC1) $(SRC2) $(HDR) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC1) $(SRC2) -o $@

clean:
	rm -rf $(BIN_DIR)