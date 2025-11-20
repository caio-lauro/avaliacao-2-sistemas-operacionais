INCLUDE_DIR = include

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pthread -g -I$(INCLUDE_DIR)
LDFLAGS = -pthread -g

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

TARGET = $(BIN_DIR)/main

all: bin_folder obj_folder $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

bin_folder:
	mkdir -p $(BIN_DIR)

obj_folder:
	mkdir -p $(OBJ_DIR)

.SILENT: bin_folder obj_folder
.PHONY: all clean

clean:
	rm -rf $(TARGET) $(OBJ_DIR)
	rmdir $(BIN_DIR) 2>/dev/null || true