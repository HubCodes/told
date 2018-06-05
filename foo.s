

.section .data
.temp1:
	.asciz "Hello, world!\n"
.temp2:
	.asciz "./code.txt"
.section .text
.global foo
foo:
	push %rbp
	mov %rsp, %rbp
	sub $8, %rsp
	mov 16(%rbp), %rax
	mov %rax, -8(%rbp)
	mov $9, %rax
	leave
	ret
.global main
main:
	push %rbp
	mov %rsp, %rbp
	sub $40, %rsp
	movabs $.temp1, %rax
	mov %rax, -8(%rbp)
	movabsq $balloc, %rax
	mov %rax, %rbx
	mov $1, %rax
	push %rax
	call *%rbx
	add $16, %rsp
	mov %rax, -16(%rbp)
	movabsq $foo, %rax
	mov %rax, -24(%rbp)
	mov $10, %rax
	movl %eax, -32(%rbp)
	movabsq $opend, %rax
	mov %rax, %rbx
	mov $493, %rax
	push %rax
	mov $0, %rax
	push %rax
	movabs $.temp2, %rax
	push %rax
	call *%rbx
	add $32, %rsp
	movl %eax, -40(%rbp)
	movabsq $readd, %rax
	mov %rax, %rbx
	mov $500, %rax
	push %rax
	mov %rbp, %rax
	sub $16, %rax
	mov (%rax), %rax
	push %rax
	movl -40(%rbp), %eax
	cltq
	push %rax
	call *%rbx
	add $32, %rsp
	movabsq $putstr, %rax
	mov %rax, %rbx
	mov $500, %rax
	push %rax
	mov %rbp, %rax
	sub $16, %rax
	mov (%rax), %rax
	push %rax
	call *%rbx
	add $24, %rsp
	movabsq $closed, %rax
	mov %rax, %rbx
	movl -40(%rbp), %eax
	cltq
	push %rax
	call *%rbx
	add $16, %rsp
	movabsq $bfree, %rax
	mov %rax, %rbx
	mov $1, %rax
	push %rax
	mov %rbp, %rax
	sub $16, %rax
	mov (%rax), %rax
	push %rax
	call *%rbx
	add $24, %rsp
	mov $66, %rax
	mov %rax, %rbx
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	mov %rax, %rcx
	mov $0, %rax
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
	movl -32(%rbp), %eax
	cltq
	sub %rbx, %rax
	mov %rax, %rbx
	mov %rbp, %rdx
	sub $32, %rdx
	movl %ebx, (%rdx)
	mov $0, %rax
	mov %rax, %rbx
	movl -32(%rbp), %eax
	cltq
	cmp %rbx, %rax
	setl %al
	movzbq %al, %rax
	cmp $0, %rax
	jne .temp4
	jmp .temp3
.temp4:
	mov %rbp, %rax
	sub $24, %rax
	mov (%rax), %rax
	mov %rax, %rbx
	mov $4, %rax
	push %rax
	call *%rbx
	add $16, %rsp
	mov %rax, %rbx
	mov $1, %rax
	add %rbx, %rax
	mov %rax, %rbx
	mov %rbp, %rdx
	sub $32, %rdx
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
	movl -32(%rbp), %eax
	cltq
	sub %rbx, %rax
	mov %rax, %rbx
	mov %rbp, %rdx
	sub $32, %rdx
	movl %ebx, (%rdx)
	mov $0, %rax
	mov %rax, %rbx
	movl -32(%rbp), %eax
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
