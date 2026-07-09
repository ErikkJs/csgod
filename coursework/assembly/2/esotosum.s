# erik soto
# 9-1-17
# a sample MIPS program to demonstrate MIPS basics 
# Usage: $ spim -f hello.s

        .data                # data segment begins here
stuff:  .asciiz "free tay-k!\n"
str1:   .asciiz "the sum of 4 and 5 is "
        .text                # code segment begins here 
main:
        li $t1, 4
        li $t2, 5
        add $t3 , $t1 , $t2  # add the numbers

        la $a0, str1         #load the add sum string
        li $v0, 4            #print out 
        syscall

        li $v0 ,1             # load syscall
        move $a0, $t3         #load my variables into a0 
        syscall
      
        li $a0,10
        li $v0,11
        syscall 

        li $v0, 10           # 10 is system call to exit
        syscall              # execute the call 


