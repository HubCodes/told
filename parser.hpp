#pragma once

#include <tuple>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "lexer.hpp"

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

enum class TypeCode {
	CHAR,
	INT,
	DOUBLE,
	PTR
};

static TypeCode fromType(Type ty) {
	if (ty.ptrDepth > 1) {
		return TypeCode::PTR;
	} else if (ty.kind == TypeKind::CHAR) {
		return TypeCode::CHAR;
	} else if (ty.kind == TypeKind::INT) {
		return TypeCode::INT;
	} else if (ty.kind == TypeKind::DOUBLE) {
		return TypeCode::DOUBLE;
	}
}

class CodeManager {
public:
	enum Section {
		DATA,
		TEXT,
	};
public:
	virtual ~CodeManager() { }
	CodeManager(const CodeManager& other) = delete;
	CodeManager& operator=(const CodeManager& other) = delete;
	CodeManager(CodeManager&& other) = delete;
	CodeManager& operator=(CodeManager&& other);

	static CodeManager& get() {
		static CodeManager manager;
		return manager;
	}

	void clear() {
		data.clear();
		text.clear();
	}

	void getNextLabel() {
		int counter = std::stoi(label.substr(5, label.size() - 5));
		label = label.substr(0, 5) + std::to_string(counter + 1);
	}
	
	std::string getLabel() {
		return label;
	}

	void push(Section section, std::string code, bool isIndent = false) {
		if (isIndent) {
			code = "\t" + code;
		}
		switch (section) {
			case DATA:
				data.push_back(code);
				break;
			case TEXT:
				text.push_back(code);
				break;
		}
	}

	void setNowFunction(const std::string& id) {
		nowFunction = id;
	}

	void setNowFunction() {
		nowFunction.clear();
	}

	const std::string& getNowFunction() {
		return nowFunction;
	}

	void insertGlobalVar(const std::string& id, Type ty) {
		globalVars[id] = ty;
	}

	bool isGlobalVar(const std::string& id) {
		return globalVars.find(id) != globalVars.end();
	}

	bool isLocalVar(const std::string& funcId, const std::string& varId) {
		if (funcId.empty()) return false;
		const std::vector<std::tuple<std::string, int, Type>>& local = localVars[funcId];
		for (int i = 0; i < local.size(); ++i) {
			if (std::get<0>(local[i]) == varId) 
				return true;
		}
		return false;
	}

	void insertLocalVar(const std::string& funcId, const std::string& varId, Type ty) {
		if (nowOffset.find(funcId) == nowOffset.end()) {
			nowOffset[funcId] = 8;
		}
		localVars[funcId].push_back(std::make_tuple(varId, nowOffset[funcId], ty));
		nowOffset[funcId] += 8;
	}

	int getLocalVarOffset(const std::string& funcId, const std::string& varId) {
		const std::vector<std::tuple<std::string, int, Type>>& local = localVars[funcId];
		for (int i = 0; i < local.size(); ++i) {
			if (std::get<0>(local[i]) == varId) {
				return std::get<1>(local[i]);
			}
		}
		std::cerr << "Cannot fount variable " << varId << '\n';
		std::exit(EXIT_FAILURE);
	}

	TypeCode getTypeCode(const std::string& varId, bool isLocal = false, const std::string& funcId = "") {
		if (isLocal) {
			if (localVars.find(funcId) != localVars.end()) {
				const std::vector<std::tuple<std::string, int, Type>>& local = localVars[funcId];
				for (int i = 0; i < local.size(); ++i) {
					if (std::get<0>(local[i]) == varId) {
						return fromType(std::get<2>(local[i]));	
					}
				}
				std::cerr << "Cannot found variable " << varId << " on function " << funcId << '\n';
				std::exit(EXIT_FAILURE);
			} else {
				std::cerr << "Cannot found function " << funcId << '\n';
				std::exit(EXIT_FAILURE);
			}
		} else {
			if (globalVars.find(varId) != globalVars.end()) {
				return fromType(globalVars[funcId]);
			} else {
				std::cerr << "Cannot found variable on global namespace :" << varId << '\n';
				std::exit(EXIT_FAILURE);
		}
	}

	bool isVarIntegral(const std::string& varId, bool isLocal = false, const std::string& funcId = "") {
		TypeCode tc = getTypeCode(varId, isLocal, funcId)		
		return tc != TypeCode::DOUBLE;
	}

	}

	void insertFunction(const std::string& funcId) {
		functions.insert(funcId);
	}

	bool isFunction(const std::string& funcId) {
		return functions.find(funcId) != functions.end();
	}

private:
	CodeManager() : data(), text(), label(".temp0"), nowFunction(), localVars(), globalVars(), nowOffset(), functions() { }
	std::vector<std::string> data;
	std::vector<std::string> text;
	std::string label;
	std::string nowFunction;
	std::unordered_map<std::string, std::vector<std::tuple<std::string, int, Type>>> localVars;
	std::unordered_map<std::string, Type> globalVars;
	std::unordered_map<std::string, int> nowOffset;
	std::unordered_set<std::string> functions;
};

class AST {
public:
	virtual ~AST() { }
	virtual void codegen() = 0;
};

class ExprAST : public AST {
public:
	virtual void codegen() = 0;
	virtual bool isIntegral() = 0;
};

class NumberAST : public ExprAST {
public:
	NumberAST(LiteralKind kind, NumericData numericData) : kind(kind), numericData(numericData) { }
	virtual void codegen();
	virtual bool isIntegral();
	NumericData getData() const noexcept { return numericData; }
	LiteralKind getKind() const noexcept { return kind; }
private:
	LiteralKind kind;
	NumericData numericData;
};

