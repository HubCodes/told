// func putc(ch: int): int
// put 1 character

.section .data
.L0:
	.ascii "  "

.section .text
.global putc
putc:
	movl 8(%rsp), %eax
	lea .L0, %rcx
	movb %al, (%rcx)
	movabsq $.L0, %rsi
	mov $1, %rax
	mov $1, %rdi
	mov $1, %rdx
	syscall
	jmp *(%rsp)
