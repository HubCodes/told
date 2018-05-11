#pragma once

#include <tuple>
#include <vector>

#include "lexer.hpp"

union NumericData {
	char c;
	int i;
	double d;
};

struct Type {
	TypeKind kind;

	/* func() 타입을 위함 */
	Type returnType;
	std::vector<Type> argsType;

	int ptrDepth; // func kind를 가지고 있다면 1과 같거나 더 크다.
};

class AST {
public:
	virtual ~AST();
};

class ExprAST : public AST {

};

class NumberAST : public ExprAST {
public:
	NumberAST(LiteralKind kind, NumericData numericData) : kind(kind), numericData(numericData) { }
private:
	LiteralKind kind;
	NumericData numericData;
};

class VariableAST : public ExprAST {
public:
	VariableAST(const std::string& id) : id(id) { }
private:
	std::string id;
};

class BinaryExprAST : public ExprAST {
public:
	BinaryExprAST(OpKind kind, AST* lhs, AST* rhs) : lhs(lhs), rhs(rhs) { }
private:
	OpKind kind;
	AST* lhs;
	AST* rhs;
};

class UnaryExprAST : public ExprAST {
public:
	UnaryExprAST(OpKind kind, AST* operand) : kind(kind), operand(operand) { }
private:
	OpKind kind;
	AST* operand;
};

class FunctionDeclAST : public AST {
public:
	FunctionDeclAST(Type returnType, const std::vector<std::tuple<std::string, Type>>& argsType) : returnType(returnType), argsType(argsType) { }
private:
	Type returnType;
	std::vector<std::tuple<std::string, Type>> argsType;
};

class BlockAST : public AST {
public:
	BlockAST(const std::vector<AST*>& blockInternal) : blockInternal(blockInternal) { }
private:
	std::vector<AST*> blockInternal;
};

class FunctionDefAST : public AST {
public:
	FunctionDefAST(FunctionDeclAST* prototype, BlockAST* block) : prototype(prototype), block(block) { }
private:
	FunctionDeclAST* prototype;
	BlockAST* block;
};

class VarDefAST : public AST {
public:
	VarDefAST(Type type, const std::string& id, ExprAST* init) : type(type), id(id), init(init) { }
private:
	Type type;
	std::string id;
	ExprAST* init;
};

class IfAST : public AST {
public:
	IfAST(ExprAST* cond, BlockAST* then, BlockAST* els) : cond(cond), then(then), els(els) { }
private:
	ExprAST* cond;
	BlockAST* then;
	BlockAST* els;
};

std::vector<AST*> parse(std::istringstream& code);
