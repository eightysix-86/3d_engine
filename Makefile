# Compiler and flags
CC = gcc
CFLAGS = `sdl2-config --cflags` -Wall -Wextra -O2 -Iinclude -I/opt/homebrew/include/SDL2 -MMD -MP
LDFLAGS = `sdl2-config --libs` -L/opt/homebrew/lib -lm

# Engine sources (exclude tests and main)
SRC = $(shell find src -name '*.c')
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

# Build directory
BUILD_DIR = build

# Main executable
TARGET = $(BUILD_DIR)/3d_engine
MAIN_OBJ = main.o

# Tests
TEST_SRC = $(filter-out tests/test_framework.c, $(wildcard tests/*.c))
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_DEP = $(TEST_SRC:.c=.d)
TEST_TARGETS = $(patsubst tests/%.c,$(BUILD_DIR)/%,$(TEST_SRC))

# ------------------
# Default target: build engine
all: $(TARGET)

$(TARGET): $(OBJ) $(MAIN_OBJ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile .c -> .o with dependency generation
%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Ensure build dir exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Include dependencies
-include $(DEP) $(TEST_DEP)

# ------------------
# Run engine
run: clean $(TARGET)
	./$(TARGET)

# ------------------
# Build and run all tests
test_all: clean $(TEST_TARGETS)
	@echo "Running all tests..."
	@for t in $(TEST_TARGETS); do \
		echo "==> Running $$t"; \
		$$t || exit 1; \
	done
	@echo "✅ All tests passed!"

# Each test links with engine objects + test framework
$(BUILD_DIR)/%: tests/%.o tests/test_framework.o $(OBJ) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# ------------------
# Clean everything
clean:
	find . -name '*.o' -delete
	find . -name '*.d' -delete
	rm -rf $(BUILD_DIR)

.PHONY: all clean run test
