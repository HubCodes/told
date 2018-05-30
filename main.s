
.global main
main:
	push %rbp
	mov %rsp, %rbp
	sub $8, %rsp
	mov $4, %rax
	mov %rax, -8(%rbp)
.temp1:
	mov $2, %rax
	mov %rax, %rbx
	movq -8(%rbp), %rax
	cmp %rbx, %rax
	setg %al
	movzbq %al, %rax
	cmp $0, %rax
	je .temp2
	mov $1, %rax
	mov %rax, %rbx
	movq -8(%rbp), %rax
	sub %rbx, %rax
	mov %rax, %rbx
	mov %rbp, %rdx
	sub $8, %rdx
	movq %rbx, (%rdx)
	jmp .temp1
.temp2:
	leave
	ret
