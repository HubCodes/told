
.section .data
.section .text
#   FunctionDefAST::codegen
.global main
main:
	push %rbp
	mov %rsp, %rbp
	sub $16, %rsp
#   BlockAST::codegen
#   VarDefAST::codegen
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $balloc, %rax
	push %rax
#   NumberAST::codegen
	mov $1, %rax
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
	mov %rax, -8(%rbp)
#   VarDefAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	movl %eax, -16(%rbp)
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $getstrln, %rax
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
#   BinaryExprAST::codegen
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $strtoint, %rax
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
	push %rax
#   pushAssignExpr
	mov %rbp, %rdx
	sub $16, %rdx
	pop %rbx
	movl %ebx, (%rdx)
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $puti, %rax
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -16(%rbp), %eax
	cltq
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
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
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $bfree, %rax
	push %rax
#   NumberAST::codegen
	mov $1, %rax
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
#   BreakAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	leave
	ret
