CC := clang
SRC := src/conway.c
BIN_DIR := bin
TARGET := $(BIN_DIR)/conway

INCLUDES := -Iinclude -I/Library/Frameworks/SDL3.framework 
CFLAGS :=  -std=c11 $(INCLUDES) 
FRAMEWORKS := -F/Library/Frameworks -framework SDL3
RPATH := -Wl,-rpath,/Library/Frameworks
LDFLAGS := $(FRAMEWORKS) $(RPATH)

.PHONY: all clean

all: $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TARGET): $(SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -rf $(BIN_DIR)