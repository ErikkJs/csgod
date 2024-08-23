; lab9.asm
; CMPS 2240 lab9 
; 
; prompt and read two integers from stdin, compare and display larger integer
; uses printf and scanf from glibc 
;
; You job is to modify this program to sum the two integers and display the 
; result. You can leave existing code alone.
;
;
; To assemble and link:
;
;     $ nasm -f elf64 lab8.asm
;     $ gcc -o lab8 lab8.o   # use gcc so glibc will be linked in
;
; OR you could link manually with this:
; $ ld -dynamic-linker /lib64/ld-linux-x86-64.so.2 /usr/lib64/crt1.o \
;        /usr/lib64/crti.o lab8.o  /usr/lib64/crtn.o -lc -o lab8

section .rodata
   prompt1    db "Enter an integer: ",0   ; 0 is null character
   prompt2    db "Enter another integer: ",0
   prompt3    db "Both integers are equal. ", 0
   prompt4    db "The sum of %ld and %ld is %ld.", 10, 0    ; 10 is LF
   format_str db "The greater of %ld and %ld is %ld.",10,0  ; 10 is LF 
   num_format db "%ld",0

section .text
   global main              ; main and _start are both valid entry points
   extern printf, scanf     ; these will be linked in from glibc 

   main:
      ; prologue
      push    rbp          ; save base pointer to the stack
      mov     rbp, rsp     ; base pointer = stack pointer 
      sub     rsp, 16      ; make room for two long integers on the stack
      push    rbx          ; push callee saved registers onto the stack 
      push    r12          ; push automatically decrements stack pointer
      push    r13
      push    r14
      push    r15
      pushfq               ; push register flags onto the stack

      ; prompt for first integer 
      mov    rdi, dword prompt1    ; double word is 4 bytes; a word is 2 bytes
                                   ; rdi = 32-bit address of prompt1
      xor    rax, rax              ; rax is return value register - zero it out
      call   printf                ; call the C function from glibc 

      ; read first integer 
      lea    rsi, [rbp-8]          ; load effective address - this instruction
                                   ; computes the address 8 bytes above the
                                   ; base pointer - the integer read by scanf 

      mov    rdi, dword num_format ; load rdi with address to format string
      xor    rax, rax              ; zero out return value register
      call   scanf                 ; call C function
                                   ; scanf reads the input as an integer

      ; prompt for second integer 
      mov    rdi, dword prompt2
      xor    rax, rax
      call   printf

      ; read second integer 
      lea    rsi, [rbp-16]
      mov    rdi, dword num_format
      xor    rax, rax
      call   scanf

      ; determine if num2 (second integer) is greater than num1 (first integer)
      xor     rbx, rbx      ; RBX = 0x0
      mov     rax, [rbp-16] ; RAX = num2 ; load rax with value at rdb-16
      cmp     rax, [rbp-8]  ; compute (num1) - (num2) and set condition codes
                            ; in machine status word register based on result
      jl      lessthan      ; jump if num1 <= num2 

      je      equal
      ; num1 > num2 
      mov     rdi, dword format_str
      mov     rsi, [rbp-8]     ; num1
      mov     rdx, [rbp-16]    ; num2
      mov     rcx, [rbp-16]    ; greater of the two 
      xor     rax, rax

      call  printf             ; print out the string
      jmp add                  ; jump to add

add:
      xor   rbx, rbx    ; effectively zero out rbx
      xor   rcx, rcx    ; effectively zero out rcx
      mov rcx, [rbp-16] ; load rcx with value 16 bytes from base ptr
      mov rbx, [rbp-8]  ; load rbx with value 8 bytes from base ptr
      add rcx, rbx      ; add num1 + num2 - store in rcx

      mov rdi, prompt4  ; move the prompt into rdi
      mov rsi, [rbp-8]  ;num1
      mov rdx, [rbp-16] ;num2
      xor rax, rax  ;clear out rax

      call printf       ; print out the string
      jmp exit          ; jump to exit
lessthan:
      ; num1 < num2
      mov     rdi, dword format_str
      mov     rsi, [rbp-8]   ; num1
      mov     rdx, [rbp-16]  ; num2
      mov     rcx, [rbp-8]   ; greater of the two
      xor     rax, rax       ;clear out rax
      call    printf         ;print the string
      jmp add                ;jump to add
equal:
      ; num1 == num2
      mov rdi, dword prompt3    ; move prompt3 into rdi
      xor rax, rax
      call printf     ;print the prompt
      jmp add       ;jump to add
exit:
      ; epilogue
      popfq
      pop     r15
      pop     r14
      pop     r13
      pop     r12
      pop     rbx
      add     rsp, 16        ; set back the stack level
      leave
      ret
        