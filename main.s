

.global foo
foo:
	push %rbp
	mov %rsp, %rbp
	sub $8, %rsp
	mov 16(%rbp), %rax
	mov %rax, -8(%rbp)
	mov $4, %rax
	mov %rax, %rbx
	mov %rbp, %rdx
	sub $8, %rdx
	movb %al, (%rdx)
	movzbq -8(%rbp), %rax
	leave
	ret
.global main
main:
	push %rbp
	mov %rsp, %rbp
	sub $0, %rsp
	movabsq $foo, %rax
	mov %rax, %rbx
	mov $5, %rax
	push %rax
	call *%rbx
	leave
	ret
