CFLAGS = -Wall -pedantic -Werror
CC = gcc

SRC_DIR = src
INC_DIR = inc
BIN_DIR = bin

MAIN	= myshell
TRM		= terminal
CMD		= commands
EXT		= external

MAIN_SRC = $(SRC_DIR)/$(MAIN).c
TRM_SRC = $(SRC_DIR)/$(TRM).c
CMD_SRC = $(SRC_DIR)/$(CMD).c
EXT_SRC = $(SRC_DIR)/$(EXT).c

OBJS = $(BIN_DIR)/$(MAIN).o $(BIN_DIR)/$(TRM).o $(BIN_DIR)/$(CMD).o $(BIN_DIR)/$(EXT).o


all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/$(MAIN).o: $(MAIN_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c -o $@ $^ -I$(INC_DIR)

$(BIN_DIR)/$(TRM).o: $(TRM_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c -o $@ $^ -I$(INC_DIR)

$(BIN_DIR)/$(CMD).o: $(CMD_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c -o $@ $^ -I$(INC_DIR)

$(BIN_DIR)/$(EXT).o: $(EXT_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c -o $@ $^ -I$(INC_DIR)

clean:
	rm -rf $(BIN_DIR)

clear:
	rm -rf $(BIN_DIR)

.PHONY: all clean clear