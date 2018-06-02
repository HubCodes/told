// func putstr(buffer: *char, count: int): int
// puts string to stdout

.global putstr
putstr:
	mov $1, %rax
	mov $1, %rdi
	mov 8(%rsp), %rsi
	mov 16(%rsp), %rdx
	syscall
	jmp *(%rsp)
