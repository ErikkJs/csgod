#50.s
#Erik Soto
#CMPS 2240 final project

.data
error_msg: .asciiz "Not enough arguements."

.text
main:
    move $s0, $a0
    move $s1, $a1
    
    li $t0, 3
    blt $s0, $t0, error
    
    lw $a0, 4($s1)
    jal atoi
    move $s2, $v0

    lw $a0, 8($s1)
    jal atoi
    move $s3, $v0

    blt $s3,$s2, LessThan
    j display

LessThan:
    move $s4, $s2
    move $s2, $s3
    move $s3, $s4
    
    j display

display:
    move $a0, $s2
    li $v0, 1
    syscall

    li $a0, 32
    li $v0, 11
    syscall 

    move $a0, $s3
    li $v0, 1
    syscall
    j exit

atoi:
    move $v0, $zero
                         # detect sign + or -
    li $t0, 1            # assume sign is positive
    lbu $t1, 0($a0)      # load byte from 1st argument into t1
    bne $t1, 45, digit   # if (t1 != '-') goto digit
    li $t0, -1           # save negative sign, will multiply by this later.
    addu $a0, $a0, 1     # move to the character after '-'

digit:
                         # read character
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
#----

error:
    la $a0, error_msg
    la $v0, 4
    syscall
    j exit

exit:
    li $v0 , 10
    syscall
