# lab1.s
# a program to demonstrate the structure of a MIPS source program for lab1
# take this file and modify it
#
# Usage: $ spim -f lab1.s

     .data                   # what follows is in data segment of executable
stuff: .asciiz "hi again"    # the z adds a trailing \0 null char
dot:   .byte 46              # 46 is the ascii-decimal equivalent 
lf:    .byte 10              # 10 is an ascii linefeed 

     .text                   # code follows 
main:
     la $a0, stuff           # stuff is an address of a string
     li $v0, 4               # 4 is syscall to print a string
     syscall                 # execute the call 

     lb $a0, dot             # load the value of the ascii dot into a0 
     li $v0, 11              # 11 is syscall to print a char 
     syscall                 # execute the call 

     lb $a0, lf              # load the value of the linefeed into a0 
     syscall                 # execute the call 

     li $a0, 20              # load 20 into $a0 
     li $v0, 1               # 1 is syscall to print int
     syscall                 # execute the call 

     li $a0, 32              # another way to print an ascii character
     li $v0, 11              # 11 is syscall to print a char 
     syscall                 # execute the call 

     li $t0, 55              # load 55 into register t0
     move $a0, $t0           # copy value in reg t0 to reg a0
     li $v0, 1               # 1 is syscall to print int
     syscall                 # execute the print_int syscall 
 

     lb $a0, lf              # load the value of the linefeed into a0 
     li $v0, 11              # 11 is syscall to print a char 
     syscall                 # execute the call 

     li $v0, 10              # 10 is system call to exit
     syscall                 # execute the call 

