CC := gcc
SRC_DIR := ./src
SRC := $(SRC_DIR)/main.c
BIN_DIR := bin

TARGET := $(BIN_DIR)/calc

.PHONY: all clean


all: $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TARGET) : $(SRC) | $(BIN_DIR)
	$(CC) $< -o $@

clean:
	rm -rf $(BIN_DIR)