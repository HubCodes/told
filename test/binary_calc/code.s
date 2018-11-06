
.section .data
.temp1:
	.asciz "  "
.temp2:
	.asciz "Input number 1 >"
.temp3:
	.asciz "Input operator >"
.temp4:
	.asciz "Input number 2 >"
.temp5:
	.asciz "="
.section .text
#   FunctionDefAST::codegen
.global main
main:
	push %rbp
	mov %rsp, %rbp
	sub $32, %rsp
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
#   StringAST::codegen
	movabs $.temp1, %rax
	mov %rax, -16(%rbp)
#   VarDefAST::codegen
#   VarDefAST::codegen
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $putstrln, %rax
	push %rax
#   StringAST::codegen
	movabs $.temp2, %rax
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
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
	sub $24, %rdx
	pop %rbx
	movl %ebx, (%rdx)
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $putstrln, %rax
	push %rax
#   StringAST::codegen
	movabs $.temp3, %rax
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
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
#   PointerDerefAST::codegen
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	push %rax
#   NumberAST::codegen
	mov $0, %rax
	pop %rbx
	add %rax, %rbx
	movzbq (%rbx), %rax
	push %rax
#   pushAssignExpr
	mov %rbp, %rdx
	sub $16, %rdx
	pop %rbx
	movq %rbx, (%rdx)
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $putstrln, %rax
	push %rax
#   StringAST::codegen
	movabs $.temp4, %rax
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
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
	sub $32, %rdx
	pop %rbx
	movl %ebx, (%rdx)
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $puti, %rax
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -24(%rbp), %eax
	cltq
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $putc, %rax
	push %rax
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $16, %rax
	mov (%rax), %rax
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $puti, %rax
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -32(%rbp), %eax
	cltq
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $putstrln, %rax
	push %rax
#   StringAST::codegen
	movabs $.temp5, %rax
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
#   IfAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $16, %rax
	mov (%rax), %rax
	push %rax
#   NumberAST::codegen
	mov $43, %rax
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	sete %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
	cmp $0, %rax
	je .temp6
#   BlockAST::codegen
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $puti, %rax
	push %rax
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -24(%rbp), %eax
	cltq
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -32(%rbp), %eax
	cltq
#   pushBinopExpr
	pop %rbx
	add %rax, %rbx
	mov %rbx, %rax
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
.temp6:
#   BlockAST::codegen
#   IfAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $16, %rax
	mov (%rax), %rax
	push %rax
#   NumberAST::codegen
	mov $45, %rax
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	sete %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
	cmp $0, %rax
	je .temp7
#   BlockAST::codegen
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $puti, %rax
	push %rax
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -24(%rbp), %eax
	cltq
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -32(%rbp), %eax
	cltq
#   pushBinopExpr
	pop %rbx
	sub %rax, %rbx
	mov %rbx, %rax
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
.temp7:
#   BlockAST::codegen
#   IfAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $16, %rax
	mov (%rax), %rax
	push %rax
#   NumberAST::codegen
	mov $42, %rax
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	sete %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
	cmp $0, %rax
	je .temp8
#   BlockAST::codegen
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $puti, %rax
	push %rax
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -24(%rbp), %eax
	cltq
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -32(%rbp), %eax
	cltq
#   pushBinopExpr
	pop %rbx
	imul %rax, %rbx
	mov %rbx, %rax
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
.temp8:
#   BlockAST::codegen
#   IfAST::codegen
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $16, %rax
	mov (%rax), %rax
	push %rax
#   NumberAST::codegen
	mov $47, %rax
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	sete %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
	cmp $0, %rax
	je .temp9
#   BlockAST::codegen
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $puti, %rax
	push %rax
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -24(%rbp), %eax
	cltq
	push %rax
#   VariableAST::codegen
#   loadVar
	movl -32(%rbp), %eax
	cltq
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
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
.temp9:
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
	leave
	ret
