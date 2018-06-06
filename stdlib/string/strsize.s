
.section .data
.section .text
#   FunctionDefAST::codegen
.global strsize
strsize:
	push %rbp
	mov %rsp, %rbp
	sub $16, %rsp
	mov 16(%rbp), %rax
	mov %rax, -8(%rbp)
#   BlockAST::codegen
#   VarDefAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	movl %eax, -16(%rbp)
#   ForAST::codegen
.temp1:
#   BinaryExprAST::codegen
#   PointerDerefAST::codegen
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -16(%rbp), %eax
	cltq
	pop %rbx
	add %rax, %rbx
	movzbq (%rbx), %rax
	push %rax
#   NumberAST::codegen
	mov $0, %rax
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	setne %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
	cmp $0, %rax
	je .temp2
#   BlockAST::codegen
#   BinaryExprAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -16(%rbp), %eax
	cltq
	push %rax
#   NumberAST::codegen
	mov $1, %rax
#   pushBinopExpr
	pop %rbx
	add %rax, %rbx
	mov %rbx, %rax
	push %rax
#   pushAssignExpr
	mov %rbp, %rdx
	sub $16, %rdx
	pop %rbx
	movl %ebx, (%rdx)
	jmp .temp1
.temp2:
#   BreakAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -16(%rbp), %eax
	cltq
	leave
	ret
