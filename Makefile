# Compiler and flags
CC = gcc
CFLAGS = -g -Iinclude
LDFLAGS = -lpthread -lsqlite3

# Directories
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include
BIN_DIR = bin
DOCS_DIR = docs

# Target executable
TARGET = $(BIN_DIR)/server

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Default target
all: $(TARGET)

# Build the target executable
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Build complete: $@"

# Compile source files to object files in OBJ_DIR
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
	@echo "Cleaned up build artifacts."

# Help target to display usage
help:
	@echo "Usage:"
	@echo "  make         - Build the server executable"
	@echo "  make clean   - Clean up build artifacts"
	@echo "  make help    - Display this help message"

# Phony targets
.PHONY: all clean help
