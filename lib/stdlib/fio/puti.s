
.section .data
.temp1:
	.asciz "\0\0\0\0\0\0\0\0\0\0\0"
.section .text
#   FunctionDefAST::codegen
.global puti
puti:
	push %rbp
	mov %rsp, %rbp
	sub $64, %rsp
	mov 16(%rbp), %rax
	mov %rax, -8(%rbp)
#   BlockAST::codegen
#   VarDefAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	movl %eax, -16(%rbp)
#   VarDefAST::codegen
#   StringAST::codegen
	movabs $.temp1, %rax
	mov %rax, -24(%rbp)
#   VarDefAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	movl %eax, -32(%rbp)
#   VarDefAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	movl %eax, -40(%rbp)
#   VarDefAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -8(%rbp), %eax
	cltq
	movl %eax, -48(%rbp)
#   VarDefAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	movl %eax, -56(%rbp)
#   VarDefAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	movl %eax, -64(%rbp)
#   IfAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -8(%rbp), %eax
	cltq
	push %rax
#   NumberAST::codegen
	mov $0, %rax
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	sete %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
	cmp $0, %rax
	je .temp2
#   BlockAST::codegen
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $putc, %rax
	push %rax
#   NumberAST::codegen
	mov $48, %rax
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
#   BreakAST::codegen
#   NumberAST::codegen
	mov $1, %rax
	leave
	ret
.temp2:
#   ForAST::codegen
.temp3:
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -48(%rbp), %eax
	cltq
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
	je .temp4
#   BlockAST::codegen
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
	jmp .temp3
.temp4:
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -40(%rbp), %eax
	cltq
	push %rax
#   pushAssignExpr
	mov %rbp, %rdx
	sub $32, %rdx
	pop %rbx
	movl %ebx, (%rdx)
#   ForAST::codegen
.temp5:
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -40(%rbp), %eax
	cltq
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
	je .temp6
#   BlockAST::codegen
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
	sub %rax, %rbx
	mov %rbx, %rax
	push %rax
#   pushAssignExpr
	mov %rbp, %rdx
	sub $56, %rdx
	pop %rbx
	movl %ebx, (%rdx)
#   BinaryExprAST::codegen
#   BinaryExprAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -8(%rbp), %eax
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
	mov %rdx, %rbx
	mov %rbx, %rax
	push %rax
#   NumberAST::codegen
	mov $48, %rax
#   pushBinopExpr
	pop %rbx
	add %rax, %rbx
	mov %rbx, %rax
	push %rax
#   pushAssignExpr
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $24, %rax
	mov (%rax), %rax
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -56(%rbp), %eax
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
	movl -8(%rbp), %eax
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
	sub $8, %rdx
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
	sub %rax, %rbx
	mov %rbx, %rax
	push %rax
#   pushAssignExpr
	mov %rbp, %rdx
	sub $40, %rdx
	pop %rbx
	movl %ebx, (%rdx)
	jmp .temp5
.temp6:
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -32(%rbp), %eax
	cltq
	push %rax
#   pushAssignExpr
	mov %rbp, %rdx
	sub $64, %rdx
	pop %rbx
	movl %ebx, (%rdx)
#   BinaryExprAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	push %rax
#   pushAssignExpr
	mov %rbp, %rdx
	sub $56, %rdx
	pop %rbx
	movl %ebx, (%rdx)
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $putstr, %rax
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -32(%rbp), %eax
	cltq
	push %rax
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $24, %rax
	mov (%rax), %rax
	push %rax
	mov 16(%rsp), %r10
	call *%r10
	add $24, %rsp
#   ForAST::codegen
.temp7:
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -56(%rbp), %eax
	cltq
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -64(%rbp), %eax
	cltq
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	setl %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
	cmp $0, %rax
	je .temp8
#   BlockAST::codegen
#   BinaryExprAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	push %rax
#   pushAssignExpr
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $24, %rax
	mov (%rax), %rax
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -56(%rbp), %eax
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
	movl -56(%rbp), %eax
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
	sub $56, %rdx
	pop %rbx
	movl %ebx, (%rdx)
	jmp .temp7
.temp8:
#   BreakAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -40(%rbp), %eax
	cltq
	leave
	ret
