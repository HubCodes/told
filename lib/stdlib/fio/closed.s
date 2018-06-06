// func closed(fd: int): int
// wrapping sys_close

.global closed
closed:
	mov $3, %rax
	mov 8(%rsp), %rdi
	syscall
	jmp *(%rsp)
