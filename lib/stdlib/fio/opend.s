// func opend(filepath: *char, option: int, mode: int): int
// wrapping sys_open

.global opend
opend:
	mov $2, %rax
	mov 8(%rsp), %rdi
	mov 16(%rsp), %rsi
	mov 24(%rsp), %rdx
	syscall
	jmp *(%rsp)
