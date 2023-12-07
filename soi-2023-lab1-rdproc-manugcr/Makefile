CC = gcc
CFLAGS = -Wall -pedantic -g -Werror -Wextra -Wconversion -std=gnu11

SRC_DIR = src
INC_DIR = include
BIN_DIR = bin

MAIN_SRC = $(SRC_DIR)/rdproc.c
UTILS_SRC = $(SRC_DIR)/utils.c
SYSDATA_SRC = $(SRC_DIR)/sys_data.c
JSON_SRC = $(SRC_DIR)/cJSON.c
DSKLOAD_SRC = $(SRC_DIR)/disk_load.c
PIDINF_SRC = $(SRC_DIR)/pid_info.c

OBJS = $(BIN_DIR)/rdproc.o $(BIN_DIR)/utils.o $(BIN_DIR)/sys_data.o $(BIN_DIR)/cJSON.o $(BIN_DIR)/disk_load.o $(BIN_DIR)/pid_info.o

TARGET = rdproc

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/rdproc.o: $(MAIN_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c -o $@ $^ -I$(INC_DIR)

$(BIN_DIR)/utils.o: $(UTILS_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c -o $@ $^ -I$(INC_DIR)

$(BIN_DIR)/sys_data.o: $(SYSDATA_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c -o $@ $^ -I$(INC_DIR)

$(BIN_DIR)/cJSON.o: $(JSON_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c -o $@ $^ -I$(INC_DIR)

$(BIN_DIR)/disk_load.o: $(DSKLOAD_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c -o $@ $^ -I$(INC_DIR)

$(BIN_DIR)/pid_info.o: $(PIDINF_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c -o $@ $^ -I$(INC_DIR)

clean:
	rm -rf $(BIN_DIR)

clear:
	rm -rf $(BIN_DIR)

.PHONY: all clean clear