
.section .data
.temp1:
	.asciz "Hello, world!  "
.section .text
.global main
main:
	push %rbp
	mov %rsp, %rbp
	sub $16, %rsp
	movabs $.temp1, %rax
	mov %rax, -8(%rbp)
	mov $0, %rax
	movl %eax, -16(%rbp)
	mov $10, %rax
	mov %rax, %rbx
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	mov %rax, %rcx
	mov $13, %rax
	add %rax, %rcx
	movb %bl, (%rcx)
	mov $0, %rax
	mov %rax, %rbx
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	mov %rax, %rcx
	mov $14, %rax
	add %rax, %rcx
	movb %bl, (%rcx)
	mov $10, %rax
	mov %rax, %rbx
	movl -16(%rbp), %eax
	cmp %rbx, %rax
	setl %al
	movzbq %al, %rax
	cmp $0, %rax
	je .temp4
.temp4:
	leave
	ret
