// func writed(fd: int, buffer: *char, count: int): int
// wrapping sys_write

.global writed
writed:
	mov $1, %rax
	mov 8(%rsp), %rdi
	mov 16(%rsp), %rsi
	mov 24(%rsp), %rdx
	syscall
	jmp *(%rsp)
