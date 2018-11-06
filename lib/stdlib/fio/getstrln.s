
.section .data
.temp1:
	.asciz "  "
.section .text
#   FunctionDefAST::codegen
.global getstrln
getstrln:
	push %rbp
	mov %rsp, %rbp
	sub $24, %rsp
	mov 16(%rbp), %rax
	mov %rax, -8(%rbp)
#   BlockAST::codegen
#   VarDefAST::codegen
#   StringAST::codegen
	movabs $.temp1, %rax
	mov %rax, -16(%rbp)
#   VarDefAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	movl %eax, -24(%rbp)
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $getstr, %rax
	push %rax
#   NumberAST::codegen
	mov $1, %rax
	push %rax
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $16, %rax
	mov (%rax), %rax
	push %rax
	mov 16(%rsp), %r10
	call *%r10
	add $24, %rsp
#   ForAST::codegen
.temp2:
#   BinaryExprAST::codegen
#   PointerDerefAST::codegen
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $16, %rax
	mov (%rax), %rax
	push %rax
#   NumberAST::codegen
	mov $0, %rax
	pop %rbx
	add %rax, %rbx
	movzbq (%rbx), %rax
	push %rax
#   NumberAST::codegen
	mov $10, %rax
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	setne %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
	cmp $0, %rax
	je .temp3
#   BlockAST::codegen
#   BinaryExprAST::codegen
#   PointerDerefAST::codegen
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $16, %rax
	mov (%rax), %rax
	push %rax
#   NumberAST::codegen
	mov $0, %rax
	pop %rbx
	add %rax, %rbx
	movzbq (%rbx), %rax
	push %rax
#   pushAssignExpr
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -24(%rbp), %eax
	cltq
	mov %rax, %rcx
	pop %rax
	pop %rbx
	add %rcx, %rax
	movb %bl, (%rax)
#   BinaryExprAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -24(%rbp), %eax
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
	sub $24, %rdx
	pop %rbx
	movl %ebx, (%rdx)
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $getstr, %rax
	push %rax
#   NumberAST::codegen
	mov $1, %rax
	push %rax
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $16, %rax
	mov (%rax), %rax
	push %rax
	mov 16(%rsp), %r10
	call *%r10
	add $24, %rsp
	jmp .temp2
.temp3:
#   BinaryExprAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	push %rax
#   pushAssignExpr
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -24(%rbp), %eax
	cltq
	mov %rax, %rcx
	pop %rax
	pop %rbx
	add %rcx, %rax
	movb %bl, (%rax)
#   BreakAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -24(%rbp), %eax
	cltq
	leave
	ret
