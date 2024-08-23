# filename: calc.s
# description: cs 224 lab 06 

# prompt the user to enter two integers, sum integers and display the result

.text
.globl read 
.globl main 
.ent  main

main:


  # grab command line stuff - a0 is arg count and a1 points to list of args
  move $s0, $a0         #first num
  move $s1, $a1         #sign
  move $s2, $a2         #last num

  # zero out these registers just to be safe
  move $s3, $zero
  move $s4, $zero
  move $s5, $zero
     
  # check if less than three arguments are given
  li $t0, 4 
  blt $a0, $t0, exit_on_error
     
  # parse the first number
  lw $a0, 4($s1)
  jal atoi
  move $s3, $v0
     
  # parse the sign
  lw $a0, 8($s1)
  jal atoi
  move $s4, $v0

  # pase the last num 
  lw $a0, 12($s1)
  jal atoi
  move $s5, $v0
  
# load a0 and a1 with two integers
  move   $a0, $s2
  move   $a1, $s3 
  move   $a2, $s4

  move $t0, $a0
  move $t1, $a1

  li 



  jal read
  li  $v0,10       # 10 is exit system call
  syscall    

.end  main


read:

  # prompt for first integer
  li $v0 4         # load immediate with 4 to setup syscall 4 (print_str)
  la $a0, iprompt  # load address of prompt into $a0 for print_str
  syscall          # display the prompt 

  # read the first integer 
  li $v0 5         # setup syscall 5 (read_int)
  syscall          # integer returned in $v0
  move $t0, $v0    # move first integer to $t0

  # prompt for second integer
  li $v0, 4        # load immediate with 4 to setup syscall 4 (print_str)
  la $a0, iprompt  # load address of prompt into $a0 for print_str
  syscall          # display the prompt 

  # read the second integer 
  li $v0 5         # setup syscall 5 (read_int)
  syscall          # integer returned in $v0
  move $t1, $v0    # move first integer to $t1

  # sum the two integers
  add $t2, $t0, $t1 

  move $a0,$t2     # move the result into register $a0
  li $v0, 1        # setup syscall 1 (print_int)
  syscall          # make the call to display the integer 
   
  li $v0, 4        # print newline
  la $a0, newline  
  syscall          
 
  jr $ra

.data

iprompt: .asciiz "Enter an integer [return]:\n"
newline: .asciiz "\n"



# --------- ATOI FUNCTION 
atoi:
    move $v0, $zero
     
    # detect sign
    li $t0, 1
    lbu $t1, 0($a0)
    bne $t1, 45, digit
    li $t0, -1
    addu $a0, $a0, 1

digit:
    # read character
    lbu $t1, 0($a0)
     
    # finish when non-digit encountered
    bltu $t1, 48, finish
    bgtu $t1, 57, finish
     
    # translate character into digit
    subu $t1, $t1, 48
     
    # multiply the accumulator by ten
    li $t2, 10
    mult $v0, $t2
    mflo $v0
     
    # add digit to the accumulator
    add $v0, $v0, $t1
     
    # next character
    addu $a0, $a0, 1
    b digit

finish:
    mult $v0, $t0
    mflo $v0
    jr $ra
#----------------------------------------

