SRC = src/project.cpp
HDR = include/*.h*

INCLUDES = -Iinclude -I/Library/Frameworks/SDL3.framework
CFLAGS = -Wall -Wextra -std=c++17 $(INCLUDES)
CC = g++
TARGET = bin/project

FRAMEWORKS := -F/Library/Frameworks -framework SDL3
RPATH := -Wl,-rpath,/Library/Frameworks
LDFLAGS := $(FRAMEWORKS) $(RPATH)

all: $(TARGET)

$(TARGET): $(SRC) $(HDR) | bin
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -rf $(BIN_DIR)