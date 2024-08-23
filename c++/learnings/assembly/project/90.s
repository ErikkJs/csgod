.data
error_msg: .asciiz "Not enough arguements \n" 
endl: .asciiz "\n" 
.text

main:

    addi $sp,$sp,-32
    sw $ra,20($sp)
    sw $fp, 16($sp)
    addi $fp,$sp,28

    move $s0, $a0
    move $s1, $a1
    
    li $t0, 2
    blt $a0,$t0, error

    lw $a0, 4($s1)         #First arguement to int
    jal atoi
    move $s2, $v0

    lw $a0, 8($s1)          #second arguement to int
    jal atoi
    move $s3, $v0

    blt $s2,$s3,Lessthan
    li $t7,0
Main: 
    jal Cnk
    jal display

    li $s3,0
    jal displayRow
    j exit

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
    move $t0,$s2
    move $s2,$s3
    move $s3,$t0
    
    j Main
Cnk:
    sw $ra,12($sp) 
    
    move $t0, $s2           #$t2 holds fac of first arg
    li $t4, 1
    jal Loop

    move $t0, $s3           #$t3 holds fac of second arg
    li $t4, 1
    jal Loop
    
    sub $s4,$s2,$s3         # $s4 holds first - second
    
    move $t0, $s4
    li $t4,1
    jal Loop                # $t4 holds fac
    
    sub $sp,$sp,12
    lw $ra,12($sp)
    
    lw $t1,4($sp)
    lw $t2,8($sp)
    mult $t1,$t2
    mflo $s5
    
    lw $t1,($sp)
    div $t1, $s5
    mflo $s5

    jr $ra
displayRow:

    jal Cnk
    li $t7,0

    bgt $s5,$t7,L2

    li $a0,1
    li $v0,1
    syscall
    j L3
L2:
    move $a0, $s5
    li $v0,1
    syscall

    j L3

L3:
    li $a0,32
    li $v0,11
    syscall

    addi $s3,$s3,1
    ble $s3,$s2, displayRow

    j exit
Loop: #fac
    mult $t4, $t0
    mflo $t4

    li $t5, 1
    sub $t0, $t0,$t5 
    
    bgt $t0, $zero, Loop
    sw $t4,($sp)
    add $sp,$sp,4

    jr $ra
display:
    
    move $a0, $s2
    li $v0 , 1
    syscall

    li $a0, 32
    li $v0, 11
    syscall 
    
    lw $a0,($sp)
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

    lw $a0, 4($sp)
    li $v0, 1
    syscall
    
    la $a0, endl
    li $v0, 4
    syscall
    lw $t1,4($sp)
    lw $t2,8($sp)
    mult $t1,$t2
    mflo $t0

    lw $t1,($sp)
    div $t1,$t0
    mflo $t0

    move $a0, $t0
    li $v0,1
    syscall 
    
    la $a0, endl
    li $v0,4
    syscall
   jr $ra
#calc:

exit:
    li $v0 10
    syscall
