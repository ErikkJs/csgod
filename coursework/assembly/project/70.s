.data
error_msg: .asciiz "Not enough arguements \n" 
endl: .asciiz "\n" 
.text

main:

    move $s0, $a0
    move $s1, $a1
    
    li $t0, 2
    blt $a0,$t0, error

    lw $a0, 4($s1)
    jal atoi
    move $s2, $v0

    lw $a0, 8($s1)
    jal atoi
    move $s3, $v0
    
    li $t0,0
    beq $t0,$s3,error
    beq $t0,$s2,error


    move $a0, $s2
    li $t4, 1
    jal Loop
    move $t2,$v0 

    move $a0, $s3
    li $t4, 1
    jal Loop
    move $t3, $v0


    blt $s2,$s3, Lessthan

    j display

Loop:
    
    mult $t4, $a0
    mflo $t4

    li $t3, 1
    sub $a0, $a0,$t3 
    
    bgt $a0, $zero, Loop
    move $v0,$t4
    jr $ra

atoi:
    move $v0, $zero

    li $t0, 1
    lbu $t1, 0($a0)      # load byte from 1st argument into t1
    bne $t1, 45, digit   # if (t1 != '-') goto digit
    li $t0, -1           # save negative sign, will multiply by this later.
    addu $a0, $a0, 1     # move to the character after '-'

digit:
    lbu $t1, 0($a0)
    
    bltu $t1, 48, finish
    bgtu $t1, 57, finish

    subu $t1,$t1,48

    li $t2, 10
    mult $v0, $t2
    mflo $v0

    add $v0, $v0, $t1

    addu $a0, $a0,1
    b digit

finish:
    mult $v0,$t0
    mflo $v0
    jr $ra

error:
    la $a0, error_msg
    li $v0, 4
    syscall

    j exit

Lessthan:
    
    move $s4,$s2
    move $s2,$s3
    move $s3, $s4

    move $s4,$t2
    move $t2,$t3
    move $t3,$s4

display:
    
    move $a0, $s2
    li $v0 , 1
    syscall

    li $a0, 32
    li $v0, 11
    syscall 
    
    move $a0, $t2
    li $v0, 1
    syscall
    
    la $a0, endl
    li $v0, 4
    syscall

    move $a0, $s3
    li $v0, 1
    syscall

    li $a0, 32
    li $v0, 11
    syscall

    move $a0, $t3
    li $v0, 1
    syscall
    
    la $a0, endl
    li $v0, 4
    syscall
    
    j exit
exit:
    li $v0 10
    syscall
