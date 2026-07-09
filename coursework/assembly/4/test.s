# filename: gcd.s
# purpose: return greatest common denominator of two ints using a loop

#      $ spim -f gcd.s
#  or
#      $ spim
#      spim> re "gcd.s"
#      spim> run


main:
  # load a0 and a1 with two integers
  li   $a0, 30 
  li   $a1, 45 

  move $t0, $a0
  move $t1, $a1

loop:
  beq $t1, $0, display # loop control
  div $t0, $t1        # integer division; hi holds quotient; lo holds remainder 
  move $t0, $t1
  mfhi $t1            # move hi (quotient) to $t1
  j loop

display:
  # move the result from t0 to a0 and then print it
  move $a0, $t0  
  li $v0, 1
  syscall
  li $v0, 10
  syscall
