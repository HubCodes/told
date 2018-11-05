
.section .data
.section .text
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
	mov %rax, %r10
#   NumberAST::codegen
	mov $100, %rax
	push %rax
	call *%r10
	add $8, %rsp
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $putc, %rax
	mov %rax, %r10
#   NumberAST::codegen
	mov $10, %rax
	push %rax
	call *%r10
	add $8, %rsp
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $puti, %rax
	mov %rax, %r10
#   NumberAST::codegen
	mov $1, %rax
	push %rax
	call *%r10
	add $8, %rsp
#   CallExprAST::codegen
#   VariableAST::codegen
	movabsq $putc, %rax
	mov %rax, %r10
#   NumberAST::codegen
	mov $10, %rax
	push %rax
	call *%r10
	add $8, %rsp
	leave
	ret
