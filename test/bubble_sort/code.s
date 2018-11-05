

.section .data
.temp6:
	.long 5
	.long 3
	.long 7
	.long 9
	.long 1
.section .text
#   FunctionDefAST::codegen
.global bubble_sort
bubble_sort:
	push %rbp
	mov %rsp, %rbp
	sub $40, %rsp
	mov 16(%rbp), %rax
	mov %rax, -8(%rbp)
	mov 24(%rbp), %rax
	mov %rax, -16(%rbp)
#   BlockAST::codegen
#   VarDefAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	movl %eax, -24(%rbp)
#   VarDefAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	movl %eax, -32(%rbp)
#   VarDefAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	movl %eax, -40(%rbp)
#   ForAST::codegen
.temp1:
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
	cmp $0, %rax
	je .temp2
#   BlockAST::codegen
#   ForAST::codegen
.temp3:
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -32(%rbp), %eax
	cltq
	push %rax
#   BinaryExprAST::codegen
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
	push %rax
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
#   pushBinopExpr
	pop %rbx
	sub %rax, %rbx
	mov %rbx, %rax
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	setl %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
	cmp $0, %rax
	je .temp4
#   BlockAST::codegen
#   IfAST::codegen
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
	movl -32(%rbp), %eax
	cltq
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
	movl -32(%rbp), %eax
	cltq
	push %rax
#   NumberAST::codegen
	mov $4, %rax
#   pushBinopExpr
	pop %rbx
	add %rax, %rbx
	mov %rbx, %rax
	pop %rbx
	add %rax, %rbx
	movl (%rbx), %eax
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	setg %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
	cmp $0, %rax
	je .temp5
#   BlockAST::codegen
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
	movl -32(%rbp), %eax
	cltq
	pop %rbx
	add %rax, %rbx
	movl (%rbx), %eax
	push %rax
#   pushAssignExpr
	mov %rbp, %rdx
	sub $40, %rdx
	pop %rbx
	movl %ebx, (%rdx)
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
	movl -32(%rbp), %eax
	cltq
	push %rax
#   NumberAST::codegen
	mov $4, %rax
#   pushBinopExpr
	pop %rbx
	add %rax, %rbx
	mov %rbx, %rax
	pop %rbx
	add %rax, %rbx
	movl (%rbx), %eax
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
	movl -32(%rbp), %eax
	cltq
	mov %rax, %rcx
	pop %rax
	pop %rbx
	add %rcx, %rax
	movl %ebx, (%rax)
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -40(%rbp), %eax
	cltq
	push %rax
#   pushAssignExpr
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	push %rax
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -32(%rbp), %eax
	cltq
	push %rax
#   NumberAST::codegen
	mov $4, %rax
#   pushBinopExpr
	pop %rbx
	add %rax, %rbx
	mov %rbx, %rax
	mov %rax, %rcx
	pop %rax
	pop %rbx
	add %rcx, %rax
	movl %ebx, (%rax)
.temp5:
#   BinaryExprAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -32(%rbp), %eax
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
	sub $32, %rdx
	pop %rbx
	movl %ebx, (%rdx)
	jmp .temp3
.temp4:
#   BinaryExprAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	push %rax
#   pushAssignExpr
	mov %rbp, %rdx
	sub $32, %rdx
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
#   VariableAST::codegen
#   loadVar
	movl -16(%rbp), %eax
	cltq
	leave
	ret
#   FunctionDefAST::codegen
.global main
main:
	push %rbp
	mov %rsp, %rbp
	sub $24, %rsp
#   BlockAST::codegen
#   VarDefAST::codegen
#   ArrayAST::codegen
	movabsq $.temp6, %rax
	mov %rax, -8(%rbp)
#   VarDefAST::codegen
#   NumberAST::codegen
	mov $5, %rax
	movl %eax, -16(%rbp)
#   VarDefAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	movl %eax, -24(%rbp)
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $bubble_sort, %rax
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
#   ForAST::codegen
.temp7:
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
	cmp $0, %rax
	je .temp8
#   BlockAST::codegen
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $puti, %rax
	push %rax
#   PointerDerefAST::codegen
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
	pop %rbx
	add %rax, %rbx
	movl (%rbx), %eax
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
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
	jmp .temp7
.temp8:
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
	leave
	ret
