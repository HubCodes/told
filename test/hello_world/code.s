
.section .data
.temp1:
	.asciz "Hello, world!"
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
	movabsq $putstrln, %rax
	push %rax
#   StringAST::codegen
	movabs $.temp1, %rax
	push %rax
	mov 8(%rsp), %r10
	call *%r10
	add $16, %rsp
#   BreakAST::codegen
#   NumberAST::codegen
	mov $0, %rax
	leave
	ret
