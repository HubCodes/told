// func getstr(buffer: *char, count: int): int
// gets string from stdin with count arg

.global getstr
getstr:
	mov $0, %rax
	mov $0, %rdi
	mov 8(%rsp), %rsi
	mov 16(%rsp), %rdx
	syscall
	jmp *(%rsp)
