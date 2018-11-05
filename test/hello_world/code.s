
.section .data
.temp1:
	.asciz "Hello, world!\n"
.section .text
#   FunctionDefAST::codegen
.global main
main:
	push %rbp
	mov %rsp, %rbp
	sub $16, %rsp
#   BlockAST::codegen
#   VarDefAST::codegen
#   StringAST::codegen
	movabs $.temp1, %rax
	mov %rax, -8(%rbp)
#   VarDefAST::codegen
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $strsize, %rax
	mov %rax, %r10
#   VariableAST::codegen
#   loadVar
	mov %rbp, %rax
	sub $8, %rax
	mov (%rax), %rax
	push %rax
	call *%r10
	add $8, %rsp
	movl %eax, -16(%rbp)
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $putstr, %rax
	mov %rax, %r10
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
	call *%r10
	add $16, %rsp
#   BreakAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	leave
	ret
