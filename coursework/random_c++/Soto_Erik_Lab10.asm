; Erik Soto lab10.asm
section .rodata
	prompt1    db "Please enter a number: ",0   ; 0 is null character
	prompt2    db "Enter another number: ",0
	format_str db "The sum is: %ld.",10,0  ; 10 is LF
	num_format db "%ld",0
section .text
	global main              ; main and _start are both valid entry points
	extern printf, scanf     ; these will be linked in from glibc
main:
	; prologue
	push    rbp          ; save base pointer to the stack
	mov     rbp, rsp     ; base pointer = stack pointer
	sub     rsp, 80      ; make room for integers on the stack
	mov     r12, 10      ; push automatically decrements stack pointer
	xor		rbx, rbx     ; sum = 0
	move 	rdi, dword prompt1
start:
	cmp		r12, 10             ;acts as a counter
	jz		continue
	mov		rdi, dword prompt2
continue:
	call	printf
	lea		rsi, [rbp-8]
	mov		rdi, dword num_format
	call 	scanf
	add		rbx, [rpb-8]
	sub		r12, 1              ;decrement counter.
	jnz		start
	mov		rsi, rbx
	mov		rsi, dword format_str
	call	printf
	add		rsp, 80
	leave
