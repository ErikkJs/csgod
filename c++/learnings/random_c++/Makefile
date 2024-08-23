# CMPS 2240 
# Makefile for nasm assembly code in lab10

all: lab10

lab10: Soto_Erik_Lab10.asm 
	nasm -f elf64 -g -F dwarf Soto_Erik_Lab10.asm    # -g -F dwarf adds debugging stuff
	gcc -o lab10  lab10.o -lc

clean:
	rm -f *.o
	rm -f lab10


