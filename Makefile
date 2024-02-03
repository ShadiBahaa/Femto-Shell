CC = gcc
CFLAGS = -Wall -O2
SRC_DIR = src
OBJ_DIR = obj
OUTPUT_DIR = output
EXECUTABLE = femto_shell.exe

# Get all .c files in the src directory
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# Generate corresponding .o file names in the obj directory
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Default target
all: $(OUTPUT_DIR)/$(EXECUTABLE)

# Rule to compile .c files to .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to link .o files into the final executable
$(OUTPUT_DIR)/$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

# Create necessary directories
$(shell mkdir -p $(OBJ_DIR) $(OUTPUT_DIR))

# Clean target to remove generated files
clean:
	rm -rf $(OBJ_DIR) $(OUTPUT_DIR)

.PHONY: all clean