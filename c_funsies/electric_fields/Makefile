CC := clang++
SRC := src/fields.cpp
HDR := include/*.h*
BIN_DIR := bin
TARGET := $(BIN_DIR)/fields

INCLUDES := -Iinclude -I/Library/Frameworks/SDL3.framework 
CFLAGS :=  -Wall -Wextra -std=c++17 $(INCLUDES)
FRAMEWORKS := -F/Library/Frameworks -framework SDL3
RPATH := -Wl,-rpath,/Library/Frameworks
LDFLAGS := $(FRAMEWORKS) $(RPATH)

.PHONY: all clean

all: $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TARGET): $(SRC) $(HDR) | $(BIN_DIR)  
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -rf $(BIN_DIR)