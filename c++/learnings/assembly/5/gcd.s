# filename: gcd.s
# purpose: calculate the greatest common denominator 
# read two integers from the cmdline 

.data

newline: .asciiz "\n"
usage:   .asciiz "\nUsage: spim -f gcd.s <int> <int>\n"

.text
.globl main

main:
                      # grab command line args
                      # a0 is arg count and a1 points to list of args
    move $s0, $a0     #
    move $s1, $a1     #

                      # zero out these registers just to be safe
    move $s2, $zero   #
    move $s3, $zero   #
    move $s4, $zero   #
     
                      # check if less than three arguments are given
    li $t0, 3         #
                      # a0 is count of arguments, saved for you by spim
    blt $a0, $t0, show_usage
     
                      # parse the first number
    lw $a0, 4($s1)    # load word a0 = *(s1+4)
    jal atoi          # call atoi
    move $s2, $v0     # save the return value
     
                      # parse the second number
    lw $a0, 8($s1)    # load word a0 to *(s1+8)
    jal atoi          # call atoi
    move $s3, $v0     # save return value
   
                      # load a0 and a1 with two integers
    move   $a0, $s2   # 
    move   $a1, $s3   #

    move $t0, $a0     # also save in temp registers
    move $t1, $a1     #

loop:
    beq $t1, $0, done # if (t1 == 0) goto done
    div $t0, $t1      # t0 / t1
    move $t0, $t1     # t0 = t1
    mfhi $t1          # t1 = remainder of division
    j loop            # continue looping

done:
                      # move the result from t0 to v0 to print it
    move $a0, $t0     #
    li $v0, 1         #
    syscall           #

    la $a0, newline   #  
    li $v0, 4         #
    syscall           #
 
    li $v0, 10        # 10=exit
    syscall           #


show_usage:           # Show usage message, then exit.
    li $v0, 4         # 4=print string
    la $a0, usage     #
    syscall           #
    li $v0, 10        # 10=exit
    syscall           #


#------------------------------------------------------------------
#--- ATOI FUNCTION ------------------------------------------------
atoi:
    move $v0, $zero
                         # detect sign + or -
    li $t0, 1            # assume sign is positive
    lbu $t1, 0($a0)      # load byte from 1st argument into t1
    bne $t1, 45, digit   # if (t1 != '-') goto digit
    li $t0, -1           # save negative sign, will multiply by this later.
    addu $a0, $a0, 1     # move to the character after '-'

digit:
    # read character     #
    lbu $t1, 0($a0)      # load next character
                         # finish when non-digit encountered
                         # must be a character between 0 and 9
    bltu $t1, 48, finish # check for characters before 0
    bgtu $t1, 57, finish # check for characters after 9
     
                         # translate character into digit
    subu $t1, $t1, 48    # t1 = t1 - '0'
     
                         # multiply the accumulator by ten
                         # this moves the decimal point
    li $t2, 10           # load 10
    mult $v0, $t2        # multiply
    mflo $v0             # get the result
     
                         # add digit to the accumulator
    add $v0, $v0, $t1    # did it.
     
                         # next character
    addu $a0, $a0, 1     # move to next character in argument
    b digit              # continue loop

finish:
    mult $v0, $t0        # set the number's sign
    mflo $v0             # put the result of multiply into v0
    jr $ra               # return
#------------------------------------------------------------------

