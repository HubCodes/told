#include <iostream>
#include <string>
#include <unordered_map>

#include "lexer.hpp"
#include "parser.hpp"

#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"

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

llvm::LLVMContext context;

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

llvm::Value* NumberAST::codegen() { 
#ifdef DEBUG_
	PRINT("NumericAST");
	PRINT((kind == LiteralKind::CHAR) ? "char data :" + std::to_string(numericData.c) : (kind == LiteralKind::INT) ? "int data: " + std::to_string(numericData.i) : "double data: " + std::to_string(numericData.d));
	PRINT(DEL);
	return nullptr;
#else
	switch (kind) {
		case LiteralKind::CHAR:
			return llvm::ConstantInt::get(getInt8Ty(context), numericData.c);
		case LiteralKind::INT:
			return llvm::ConstantInt::get(getInt32Ty(context), numericData.i);
		case LiteralKind::DOUBLE:
			return llvm::ConstantFP::get(getDoubleTy(context), numericData.d);
	}
#endif
}

llvm::Value* VariableAST::codegen() { 
#ifdef DEBUG_
	PRINT("VariableAST");
	PRINT(id);
	PRINT(DEL);
	return nullptr;
#else
			
#endif
}

llvm::Value* StringAST::codegen() { 
#ifdef DEBUG_
	PRINT("StringAST");
	PRINT(str);
	PRINT(DEL);
	return nullptr;
#else

#endif
}

llvm::Value* BinaryExprAST::codegen() { 
#ifdef DEBUG_
	PRINT("BinaryExprAST");
	PRINT(opMap[static_cast<int>(kind)]);
	PUSH;
	lhs->codegen();
	rhs->codegen();
	POP;
	PRINT(DEL);
	return nullptr;
#else

#endif
}

llvm::Value* UnaryExprAST::codegen() { 
#ifdef DEBUG_
	PRINT("UnaryExprAST");
	PRINT(opMap[static_cast<int>(kind)]);
	PUSH;
	operand->codegen();
	POP;
	PRINT(DEL);
	return nullptr;
#else

#endif
}

llvm::Value* CallExprAST::codegen() { 
#ifdef DEBUG_
	PRINT("CallExprAST");
	PUSH;
	func->codegen();
	for (auto&& a : args) {
		a->codegen();
	}
	POP;
	PRINT(DEL);
	return nullptr;
#else

#endif
}

llvm::Value* PointerDerefAST::codegen() { 
#ifdef DEBUG_
	PRINT("PointerDerefAST");
	PUSH;
	ptr->codegen();
	index->codegen();
	POP;
	PRINT(DEL);
	return nullptr;
#else

#endif
}

llvm::Value* FunctionDeclAST::codegen() { 
#ifdef DEBUG_
	PRINT("FunctionDeclAST");
	PRINT("function name: " + id);
	PRINT(DEL);
	return nullptr;
#else

#endif
}

llvm::Value* BlockAST::codegen() { 
#ifdef DEBUG_
	PRINT("BlockAST");
	PUSH;
	for (auto&& a : blockInternal) {
		a->codegen();
	}
	POP;
	PRINT(DEL);
	return nullptr;
#else

#endif
}

llvm::Value* FunctionDefAST::codegen() { 
#ifdef DEBUG_
	PRINT("FunctionDefAST");
	PUSH;
	prototype->codegen();
	block->codegen();
	POP;
	PRINT(DEL);
	return nullptr;
#else

#endif
}

llvm::Value* VarDefAST::codegen() { 
#ifdef DEBUG_
	PRINT("VarDefAST");
	PRINT("Variable name: " + id);
	PUSH;
	init->codegen();
	POP;
	PRINT(DEL);
	return nullptr;
#else

#endif
}

llvm::Value* IfAST::codegen() { 
#ifdef DEBUG_
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
	return nullptr;
#else

#endif
}

llvm::Value* ForAST::codegen() { 
#ifdef DEBUG_
	PRINT("ForAST");
	PUSH;
	if (cond) {
		cond->codegen();
	}
	then->codegen();
	POP;
	PRINT(DEL);
	return nullptr;
#else

#endif
}

llvm::Value* BreakAST::codegen() { 
#ifdef DEBUG_
	PRINT("BreakAST");
	PUSH;
	if (cond) {
		cond->codegen();
	}
	POP;
	PRINT(DEL);
	return nullptr;
#else

#endif
}

llvm::Value* ContinueAST::codegen() { 
#ifdef DEBUG_
	PRINT("ContinueAST");
	PUSH;
	if (cond) {
		cond->codegen();
	}
	POP;
	PRINT(DEL);
	return nullptr;
#else

#endif
}

llvm::Value* ReturnAST::codegen() { 
#ifdef DEBUG_
	PRINT("ReturnAST");
	PUSH;
	ret->codegen();
	POP;
	PRINT(DEL);
	return nullptr;
#else

#endif
}
