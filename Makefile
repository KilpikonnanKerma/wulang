CC=gcc
ASM=nasm

CFLAGS=-Wall -Wextra -std=c11

COMPILER_SRC=src/*.c
COMPILER = wuc

all: compiler

compiler: COMPILER_SRC
	$(CC) $(CFLAGS) -o $(COMPILER) $(COMPILER_SRC)