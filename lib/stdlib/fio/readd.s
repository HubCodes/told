// func readd(fd: int, buffer: *char, count: int): int
// wrapping sys_read

.global readd
readd:
	mov $0, %rax
	mov 8(%rsp), %rdi
	mov 16(%rsp), %rsi
	mov 24(%rsp), %rdx
	syscall
	jmp *(%rsp)