class VariableAST : public ExprAST {
public:
	VariableAST(const std::string& id) : id(id) { }
	virtual void codegen();
	virtual bool isIntegral();
	const std::string& getId() const noexcept { return id; }
private:
	std::string id;
};

class StringAST : public ExprAST {
public:
	StringAST(const std::string& str) : str(str) { }
	virtual void codegen();
	virtual bool isIntegral();
	const std::string& getStr() const noexcept { return str; }
private:
	std::string str;
};

class BinaryExprAST : public ExprAST {
public:
	BinaryExprAST(OpKind kind, ExprAST* lhs, ExprAST* rhs) : kind(kind), lhs(lhs), rhs(rhs) { }
	virtual void codegen();
	virtual bool isIntegral();
private:
	OpKind kind;
	ExprAST* lhs;
	ExprAST* rhs;
};

class UnaryExprAST : public ExprAST {
public:
	UnaryExprAST(OpKind kind, ExprAST* operand) : kind(kind), operand(operand) { }
	virtual void codegen();
	virtual bool isIntegral();
private:
	OpKind kind;
	ExprAST* operand;
};

class CallExprAST : public ExprAST {
public:
	CallExprAST(ExprAST* func, const std::vector<ExprAST*>& args) : func(func), args(args) { }
	virtual void codegen();
	virtual bool isIntegral();
private:
	ExprAST* func;
	std::vector<ExprAST*> args;
};

class PointerDerefAST : public ExprAST {
public:
	PointerDerefAST(ExprAST* ptr, ExprAST* index) : ptr(ptr), index(index) { }
	virtual void codegen();
	virtual bool isIntegral();
	ExprAST* getPtr() noexcept { return ptr; }
	ExprAST* getIndex() noexcept { return index; }
	TypeCode getType() noexcept {
		VariableAST* p = dynamic_cast<VariableAST*>(ptr);
		if (p == nullptr) {
			std::cerr << "Pointer dereference must have only one address variable, not complex.\n";
			std::exit(EXIT_FAILURE);
		}
		CodeManager& m = CodeManager::get();
		bool isLocal = m.isLocalVar(p->getId());
		return isLocal ?
			fromType(CodeManager::get().getTypeCode(p->getId(), true, m.getNowFunction())) :
			fromType(CodeManager::get().getTypeCode(p->getId()));
	}
private:
	ExprAST* ptr;
	ExprAST* index;
};

class FunctionDeclAST : public AST {
public:
	FunctionDeclAST(const std::string& id, Type returnType, const std::vector<std::tuple<std::string, Type>>& argsType) : id(id), returnType(returnType), argsType(argsType) { }
	virtual void codegen();
	std::string getId() const noexcept { return id; }
	Type getReturnType() const noexcept { return returnType; }
	int argsCount() const noexcept { return argsType.size(); }
private:
	std::string id;
	Type returnType;
	std::vector<std::tuple<std::string, Type>> argsType;
};

class VarDefAST : public AST {
public:
	VarDefAST(Type type, const std::string& id, ExprAST* init) : type(type), id(id), init(init) { }
	virtual void codegen();
private:
	Type type;
	std::string id;
	ExprAST* init;
};

class BlockAST : public AST {
public:
	BlockAST(const std::vector<AST*>& blockInternal) : blockInternal(blockInternal) { }
	virtual void codegen();
	int getVariableCount() {
		int count = 0;
		for (int i = 0; i < blockInternal.size(); ++i) {
			if (VarDefAST* vardef = dynamic_cast<VarDefAST*>(blockInternal[i])) {
				count++;	
			}
		}
		return count;
	}
private:
	std::vector<AST*> blockInternal;
};

class FunctionDefAST : public AST {
public:
	FunctionDefAST(FunctionDeclAST* prototype, BlockAST* block) : prototype(prototype), block(block) { }
	virtual void codegen();
private:
	FunctionDeclAST* prototype;
	BlockAST* block;
};

class ArrayAST : public ExprAST {
public:
	ArrayAST(Type type, const std::vector<ExprAST*>& inits) : type(type), inits(inits) { }
	virtual void codegen();
	virtual bool isIntegral();
	const std::vector<ExprAST*>& getInits() const noexcept { return inits; }
	Type getType() const noexcept { return type; }
private:
	Type type;
	std::vector<ExprAST*> inits;
};

class IfAST : public AST {
public:
	IfAST(ExprAST* cond, BlockAST* then, BlockAST* els) : cond(cond), then(then), els(els) { }
	virtual void codegen();
private:
	ExprAST* cond;
	BlockAST* then;
	BlockAST* els;
};

class ForAST : public AST {
public:
	ForAST(ExprAST* cond, BlockAST* then) : cond(cond), then(then) { }
	virtual void codegen();
private:
	ExprAST* cond;
	BlockAST* then;
};

class BreakAST : public AST {
public:
	BreakAST(ExprAST* cond) : cond(cond) { }
	virtual void codegen();
private:
	ExprAST* cond;
};

class ContinueAST : public AST {
public:
	ContinueAST(ExprAST* cond) : cond(cond) { }
	virtual void codegen();
private:
	ExprAST* cond;
};

class ReturnAST : public AST {
public:
	ReturnAST(ExprAST* ret) : ret(ret) { }
	virtual void codegen();
private:
	ExprAST* ret;
};

std::vector<AST*> parse(std::istringstream& code);
