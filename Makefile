# ---- Project ---------------------------------------------------------------
TARGET    := shellc
SRC_DIR   := src
INC_DIR   := include
BUILD_DIR := build

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# ---- Toolchain -------------------------------------------------------------
CC       ?= cc
CPPFLAGS := -I$(INC_DIR) -D_POSIX_C_SOURCE=200809L -MMD -MP
CFLAGS   := -std=c17 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -g
LDFLAGS  :=
LDLIBS   :=

# `make debug` builds a separate copy with AddressSanitizer + UndefinedBehaviorSanitizer
SANITIZE := -fsanitize=address,undefined -fno-omit-frame-pointer

# ---- Rules -----------------------------------------------------------------
.PHONY: all run debug clean rebuild

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

run: all
	./$(BUILD_DIR)/$(TARGET)

debug:
	$(MAKE) BUILD_DIR=$(BUILD_DIR)/debug CFLAGS="$(CFLAGS) -O0 $(SANITIZE)"

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all

-include $(DEPS)
