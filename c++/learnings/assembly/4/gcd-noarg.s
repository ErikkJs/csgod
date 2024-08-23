# filename: gcd-noarg.s
# purpose: return greatest common denominator of two ints using a loop
#
#      $ spim -f gcd.s
#  or
#      $ spim
#      spim> re "gcd.s"
#      spim> run

.data
newline: .asciiz   "\n"

.text
.globl main

main:
                       # load a0 and a1 with two integers
  li   $a0, 30         #
  li   $a1, 45         #
  move $t0, $a0        # save in temp registers also
  move $t1, $a1        #

loop:
  beq $t1, $0, show    # loop control
  div $t0, $t1         # integer division
                       # hi holds quotient, lo holds remainder 

  move $t0, $t1        #
  mfhi $t1             # move hi (quotient) to $1
  j loop               #

show:                  # move the result from t0 to v0 to print it
  move $a0, $t0        #
  li $v0, 1            # print int
  syscall              #

  li $v0, 4            #
  la $a0, newline      # print string
  syscall              #

  li $v0, 10           # exit
  syscall              #

