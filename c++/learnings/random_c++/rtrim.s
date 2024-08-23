# filename: rtrim.s
# purpose: read a userinput string and remove all
# trailing white spaces.
# Author : Alec Sherlock

.data
        newline:    .asciiz "\n"
        prompt:     .asciiz "Enter a word followed by some whitespace: "
        stringSpace: .space 30
        period:     .asciiz "."
.text
main:

        li      $v0, 4          #load the syscall that prompts the user
        la      $a0, prompt
        syscall
 
        la      $a0, stringSpace#load address of the string
        li      $v0, 8          #load read string
        syscall                 #read user input

        move 	$s0,  $a0	    # $s0 = $a0
        
pointBeg:
        move    $t0, $s0	#set beginning point of pointer

findEndOfString:
        lbu     $t1, 0($t0)     #load first char into $t1
        beqz    $t1, endOfString
        addiu   $t0, $t0, 1
        j	findEndOfString # jump to findEndOfString
endOfString:
        subu    $t0, $t0, 1

isThisASpace:
        lbu     $t2, 0($t0)     #load char into $t2
        bgt	$t2, 32, end	# if $t2 is a space then end
        subu    $t0, $t0, 1     #increment char
        j       isThisASpace    #jump to theloop, yo

end:
        addu    $t0, $t0, 1
        sb      $0, 0($t0)
        
        li      $v0, 4
        move    $a0, $s0
        syscall

        la      $a0, period
        syscall

        la      $a0, newline
        syscall
        li      $v0, 10
        syscall
        