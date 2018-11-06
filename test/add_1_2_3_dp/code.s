

.section .data
.temp5:
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
.temp6:
	.asciz "          "
.section .text
#   FunctionDefAST::codegen
.global solve
solve:
	push %rbp
	mov %rsp, %rbp
	sub $24, %rsp
	mov 16(%rbp), %rax
	mov %rax, -8(%rbp)
	mov 24(%rbp), %rax
	mov %rax, -16(%rbp)
#   BlockAST::codegen
#   VarDefAST::codegen
#   BinaryExprAST::codegen
#   NumberAST::codegen
	mov $4, %rax
	push %rax
#   NumberAST::codegen
	mov $4, %rax
#   pushBinopExpr
	pop %rbx
	imul %rax, %rbx
	mov %rbx, %rax
	movl %eax, -24(%rbp)
#   BinaryExprAST::codegen
#   NumberAST::codegen
	mov $1, %rax
	push %rax
#   pushAssignExpr
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	push %rax
#   BinaryExprAST::codegen
#   NumberAST::codegen
	mov $1, %rax
	push %rax
#   NumberAST::codegen
	mov $4, %rax
#   pushBinopExpr
	pop %rbx
	imul %rax, %rbx
	mov %rbx, %rax
	mov %rax, %rcx
	pop %rax
	pop %rbx
	add %rcx, %rax
	movl %ebx, (%rax)
#   BinaryExprAST::codegen
#   NumberAST::codegen
	mov $2, %rax
	push %rax
#   pushAssignExpr
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	push %rax
#   BinaryExprAST::codegen
#   NumberAST::codegen
	mov $2, %rax
	push %rax
#   NumberAST::codegen
	mov $4, %rax
#   pushBinopExpr
	pop %rbx
	imul %rax, %rbx
	mov %rbx, %rax
	mov %rax, %rcx
	pop %rax
	pop %rbx
	add %rcx, %rax
	movl %ebx, (%rax)
#   BinaryExprAST::codegen
#   NumberAST::codegen
	mov $4, %rax
	push %rax
#   pushAssignExpr
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	push %rax
#   BinaryExprAST::codegen
#   NumberAST::codegen
	mov $3, %rax
	push %rax
#   NumberAST::codegen
	mov $4, %rax
#   pushBinopExpr
	pop %rbx
	imul %rax, %rbx
	mov %rbx, %rax
	mov %rax, %rcx
	pop %rax
	pop %rbx
	add %rcx, %rax
	movl %ebx, (%rax)
#   ForAST::codegen
.temp1:
#   BinaryExprAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -24(%rbp), %eax
	cltq
	push %rax
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -16(%rbp), %eax
	cltq
	push %rax
#   NumberAST::codegen
	mov $4, %rax
#   pushBinopExpr
	pop %rbx
	imul %rax, %rbx
	mov %rbx, %rax
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	setl %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
	push %rax
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -24(%rbp), %eax
	cltq
	push %rax
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -16(%rbp), %eax
	cltq
	push %rax
#   NumberAST::codegen
	mov $4, %rax
#   pushBinopExpr
	pop %rbx
	imul %rax, %rbx
	mov %rbx, %rax
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	sete %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
#   pushBinopExpr
	pop %rbx
	cmp $0, %rbx
	jne .temp3
	cmp $0, %rax
	jne .temp3
	mov $0, %rbx
	jmp .temp4
.temp3:
	mov $1, %rbx
.temp4:
	mov %rbx, %rax
	cmp $0, %rax
	je .temp2
#   BlockAST::codegen
#   BinaryExprAST::codegen
#   BinaryExprAST::codegen
#   BinaryExprAST::codegen
#   PointerDerefAST::codegen
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	push %rax
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -24(%rbp), %eax
	cltq
	push %rax
#   BinaryExprAST::codegen
#   NumberAST::codegen
	mov $1, %rax
	push %rax
#   NumberAST::codegen
	mov $4, %rax
#   pushBinopExpr
	pop %rbx
	imul %rax, %rbx
	mov %rbx, %rax
#   pushBinopExpr
	pop %rbx
	sub %rax, %rbx
	mov %rbx, %rax
	pop %rbx
	add %rax, %rbx
	movl (%rbx), %eax
	push %rax
#   PointerDerefAST::codegen
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	push %rax
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -24(%rbp), %eax
	cltq
	push %rax
#   BinaryExprAST::codegen
#   NumberAST::codegen
	mov $2, %rax
	push %rax
#   NumberAST::codegen
	mov $4, %rax
#   pushBinopExpr
	pop %rbx
	imul %rax, %rbx
	mov %rbx, %rax
#   pushBinopExpr
	pop %rbx
	sub %rax, %rbx
	mov %rbx, %rax
	pop %rbx
	add %rax, %rbx
	movl (%rbx), %eax
#   pushBinopExpr
	pop %rbx
	add %rax, %rbx
	mov %rbx, %rax
	push %rax
#   PointerDerefAST::codegen
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	push %rax
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -24(%rbp), %eax
	cltq
	push %rax
#   BinaryExprAST::codegen
#   NumberAST::codegen
	mov $3, %rax
	push %rax
#   NumberAST::codegen
	mov $4, %rax
#   pushBinopExpr
	pop %rbx
	imul %rax, %rbx
	mov %rbx, %rax
#   pushBinopExpr
	pop %rbx
	sub %rax, %rbx
	mov %rbx, %rax
	pop %rbx
	add %rax, %rbx
	movl (%rbx), %eax
#   pushBinopExpr
	pop %rbx
	add %rax, %rbx
	mov %rbx, %rax
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
	movl %ebx, (%rax)
#   BinaryExprAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -24(%rbp), %eax
	cltq
	push %rax
#   NumberAST::codegen
	mov $4, %rax
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
	jmp .temp1
.temp2:
#   BreakAST::codegen
#   PointerDerefAST::codegen
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	push %rax
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -16(%rbp), %eax
	cltq
	push %rax
#   NumberAST::codegen
	mov $4, %rax
#   pushBinopExpr
	pop %rbx
	imul %rax, %rbx
	mov %rbx, %rax
	pop %rbx
	add %rax, %rbx
	movl (%rbx), %eax
	leave
	ret
#   FunctionDefAST::codegen
.global main
main:
	push %rbp
	mov %rsp, %rbp
	sub $32, %rsp
#   BlockAST::codegen
#   VarDefAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	movl %eax, -8(%rbp)
#   VarDefAST::codegen
#   ArrayAST::codegen
	movabsq $.temp5, %rax
	mov %rax, -16(%rbp)
#   VarDefAST::codegen
#   VarDefAST::codegen
#   StringAST::codegen
	movabs $.temp6, %rax
	mov %rax, -32(%rbp)
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $getstrln, %rax
	push %rax
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $32, %rax
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
	sub $32, %rax
	mov (%rax), %rax
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
	push %rax
#   pushAssignExpr
	mov %rbp, %rdx
	sub $24, %rdx
	pop %rbx
	movl %ebx, (%rdx)
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $puti, %rax
	push %rax
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $solve, %rax
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -24(%rbp), %eax
	cltq
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
