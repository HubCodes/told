

.section .data
.section .text
#   FunctionDefAST::codegen
.global fibonacci
fibonacci:
	push %rbp
	mov %rsp, %rbp
	sub $8, %rsp
	mov 16(%rbp), %rax
	mov %rax, -8(%rbp)
#   BlockAST::codegen
#   IfAST::codegen
#   BinaryExprAST::codegen
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
	push %rax
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -8(%rbp), %eax
	cltq
	push %rax
#   NumberAST::codegen
	mov $2, %rax
#   pushBinopExpr
	pop %rbx
	cmp %rax, %rbx
	sete %bl
	movzbq %bl, %rbx
	mov %rbx, %rax
#   pushBinopExpr
	pop %rbx
	cmp $0, %rbx
	jne .temp2
	cmp $0, %rax
	jne .temp2
	mov $0, %rbx
	jmp .temp3
.temp2:
	mov $1, %rbx
.temp3:
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
#   BreakAST::codegen
#   BinaryExprAST::codegen
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $fibonacci, %rax
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
	push %rax
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $fibonacci, %rax
	push %rax
#   BinaryExprAST::codegen
#   VariableAST::codegen
#   loadVar
	movl -8(%rbp), %eax
	cltq
	push %rax
#   NumberAST::codegen
	mov $2, %rax
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
	add %rax, %rbx
	mov %rbx, %rax
	leave
	ret
#   FunctionDefAST::codegen
.global main
main:
	push %rbp
	mov %rsp, %rbp
	sub $0, %rsp
#   BlockAST::codegen
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $puti, %rax
	push %rax
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $fibonacci, %rax
	push %rax
#   NumberAST::codegen
	mov $10, %rax
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
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
