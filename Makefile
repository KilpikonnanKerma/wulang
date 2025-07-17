CC=gcc
ASM=nasm

CFLAGS=-Wall -Wextra -std=c11

#folders
SRC_DIR=src
BIN_DIR=bin

COMPILER_SRC=$(SRC_DIR)/*.c
COMPILER = $(BIN_DIR)/wuc

all: compiler

dirs: clean
	mkdir -p $(BIN_DIR)

compiler: dirs
	$(CC) $(CFLAGS) -o $(COMPILER) $(COMPILER_SRC)

# hardcoded test
test: all
	$(COMPILER) test/test.wu -o test/test.asm
	$(ASM) -f elf64 test/test.asm -o $(BIN_DIR)/test.obj
	$(CC) -o $(BIN_DIR)/test.exe $(BIN_DIR)/test.obj -no-pie

clean:
	rm -rf $(BIN_DIR)