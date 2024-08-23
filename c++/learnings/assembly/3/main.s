# filename: main.s
# purpose:  test output facilities in print.s and input facilities in read.s

#  spim> re "main.s"
#  spim> re "printf.s"
#  spim> re "read.s"
#  spim> run
#  spim> exit 

.text
.globl  main
.ent  main

main:
  la  $a0,format1  # Load address of format string #1 into $a0
  jal printf       # call printf

                   # test single %d 
  la  $a0,format2  # load address of format string #2 into $a0
  li  $a1, 10      # test %d format - load 10 into $a1
  jal printf       # call printf

                   # test three %d format strings
  la  $a0,format3  # load address of format string #3 into $a0
  jal printf       # call printf





   # ask for fist name
   
  # print first name string thing
  la $a0,fnameprompt # load first name string
  jal printf
  
  # read the input and display first name 
  la $a0 fname_buf #load buffer
  li $v0 8 # setup system call the cin
  syscall
 
  # print last name string thing
  la $a0,lnameprompt # load last name string
  jal printf
  
  # read the input and display last name 
  la $a0 lname_buf #load buffer
  li $v0 8 # setup system call the cin
  syscall


# load the number prompt
  la $a0 idprompt
  jal printf
   
  #ask for an id
  li $v0, 5      # load immidiate
  syscall

  move $t0, $v0 # move int to a temp
  
# priint the string
  la $a0, a_firstname
  jal printf
   
 la $a0 fname_buf
 jal printf


# priint the string
  la $a0, a_lastname
  jal printf
   
 la $a0 lname_buf
 jal printf

 #print the id
  la $a0, a_id #print id:
  jal printf
  li $v0, 1
  move $a0 , $t0
  syscall 
  

  
   
  li  $v0,10       # 10 is exit system call
  syscall    

.end  main



.data
format1: 
  .asciiz "Go.\n"        # asciiz adds trailing null byte to string
format2: 
  .asciiz "Roadrunners\n"  
format3: 
  .asciiz "in 2017\n"
a_firstname:
  .asciiz "First name:"
a_lastname: 
  .asciiz "Last name:"
a_id:
  .asciiz "ID:" 
format5: 
  .asciiz "The char is: %c\n"

buffer: .space 50
fnameprompt: .asciiz "\nEnter first name [return]:\n"
lnameprompt: .asciiz "\nEnter last name [return]:\n"
idprompt: .asciiz "\n Enter ID [return]:\n"

fname_buf: .space 32
lname_buf: .space 32
id_buf: .space 32
