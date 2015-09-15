PROJ_NAME = cweb
SRC_DIR = src
BIN_DIR = bin
CC = gcc

$(PROJ_NAME):
	$(CC) -o $(BIN_DIR)/$(PROJ_NAME) $(SRC_DIR)/*.c -Wall -pedantic-errors -std=gnu99 -g

.PHONY: clean

clean:
	rm $(BIN_DIR)/$(PROJ_NAME) -r
