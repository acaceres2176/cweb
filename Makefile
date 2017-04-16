PROJ_NAME = cweb
SRC_DIR = src
BIN_DIR = bin
CC = gcc
CFLAGS = -Wall -Wextra -Wshadow -pedantic-errors -std=gnu99 -g

$(BIN_DIR)/$(PROJ_NAME):
	$(CC) $(SRC_DIR)/*.c $(CFLAGS) -o $@

.PHONY: clean

clean:
	rm $(BIN_DIR)/$(PROJ_NAME) -rf
