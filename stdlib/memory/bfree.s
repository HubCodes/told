// func bfree(ptr: *char, factor: int): int
// bfree frees memory mapping from balloc

.global bfree
bfree:
	mov 8(%rsp), %rdi
	mov $512, %eax
	movl 16(%rsp), %ecx 
	mul %ecx
	mov %rax, %rsi
	mov $11, %rax
	syscall
	jmp *(%rsp)
