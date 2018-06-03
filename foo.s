

.section .data
.temp1:
	.asciz "Hello, world!  "
.temp2:
	.asciz "Good night! "
.section .text
.global foo
foo:
	push %rbp
	mov %rsp, %rbp
	sub $0, %rsp
	mov $9, %rax
	leave
	ret
.global main
main:
	push %rbp
	mov %rsp, %rbp
	sub $32, %rsp
	movabs $.temp1, %rax
	mov %rax, -8(%rbp)
	movabs $.temp2, %rax
	mov %rax, -16(%rbp)
	mov $10, %rax
	movl %eax, -24(%rbp)
	mov $10, %rax
	movl %eax, -32(%rbp)
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
	mov %rbp, %rax
	sub $16, %rax
	mov (%rax), %rax
	mov %rax, %rcx
	mov $10, %rax
	add %rax, %rcx
	movb %bl, (%rcx)
	mov $0, %rax
	mov %rax, %rbx
	mov %rbp, %rax
	sub $16, %rax
	mov (%rax), %rax
	mov %rax, %rcx
	mov $11, %rax
	add %rax, %rcx
	movb %bl, (%rcx)
.temp3:
	mov $1, %rax
	cmp $0, %rax
	je .temp4
	movabsq $putstr, %rax
	mov %rax, %rbx
	mov $14, %rax
	push %rax
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	push %rax
	call *%rbx
	add $24, %rsp
	mov $1, %rax
	mov %rax, %rbx
	movl -24(%rbp), %eax
	cltq
	sub %rbx, %rax
	mov %rax, %rbx
	mov %rbp, %rdx
	sub $24, %rdx
	movl %ebx, (%rdx)
	mov $0, %rax
	mov %rax, %rbx
	movl -24(%rbp), %eax
	cltq
	cmp %rbx, %rax
	setl %al
	movzbq %al, %rax
	cmp $0, %rax
	jne .temp4
	jmp .temp3
.temp4:
	movabsq $putstr, %rax
	mov %rax, %rbx
	mov $12, %rax
	push %rax
	mov %rbp, %rax
	sub $16, %rax
	mov (%rax), %rax
	push %rax
	call *%rbx
	add $24, %rsp
	mov %rax, %rbx
	mov $1, %rax
	add %rbx, %rax
	mov %rax, %rbx
	mov %rbp, %rdx
	sub $24, %rdx
	movl %ebx, (%rdx)
	mov $65, %rax
	mov %rax, %rbx
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	mov %rax, %rcx
	mov $0, %rax
	add %rax, %rcx
	movb %bl, (%rcx)
.temp5:
	mov $1, %rax
	cmp $0, %rax
	je .temp6
	movabsq $putstr, %rax
	mov %rax, %rbx
	mov $14, %rax
	push %rax
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	push %rax
	call *%rbx
	add $24, %rsp
	mov $1, %rax
	mov %rax, %rbx
	movl -24(%rbp), %eax
	cltq
	sub %rbx, %rax
	mov %rax, %rbx
	mov %rbp, %rdx
	sub $24, %rdx
	movl %ebx, (%rdx)
	mov $0, %rax
	mov %rax, %rbx
	movl -24(%rbp), %eax
	cltq
	cmp %rbx, %rax
	setl %al
	movzbq %al, %rax
	cmp $0, %rax
	jne .temp6
	jmp .temp5
.temp6:
	leave
	ret
