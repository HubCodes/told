
.section .data
.section .text
#   FunctionDefAST::codegen
.global strtoint
strtoint:
	push %rbp
	mov %rsp, %rbp
	sub $48, %rsp
	mov 16(%rbp), %rax
	mov %rax, -8(%rbp)
#   BlockAST::codegen
#   VarDefAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	movl %eax, -16(%rbp)
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
	movl %eax, -24(%rbp)
#   VarDefAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -24(%rbp), %eax
	cltq
	movl %eax, -32(%rbp)
#   VarDefAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	movl %eax, -40(%rbp)
#   VarDefAST::codegen
#   NumberAST::codegen
	mov $1, %rax
	movl %eax, -48(%rbp)
#   ForAST::codegen
.temp1:
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
	cmp %rax, %rbx
	setg %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
	cmp $0, %rax
	je .temp2
#   BlockAST::codegen
#   BinaryExprAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -48(%rbp), %eax
	cltq
	push %rax
#   NumberAST::codegen
	mov $10, %rax
#   pushBinopExpr
	pop %rbx
	imul %rax, %rbx
	mov %rbx, %rax
	push %rax
#   pushAssignExpr
	mov %rbp, %rdx
	sub $48, %rdx
	pop %rbx
	movl %ebx, (%rdx)
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
	sub %rax, %rbx
	mov %rbx, %rax
	push %rax
#   pushAssignExpr
	mov %rbp, %rdx
	sub $24, %rdx
	pop %rbx
	movl %ebx, (%rdx)
	jmp .temp1
.temp2:
#   ForAST::codegen
.temp3:
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -40(%rbp), %eax
	cltq
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -32(%rbp), %eax
	cltq
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	setl %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
	cmp $0, %rax
	je .temp4
#   BlockAST::codegen
#   BinaryExprAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -16(%rbp), %eax
	cltq
	push %rax
#   BinaryExprAST::codegen
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
	movl -40(%rbp), %eax
	cltq
	pop %rbx
	add %rax, %rbx
	movzbq (%rbx), %rax
	push %rax
#   NumberAST::codegen
	mov $48, %rax
#   pushBinopExpr
	pop %rbx
	sub %rax, %rbx
	mov %rbx, %rax
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -48(%rbp), %eax
	cltq
#   pushBinopExpr
	pop %rbx
	imul %rax, %rbx
	mov %rbx, %rax
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
#   BinaryExprAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -48(%rbp), %eax
	cltq
	push %rax
#   NumberAST::codegen
	mov $10, %rax
#   pushBinopExpr
	pop %rbx
	mov %rax, %rcx
	mov %rbx, %rax
	mov %rcx, %rbx
	cqto
	idiv %rbx
	mov %rax, %rbx
	mov %rbx, %rax
	push %rax
#   pushAssignExpr
	mov %rbp, %rdx
	sub $48, %rdx
	pop %rbx
	movl %ebx, (%rdx)
#   BinaryExprAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -40(%rbp), %eax
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
	sub $40, %rdx
	pop %rbx
	movl %ebx, (%rdx)
	jmp .temp3
.temp4:
#   BreakAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -16(%rbp), %eax
	cltq
	leave
	ret
