// func puti(number: int): int
// puts a integer value.
.section .data
.buf:
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0
	.byte 0

.section .text
.global puti
puti:
	mov 8(%rsp), %rax
	cmp $0, %rax
	jl negative
	je zero
	mov $0, %r9
	jmp loop
negative:
	mov $1, %r9
	xor %rcx, %rcx
	sub %rax, %rcx
	mov %rcx, %rax
loop:
	cmp $0, %rax
	
zero:
	lea .buf, %rbx
	movb $48, %al
	movb %al, (%rbx)
	jmp print
print:
