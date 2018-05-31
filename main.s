

.global foo
foo:
	push %rbp
	mov %rsp, %rbp
	sub $8, %rsp
	mov 16(%rbp), %rax
	mov %rax, -8(%rbp)
	mov $4, %rax
	mov %rax, %rbx
	movl -8(%rbp), %eax
	add %rbx, %rax
	leave
	ret
.global main
main:
	push %rbp
	mov %rsp, %rbp
	sub $16, %rsp
	mov $4, %rax
	movl %eax, -8(%rbp)
	mov $8, %rax
	movl %eax, -16(%rbp)
	movabsq $foo, %rax
	mov %rax, %rbx
	movl -16(%rbp), %eax
	push %rax
	call *%rbx
	leave
	ret
