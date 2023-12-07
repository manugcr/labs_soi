CFLAGS		= -Wall -pedantic -Werror -Wextra -Wconversion -std=gnu11
CC			= gcc

SRC_DIR		= src
INC_DIR		= inc
BIN_DIR		= bin

MAIN		= main
MEM			= memory

MAIN_SRC	= $(SRC_DIR)/$(MAIN).c
MEM_SRC		= $(SRC_DIR)/$(MEM).c

OBJS		= $(BIN_DIR)/$(MAIN).o $(BIN_DIR)/$(MEM).o


all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/$(MAIN).o: $(MAIN_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c -o $@ $^ -I$(INC_DIR)

$(BIN_DIR)/$(MEM).o: $(MEM_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c -o $@ $^ -I$(INC_DIR)


clean:
	rm -rf $(BIN_DIR)

clear:
	rm -rf $(BIN_DIR)

.PHONY: all clean clear