
.section .data
.section .text
#   FunctionDefAST::codegen
.global putstrln
putstrln:
	push %rbp
	mov %rsp, %rbp
	sub $16, %rsp
	mov 16(%rbp), %rax
	mov %rax, -8(%rbp)
#   BlockAST::codegen
#   VarDefAST::codegen
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $strsize, %rax
	push %rax
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
	movl %eax, -16(%rbp)
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $putstr, %rax
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -16(%rbp), %eax
	cltq
	push %rax
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	push %rax
	mov 16(%rsp), %r10
	call *%r10
	add $24, %rsp
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $putc, %rax
	push %rax
#   NumberAST::codegen
	mov $10, %rax
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
#   BreakAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -16(%rbp), %eax
	cltq
	leave
	ret
