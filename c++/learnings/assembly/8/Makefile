# CMPS 224 Week 8
# Makefile for nasm assembly code in /code/lab8

all:	bare driver goodbye inout lab8

# test assembling and linking a very small x86 source
bare:	bare.asm
	nasm -f elf64 bare.asm
	ld -o bare  bare.o

# test linking C source with x86 source
driver:	driver.c io.asm
	gcc -c driver.c -o driver.o
	nasm -f elf64 io.asm
	gcc -o driver io.o driver.o

# test linking two assembly sources together
goodbye:	goodbye.asm io.asm
	nasm -f elf64 -o io.o io.asm  
	nasm -f elf64 -o goodbye.o goodbye.asm
	ld -o goodbye goodbye.o io.o -lc          # link in libc

# test
inout:	inout.asm 
	nasm -f elf64 inout.asm
	ld -o inout inout.o

lab8:	lab8.asm 
	nasm -f elf64 -g -F dwarf lab8.asm    # -g -F dwarf adds debugging stuff
	gcc -o lab8  lab8.o -lc

clean:  
	rm -f *.o

veryclean:  # start from scratch 
	rm -f *.o
	rm -f goodbye
	rm -f bare 
	rm -f inout 
	rm -f lab8
	rm -f driver 
