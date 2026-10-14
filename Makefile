# Compiler
CC = mpicc

# Directories
SRC = ./src
INCLUDE = ./include
BIN = ./bin
BUILD = ./build
RESULTS = ./results
INPUT_FILES = ./input_files

# Compiler flags
CFLAGS = -pthread -Wall -g3 -std=c99 -I$(INCLUDE) -fopenmp

# Source files
GAME_SRCS := $(wildcard $(SRC)/game_of_life/*.c)
MULTIPLY_SRCS := $(wildcard $(SRC)/multiply/*.c)
COMMON_SRC = $(SRC)/common.c

# Object files
GAME_OBJS := $(patsubst $(SRC)/game_of_life/%.c,$(BUILD)/game_of_life_%.o,$(GAME_SRCS))
MULTIPLY_OBJS := $(patsubst $(SRC)/multiply/%.c,$(BUILD)/multiply_%.o,$(MULTIPLY_SRCS))
COMMON_OBJ = $(BUILD)/common.o

# Executable names
game_of_life = $(BIN)/game_of_life
multiply = $(BIN)/multiply

# Create directories if they don't exist
.PHONY: directories
directories:
	@mkdir -p $(BIN) $(BUILD) $(RESULTS) $(INPUT_FILES)

# Rule to build object files from source files
$(BUILD)/game_of_life_%.o: $(SRC)/game_of_life/%.c | directories
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD)/multiply_%.o: $(SRC)/multiply/%.c | directories
	$(CC) $(CFLAGS) -c -o $@ $<

# Rule to build the common object file
$(COMMON_OBJ): $(COMMON_SRC) | directories
	$(CC) $(CFLAGS) -c -o $@ $<

# Rule to build the game_of_life executable
$(game_of_life): $(GAME_OBJS) $(COMMON_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to build the multiply executable
$(multiply): $(MULTIPLY_OBJS) $(COMMON_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Clean rule
.PHONY: clean
clean:
	rm -rf $(BIN) $(BUILD) $(RESULTS)

# Game of life rule
.PHONY: game_of_life
game_of_life: $(game_of_life)

# Multiply rule
.PHONY: multiply
multiply: $(multiply)

# All rule
.PHONY: all
all: $(game_of_life) $(multiply)