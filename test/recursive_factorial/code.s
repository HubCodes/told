

.section .data
.section .text
#   FunctionDefAST::codegen
.global factorial
factorial:
	push %rbp
	mov %rsp, %rbp
	sub $8, %rsp
	mov 16(%rbp), %rax
	mov %rax, -8(%rbp)
#   BlockAST::codegen
#   IfAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -8(%rbp), %eax
	cltq
	push %rax
#   NumberAST::codegen
	mov $1, %rax
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	sete %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
	cmp $0, %rax
	je .temp1
#   BlockAST::codegen
#   BreakAST::codegen
#   NumberAST::codegen
	mov $1, %rax
	leave
	ret
.temp1:
#   BlockAST::codegen
#   BreakAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -8(%rbp), %eax
	cltq
	push %rax
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $factorial, %rax
	push %rax
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -8(%rbp), %eax
	cltq
	push %rax
#   NumberAST::codegen
	mov $1, %rax
#   pushBinopExpr
	pop %rbx
	sub %rax, %rbx
	mov %rbx, %rax
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
#   pushBinopExpr
	pop %rbx
	imul %rax, %rbx
	mov %rbx, %rax
	leave
	ret
#   FunctionDefAST::codegen
.global main
main:
	push %rbp
	mov %rsp, %rbp
	sub $8, %rsp
#   BlockAST::codegen
#   VarDefAST::codegen
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $factorial, %rax
	push %rax
#   NumberAST::codegen
	mov $6, %rax
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
	movl %eax, -8(%rbp)
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $puti, %rax
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -8(%rbp), %eax
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
#   BreakAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	leave
	ret
