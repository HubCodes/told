// func balloc(factor: int): *char
// balloc provides anonymous memory mapping by sys_mmap (512 * factor byte)
// balloc recommends big size allocation

.global balloc
balloc:
	movl 8(%rsp), %eax
	movl $512, %ecx
	mul %ecx
	mov %rax, %rsi
	mov $9, %rax
	mov $0, %rdi
	mov $3, %rdx
	mov $34, %r10
	mov $-1, %r8
	mov $0, %r9
	syscall
	jmp *(%rsp)
