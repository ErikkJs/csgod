	.file	"if_vs_switch.cpp"
	.text
	.section	.text$_ZNKSt5ctypeIcE8do_widenEc,"x"
	.linkonce discard
	.align 2
	.p2align 4
	.globl	_ZNKSt5ctypeIcE8do_widenEc
	.def	_ZNKSt5ctypeIcE8do_widenEc;	.scl	2;	.type	32;	.endef
	.seh_proc	_ZNKSt5ctypeIcE8do_widenEc
_ZNKSt5ctypeIcE8do_widenEc:
.LFB1995:
	.seh_endprologue
	movl	%edx, %eax
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC0:
	.ascii "Switch Result: \0"
	.text
	.p2align 4
	.globl	_Z10testSwitchi
	.def	_Z10testSwitchi;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z10testSwitchi
_Z10testSwitchi:
.LFB2242:
	pushq	%rsi
	.seh_pushreg	%rsi
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$56, %rsp
	.seh_stackalloc	56
	.seh_endprologue
	movl	%ecx, 80(%rsp)
	movl	$0, 44(%rsp)
	movl	80(%rsp), %eax
	cmpl	$2, %eax
	je	.L4
	cmpl	$3, %eax
	je	.L5
	cmpl	$1, %eax
	jne	.L6
	movl	$10, 44(%rsp)
.L7:
	movq	.refptr._ZSt4cout(%rip), %rbx
	movl	$15, %r8d
	leaq	.LC0(%rip), %rdx
	movq	%rbx, %rcx
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x
	movl	44(%rsp), %edx
	movq	%rbx, %rcx
	call	_ZNSolsEi
	movq	%rax, %rbx
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%rbx,%rax), %rsi
	testq	%rsi, %rsi
	je	.L12
	cmpb	$0, 56(%rsi)
	je	.L9
	movsbl	67(%rsi), %edx
.L10:
	movq	%rbx, %rcx
	call	_ZNSo3putEc
	movq	%rax, %rcx
	addq	$56, %rsp
	popq	%rbx
	popq	%rsi
	jmp	_ZNSo5flushEv
	.p2align 4,,10
	.p2align 3
.L5:
	movl	$30, 44(%rsp)
	jmp	.L7
	.p2align 4,,10
	.p2align 3
.L9:
	movq	%rsi, %rcx
	call	_ZNKSt5ctypeIcE13_M_widen_initEv
	movq	(%rsi), %rax
	movl	$10, %edx
	leaq	_ZNKSt5ctypeIcE8do_widenEc(%rip), %rcx
	movq	48(%rax), %rax
	cmpq	%rcx, %rax
	je	.L10
	movq	%rsi, %rcx
	call	*%rax
	movsbl	%al, %edx
	jmp	.L10
	.p2align 4,,10
	.p2align 3
.L4:
	movl	$20, 44(%rsp)
	jmp	.L7
	.p2align 4,,10
	.p2align 3
.L6:
	movl	$0, 44(%rsp)
	jmp	.L7
.L12:
	call	_ZSt16__throw_bad_castv
	nop
	.seh_endproc
	.section .rdata,"dr"
.LC1:
	.ascii "If Result: \0"
	.text
	.p2align 4
	.globl	_Z6testIfi
	.def	_Z6testIfi;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z6testIfi
_Z6testIfi:
.LFB2243:
	pushq	%rsi
	.seh_pushreg	%rsi
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$56, %rsp
	.seh_stackalloc	56
	.seh_endprologue
	movl	%ecx, 80(%rsp)
	movl	$0, 44(%rsp)
	movl	80(%rsp), %eax
	cmpl	$1, %eax
	je	.L22
	movl	80(%rsp), %eax
	cmpl	$2, %eax
	je	.L23
	movl	80(%rsp), %eax
	cmpl	$3, %eax
	je	.L24
	movl	$0, 44(%rsp)
.L15:
	movq	.refptr._ZSt4cout(%rip), %rbx
	movl	$11, %r8d
	leaq	.LC1(%rip), %rdx
	movq	%rbx, %rcx
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x
	movl	44(%rsp), %edx
	movq	%rbx, %rcx
	call	_ZNSolsEi
	movq	%rax, %rbx
	movq	(%rax), %rax
	movq	-24(%rax), %rax
	movq	240(%rbx,%rax), %rsi
	testq	%rsi, %rsi
	je	.L25
	cmpb	$0, 56(%rsi)
	je	.L19
	movsbl	67(%rsi), %edx
.L20:
	movq	%rbx, %rcx
	call	_ZNSo3putEc
	movq	%rax, %rcx
	addq	$56, %rsp
	popq	%rbx
	popq	%rsi
	jmp	_ZNSo5flushEv
	.p2align 4,,10
	.p2align 3
.L23:
	movl	$20, 44(%rsp)
	jmp	.L15
	.p2align 4,,10
	.p2align 3
.L19:
	movq	%rsi, %rcx
	call	_ZNKSt5ctypeIcE13_M_widen_initEv
	movq	(%rsi), %rax
	movl	$10, %edx
	leaq	_ZNKSt5ctypeIcE8do_widenEc(%rip), %rcx
	movq	48(%rax), %rax
	cmpq	%rcx, %rax
	je	.L20
	movq	%rsi, %rcx
	call	*%rax
	movsbl	%al, %edx
	jmp	.L20
	.p2align 4,,10
	.p2align 3
.L22:
	movl	$10, 44(%rsp)
	jmp	.L15
	.p2align 4,,10
	.p2align 3
.L24:
	movl	$30, 44(%rsp)
	jmp	.L15
.L25:
	call	_ZSt16__throw_bad_castv
	nop
	.seh_endproc
	.def	__main;	.scl	2;	.type	32;	.endef
	.section	.text.startup,"x"
	.p2align 4
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
.LFB2244:
	subq	$56, %rsp
	.seh_stackalloc	56
	.seh_endprologue
	call	__main
	movl	$2, 44(%rsp)
	movl	44(%rsp), %ecx
	call	_Z10testSwitchi
	movl	44(%rsp), %ecx
	call	_Z6testIfi
	xorl	%eax, %eax
	addq	$56, %rsp
	ret
	.seh_endproc
	.ident	"GCC: (Rev3, Built by MSYS2 project) 13.2.0"
	.def	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x;	.scl	2;	.type	32;	.endef
	.def	_ZNSolsEi;	.scl	2;	.type	32;	.endef
	.def	_ZNSo3putEc;	.scl	2;	.type	32;	.endef
	.def	_ZNSo5flushEv;	.scl	2;	.type	32;	.endef
	.def	_ZNKSt5ctypeIcE13_M_widen_initEv;	.scl	2;	.type	32;	.endef
	.def	_ZSt16__throw_bad_castv;	.scl	2;	.type	32;	.endef
	.section	.rdata$.refptr._ZSt4cout, "dr"
	.globl	.refptr._ZSt4cout
	.linkonce	discard
.refptr._ZSt4cout:
	.quad	_ZSt4cout
