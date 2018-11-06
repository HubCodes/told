

.section .data
.temp21:
	.long 5
	.long 3
	.long 7
	.long 9
	.long 1
	.long 10
	.long 3
	.long 5
.section .text
#   FunctionDefAST::codegen
.global quick_sort
quick_sort:
	push %rbp
	mov %rsp, %rbp
	sub $56, %rsp
	mov 16(%rbp), %rax
	mov %rax, -8(%rbp)
	mov 24(%rbp), %rax
	mov %rax, -16(%rbp)
	mov 32(%rbp), %rax
	mov %rax, -24(%rbp)
#   BlockAST::codegen
#   VarDefAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -16(%rbp), %eax
	cltq
	movl %eax, -32(%rbp)
#   VarDefAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -24(%rbp), %eax
	cltq
	movl %eax, -40(%rbp)
#   VarDefAST::codegen
#   PointerDerefAST::codegen
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	push %rax
#   BinaryExprAST::codegen
#   BinaryExprAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -16(%rbp), %eax
	cltq
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -24(%rbp), %eax
	cltq
#   pushBinopExpr
	pop %rbx
	add %rax, %rbx
	mov %rbx, %rax
	push %rax
#   NumberAST::codegen
	mov $2, %rax
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
#   NumberAST::codegen
	mov $4, %rax
#   pushBinopExpr
	pop %rbx
	imul %rax, %rbx
	mov %rbx, %rax
	pop %rbx
	add %rax, %rbx
	movl (%rbx), %eax
	movl %eax, -48(%rbp)
#   VarDefAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	movl %eax, -56(%rbp)
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
	imul %rax, %rbx
	mov %rbx, %rax
	pop %rbx
	add %rax, %rbx
	movl (%rbx), %eax
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -48(%rbp), %eax
	cltq
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	setl %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
	cmp $0, %rax
	je .temp2
#   BlockAST::codegen
#   BinaryExprAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -32(%rbp), %eax
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
	sub $32, %rdx
	pop %rbx
	movl %ebx, (%rdx)
	jmp .temp1
.temp2:
#   ForAST::codegen
.temp3:
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
	movl -40(%rbp), %eax
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
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -48(%rbp), %eax
	cltq
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	setg %bl
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
	sub %rax, %rbx
	mov %rbx, %rax
	push %rax
#   pushAssignExpr
	mov %rbp, %rdx
	sub $40, %rdx
	pop %rbx
	movl %ebx, (%rdx)
	jmp .temp3
.temp4:
#   IfAST::codegen
#   BinaryExprAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -32(%rbp), %eax
	cltq
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -40(%rbp), %eax
	cltq
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
	movl -32(%rbp), %eax
	cltq
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -40(%rbp), %eax
	cltq
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	sete %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
#   pushBinopExpr
	pop %rbx
	cmp $0, %rbx
	jne .temp6
	cmp $0, %rax
	jne .temp6
	mov $0, %rbx
	jmp .temp7
.temp6:
	mov $1, %rbx
.temp7:
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
	imul %rax, %rbx
	mov %rbx, %rax
	pop %rbx
	add %rax, %rbx
	movl (%rbx), %eax
	push %rax
#   pushAssignExpr
	mov %rbp, %rdx
	sub $56, %rdx
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
	movl -40(%rbp), %eax
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
	imul %rax, %rbx
	mov %rbx, %rax
	mov %rax, %rcx
	pop %rax
	pop %rbx
	add %rcx, %rax
	movl %ebx, (%rax)
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -56(%rbp), %eax
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
	movl -40(%rbp), %eax
	cltq
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
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -32(%rbp), %eax
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
	sub $32, %rdx
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
.temp5:
#   ForAST::codegen
.temp8:
#   BinaryExprAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -32(%rbp), %eax
	cltq
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -40(%rbp), %eax
	cltq
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
	movl -32(%rbp), %eax
	cltq
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -40(%rbp), %eax
	cltq
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	sete %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
#   pushBinopExpr
	pop %rbx
	cmp $0, %rbx
	jne .temp10
	cmp $0, %rax
	jne .temp10
	mov $0, %rbx
	jmp .temp11
