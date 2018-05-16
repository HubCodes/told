#pragma once

#include <tuple>
#include <vector>

#include "lexer.hpp"
#include "llvm/IR/Value.h"

union NumericData {
	char c;
	int i;
	double d;
};

struct Type {
	TypeKind kind;

	/* func() 타입을 위함 */
	Type* returnType;
	std::vector<Type> argsType;

	int ptrDepth; // func kind를 가지고 있다면 1과 같거나 더 크다.
};

class AST {
public:
	virtual ~AST() { }
	virtual llvm::Value* codegen() = 0;
};

class ExprAST : public AST {
public:
	virtual llvm::Value* codegen() = 0;
};

class NumberAST : public ExprAST {
public:
	NumberAST(LiteralKind kind, NumericData numericData) : kind(kind), numericData(numericData) { }
	virtual llvm::Value* codegen();
private:
	LiteralKind kind;
	NumericData numericData;
};

class VariableAST : public ExprAST {
public:
	VariableAST(const std::string& id) : id(id) { }
	virtual llvm::Value* codegen();
private:
	std::string id;
};

class StringAST : public ExprAST {
public:
	StringAST(const std::string& str) : str(str) { }
	virtual llvm::Value* codegen();
private:
	std::string str;
};

class BinaryExprAST : public ExprAST {
public:
	BinaryExprAST(OpKind kind, ExprAST* lhs, ExprAST* rhs) : kind(kind), lhs(lhs), rhs(rhs) { }
	virtual llvm::Value* codegen();
private:
	OpKind kind;
	ExprAST* lhs;
	ExprAST* rhs;
};

class UnaryExprAST : public ExprAST {
public:
	UnaryExprAST(OpKind kind, ExprAST* operand) : kind(kind), operand(operand) { }
	virtual llvm::Value* codegen();
private:
	OpKind kind;
	ExprAST* operand;
};

class CallExprAST : public ExprAST {
public:
	CallExprAST(ExprAST* func, const std::vector<ExprAST*>& args) : func(func), args(args) { }
	virtual llvm::Value* codegen();
private:
	ExprAST* func;
	std::vector<ExprAST*> args;
};

class PointerDerefAST : public ExprAST {
public:
	PointerDerefAST(ExprAST* ptr, ExprAST* index) : ptr(ptr), index(index) { }
	virtual llvm::Value* codegen();
private:
	ExprAST* ptr;
	ExprAST* index;
};

class FunctionDeclAST : public AST {
public:
	FunctionDeclAST(const std::string& id, Type returnType, const std::vector<std::tuple<std::string, Type>>& argsType) : id(id), returnType(returnType), argsType(argsType) { }
	virtual llvm::Value* codegen();
private:
	std::string id;
	Type returnType;
	std::vector<std::tuple<std::string, Type>> argsType;
};

class BlockAST : public AST {
public:
	BlockAST(const std::vector<AST*>& blockInternal) : blockInternal(blockInternal) { }
	virtual llvm::Value* codegen();
private:
	std::vector<AST*> blockInternal;
};

class FunctionDefAST : public AST {
public:
	FunctionDefAST(FunctionDeclAST* prototype, BlockAST* block) : prototype(prototype), block(block) { }
	virtual llvm::Value* codegen();
private:
	FunctionDeclAST* prototype;
	BlockAST* block;
};

class VarDefAST : public AST {
public:
	VarDefAST(Type type, const std::string& id, ExprAST* init) : type(type), id(id), init(init) { }
	virtual llvm::Value* codegen();
private:
	Type type;
	std::string id;
	ExprAST* init;
};

class IfAST : public AST {
public:
	IfAST(ExprAST* cond, BlockAST* then, BlockAST* els) : cond(cond), then(then), els(els) { }
	virtual llvm::Value* codegen();
private:
	ExprAST* cond;
	BlockAST* then;
	BlockAST* els;
};

class ForAST : public AST {
public:
	ForAST(ExprAST* cond, BlockAST* then) : cond(cond), then(then) { }
	virtual llvm::Value* codegen();
private:
	ExprAST* cond;
	BlockAST* then;
};

class BreakAST : public AST {
public:
	BreakAST(ExprAST* cond) : cond(cond) { }
	virtual llvm::Value* codegen();
private:
	ExprAST* cond;
};

class ContinueAST : public AST {
public:
	ContinueAST(ExprAST* cond) : cond(cond) { }
	virtual llvm::Value* codegen();
private:
	ExprAST* cond;
};

class ReturnAST : public AST {
public:
	ReturnAST(ExprAST* ret) : ret(ret) { }
	virtual llvm::Value* codegen();
private:
	ExprAST* ret;
};

std::vector<AST*> parse(std::istringstream& code);
