#include <iostream>
#include <string>
#include <unordered_map>

#include "lexer.hpp"
#include "parser.hpp"

static std::string indent;

#define DEBUG_
#ifdef DEBUG_
#define PRINT(x) std::cout << indent << (x) << '\n';
#define DEL		"------------------------"
#define PUSH	indent += "|   "
#define POP		indent = indent.substr(0, indent.size() - 4)
#else
#define PRINT(x)
#define DEL
#define PUSH
#define POP
#endif

#define D(x) static_cast<int>(x)

static std::unordered_map<int, std::string> opMap = {
	{ D(OpKind::ADD), "+" },
	{ D(OpKind::SUB), "-" },
	{ D(OpKind::MUL), "*" },
	{ D(OpKind::DIV), "/" },
	{ D(OpKind::MOD), "%" },
	{ D(OpKind::AND), "&" },
	{ D(OpKind::OR), "|" },
	{ D(OpKind::NOT), "!" },
	{ D(OpKind::ASSIGN), "=" },
	{ D(OpKind::LT), "<" },
	{ D(OpKind::GT), ">" },
	{ D(OpKind::EQ), "~" },
	{ D(OpKind::NEQ), "^" }
};

void NumberAST::codegen() {
	PRINT("NumericAST");
	PRINT((kind == LiteralKind::CHAR) ? "char data :" + std::to_string(numericData.c) : (kind == LiteralKind::INT) ? "int data: " + std::to_string(numericData.i) : "double data: " + std::to_string(numericData.d));
	PRINT(DEL);
}

void VariableAST::codegen() {
	PRINT("VariableAST");
	PRINT(id);
	PRINT(DEL);
}

void StringAST::codegen() {
	PRINT("StringAST");
	PRINT(str);
	PRINT(DEL);
}

void BinaryExprAST::codegen() {
	PRINT("BinaryExprAST");
	PRINT(opMap[static_cast<int>(kind)]);
	PUSH;
	lhs->codegen();
	rhs->codegen();
	POP;
	PRINT(DEL);
}

void UnaryExprAST::codegen() {
	PRINT("UnaryExprAST");
	PRINT(opMap[static_cast<int>(kind)]);
	PUSH;
	operand->codegen();
	POP;
	PRINT(DEL);
}

void CallExprAST::codegen() {
	PRINT("CallExprAST");
	PUSH;
	func->codegen();
	for (auto&& a : args) {
		a->codegen();
	}
	POP;
	PRINT(DEL);
}

void PointerDerefAST::codegen() {
	PRINT("PointerDerefAST");
	PUSH;
	ptr->codegen();
	index->codegen();
	POP;
	PRINT(DEL);
}

void FunctionDeclAST::codegen() {
	PRINT("FunctionDeclAST");
	PRINT("function name: " + id);
	PRINT(DEL);
}

void BlockAST::codegen() {
	PRINT("BlockAST");
	PUSH;
	for (auto&& a : blockInternal) {
		a->codegen();
	}
	POP;
	PRINT(DEL);
}

void FunctionDefAST::codegen() {
	PRINT("FunctionDefAST");
	PUSH;
	prototype->codegen();
	block->codegen();
	POP;
	PRINT(DEL);
}

void VarDefAST::codegen() {
	PRINT("VarDefAST");
	PRINT("Variable name: " + id);
	PUSH;
	init->codegen();
	POP;
	PRINT(DEL);
}

void IfAST::codegen() {
	PRINT("IfAST");
	PUSH;
	if (cond) {
		cond->codegen();
	}
	if (then) {
		then->codegen();
	}
	if (els) {
		els->codegen();
	}
	POP;
	PRINT(DEL);
}

void ForAST::codegen() {
	PRINT("ForAST");
	PUSH;
	if (cond) {
		cond->codegen();
	}
	then->codegen();
	POP;
	PRINT(DEL);
}

void BreakAST::codegen() {
	PRINT("BreakAST");
	PUSH;
	if (cond) {
		cond->codegen();
	}
	POP;
	PRINT(DEL);
}

void ContinueAST::codegen() {
	PRINT("ContinueAST");
	PUSH;
	if (cond) {
		cond->codegen();
	}
	POP;
	PRINT(DEL);
}

void ReturnAST::codegen() {
	PRINT("ReturnAST");
	PUSH;
	ret->codegen();
	POP;
	PRINT(DEL);
}