.temp10:
	mov $1, %rbx
.temp11:
	mov %rbx, %rax
	cmp $0, %rax
	je .temp9
#   BlockAST::codegen
#   ForAST::codegen
.temp12:
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
	imul %rax, %rbx
	mov %rbx, %rax
	pop %rbx
	add %rax, %rbx
	movl (%rbx), %eax
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -48(%rbp), %eax
	cltq
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	setl %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
	cmp $0, %rax
	je .temp13
#   BlockAST::codegen
#   BinaryExprAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -32(%rbp), %eax
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
	sub $32, %rdx
	pop %rbx
	movl %ebx, (%rdx)
	jmp .temp12
.temp13:
#   ForAST::codegen
.temp14:
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
	movl -40(%rbp), %eax
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
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -48(%rbp), %eax
	cltq
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	setg %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
	cmp $0, %rax
	je .temp15
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
	sub $40, %rdx
	pop %rbx
	movl %ebx, (%rdx)
	jmp .temp14
.temp15:
#   IfAST::codegen
#   BinaryExprAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -32(%rbp), %eax
	cltq
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -40(%rbp), %eax
	cltq
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
	movl -32(%rbp), %eax
	cltq
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -40(%rbp), %eax
	cltq
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	sete %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
#   pushBinopExpr
	pop %rbx
	cmp $0, %rbx
	jne .temp17
	cmp $0, %rax
	jne .temp17
	mov $0, %rbx
	jmp .temp18
.temp17:
	mov $1, %rbx
.temp18:
	mov %rbx, %rax
	cmp $0, %rax
	je .temp16
#   BlockAST::codegen
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
	imul %rax, %rbx
	mov %rbx, %rax
	pop %rbx
	add %rax, %rbx
	movl (%rbx), %eax
	push %rax
#   pushAssignExpr
	mov %rbp, %rdx
	sub $56, %rdx
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
	movl -40(%rbp), %eax
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
	imul %rax, %rbx
	mov %rbx, %rax
	mov %rax, %rcx
	pop %rax
	pop %rbx
	add %rcx, %rax
	movl %ebx, (%rax)
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -56(%rbp), %eax
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
	movl -40(%rbp), %eax
	cltq
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
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -32(%rbp), %eax
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
	sub $32, %rdx
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
.temp16:
	jmp .temp8
.temp9:
#   IfAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -16(%rbp), %eax
	cltq
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -40(%rbp), %eax
	cltq
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	setl %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
	cmp $0, %rax
	je .temp19
#   BlockAST::codegen
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $quick_sort, %rax
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -40(%rbp), %eax
	cltq
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
	mov 24(%rsp), %r10
	call *%r10
	add $32, %rsp
.temp19:
#   IfAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -32(%rbp), %eax
	cltq
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -24(%rbp), %eax
	cltq
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	setl %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
	cmp $0, %rax
	je .temp20
#   BlockAST::codegen
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $quick_sort, %rax
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -24(%rbp), %eax
	cltq
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -32(%rbp), %eax
	cltq
	push %rax
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	push %rax
	mov 24(%rsp), %r10
	call *%r10
	add $32, %rsp
.temp20:
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
	movabsq $.temp21, %rax
	mov %rax, -8(%rbp)
#   VarDefAST::codegen
#   NumberAST::codegen
	mov $8, %rax
	movl %eax, -16(%rbp)
#   VarDefAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	movl %eax, -24(%rbp)
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $quick_sort, %rax
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -16(%rbp), %eax
	cltq
	push %rax
#   NumberAST::codegen
	mov $0, %rax
	push %rax
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	push %rax
	mov 24(%rsp), %r10
	call *%r10
	add $32, %rsp
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $putc, %rax
	push %rax
#   NumberAST::codegen
	mov $123, %rax
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
#   ForAST::codegen
.temp22:
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
	je .temp23
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
	mov $44, %rax
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
	jmp .temp22
.temp23:
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $putc, %rax
	push %rax
#   NumberAST::codegen
	mov $125, %rax
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
