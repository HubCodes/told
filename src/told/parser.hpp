#ifndef __PARSER_HEADER__
#define __PARSER_HEADER__

#include "includes.hpp"
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
	vector<Type> argsType;

	int ptrDepth; // func kind를 가지고 있다면 1과 같거나 더 크다.
};
/*

*/		
enum class TypeCode {
	CHAR,
	INT,
	DOUBLE,
	PTR
};

#define D(x) static_cast<int>(x)

static unordered_map<int, string> opMap = {
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


static TypeCode fromType(Type ty) {
	if (ty.ptrDepth > 0) {
		return TypeCode::PTR;
	} else if (ty.kind == TypeKind::CHAR) {
		return TypeCode::CHAR;
	} else if (ty.kind == TypeKind::INT) {
		return TypeCode::INT;
	} else if (ty.kind == TypeKind::DOUBLE) {
		return TypeCode::DOUBLE;
	} else {
		return TypeCode::INT;
	}
}

static Type genFuncType(Type returnType, std::initializer_list<Type> argsType) {
	Type t;
	t.kind = TypeKind::FUNC;
	t.ptrDepth = 1;
	t.returnType = new Type(returnType);
	t.argsType = argsType;
	return t;
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

	void clear();

	void getNextLabel();
	
	string getLabel();

	void push(Section section, string code, bool isIndent = false);

	void setNowFunction(const string& id);

	void setNowFunction();

	const string& getNowFunction();

	void insertGlobalVar(const string& id, Type ty);

	bool isGlobalVar(const string& id);

	bool isLocalVar(const string& funcId, const string& varId);

	void insertLocalVar(const string& funcId, const string& varId, Type ty);

	int getLocalVarOffset(const string& funcId, const string& varId);

	Type getTypeObject(const string& varId, bool isLocal = false, const string& funcId = "");

	TypeCode getTypeCode(const string& varId, bool isLocal = false, const string& funcId = "");

	bool isVarIntegral(const string& varId, bool isLocal = false, const string& funcId = "");

	void insertFunction(const string& funcId, Type t) ;

	bool isFunction(const string& funcId) ;

	const vector<string>& getDataSeg();
	
	const vector<string>& getTextSeg();

private:
	CodeManager() : data(), text(), label(".temp0"), nowFunction(), localVars(), globalVars(), nowOffset(), functions() { }
	vector<string> data;
	vector<string> text;
	string label;
	string nowFunction;
	unordered_map<string, vector<std::tuple<string, int, Type>>> localVars;
	unordered_map<string, Type> globalVars;
	unordered_map<string, int> nowOffset;
	unordered_map<string, Type> functions;
};

static CodeManager& manager = CodeManager::get();

class Stdlib final {
public:
	~Stdlib() { }	
	Stdlib(const Stdlib& other) = delete;
	Stdlib& operator=(const Stdlib& other) = delete;
	static Stdlib& get() {
		static Stdlib lib;
		return lib;
	}
	const vector<std::pair<string, Type>>& getFuncs(const string& module) {
		if (libs.find(module) == libs.end()) {
			std::cerr << "Cannot found module " << module << '\n';
			std::exit(EXIT_FAILURE);
		}
		return libs[module];
	}
private:
	Stdlib() : libs() {
		libs["fio"].push_back({ "opend", genFuncType	({ TypeKind::INT, .ptrDepth=0 }, { { TypeKind::CHAR, .ptrDepth=1 }, { TypeKind::INT, .ptrDepth=0 }, { TypeKind::INT, .ptrDepth=0 } }) });
		libs["fio"].push_back({ "closed", genFuncType	({ TypeKind::INT, .ptrDepth=0 }, { { TypeKind::INT, .ptrDepth=0 } }) });
		libs["fio"].push_back({ "readd", genFuncType	({ TypeKind::INT, .ptrDepth=0 }, { { TypeKind::INT, .ptrDepth=0 }, { TypeKind::CHAR, .ptrDepth=1 }, { TypeKind::INT, .ptrDepth=0 } }) });
		libs["fio"].push_back({ "writed", genFuncType	({ TypeKind::INT, .ptrDepth=0 }, { { TypeKind::INT, .ptrDepth=0 }, { TypeKind::CHAR, .ptrDepth=1 }, { TypeKind::INT, .ptrDepth=0 } }) });
		libs["fio"].push_back({ "getstr", genFuncType	({ TypeKind::INT, .ptrDepth=0 }, { { TypeKind::CHAR, .ptrDepth=1 }, { TypeKind::INT, .ptrDepth=0 } }) });
		libs["fio"].push_back({ "putstr", genFuncType	({ TypeKind::INT, .ptrDepth=0 }, { { TypeKind::CHAR, .ptrDepth=1 }, { TypeKind::INT, .ptrDepth=0 } }) });
		libs["fio"].push_back({ "putc", genFuncType		({ TypeKind::INT, .ptrDepth=0 }, { { TypeKind::INT, .ptrDepth=0 } }) });

		libs["memory"].push_back({ "balloc", genFuncType({ TypeKind::CHAR, .ptrDepth=1 }, { { TypeKind::INT, .ptrDepth=0 } }) });
		libs["memory"].push_back({ "bfree", genFuncType({ TypeKind::INT, .ptrDepth=0 }, { { TypeKind::CHAR, .ptrDepth=1 }, { TypeKind::INT, .ptrDepth=0 } }) });

		libs["string"].push_back({ "strsize", genFuncType({ TypeKind::CHAR, .ptrDepth=1 }, { { TypeKind::INT, .ptrDepth=0 } }) });
	}
	unordered_map<string, vector<std::pair<string, Type>>> libs;
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
	VariableAST(const string& id) : id(id) { }
	virtual void codegen();
	virtual bool isIntegral();
	const string& getId() const noexcept { return id; }
private:
	string id;
};

class StringAST : public ExprAST {
public:
	StringAST(const string& str) : str(str) { }
	virtual void codegen();
	virtual bool isIntegral();
	const string& getStr() const noexcept { return str; }
private:
	string str;
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
	CallExprAST(ExprAST* func, const vector<ExprAST*>& args) : func(func), args(args) { }
	virtual void codegen();
	virtual bool isIntegral();
private:
	ExprAST* func;
	vector<ExprAST*> args;
};

class PointerDerefAST : public ExprAST {
public:
	PointerDerefAST(ExprAST* ptr, ExprAST* index) : ptr(ptr), index(index) { }
	virtual void codegen();
	virtual bool isIntegral();
	ExprAST* getPtr() noexcept { return ptr; }
	ExprAST* getIndex() noexcept { return index; }
	TypeCode getType() noexcept {
		CodeManager& m = check();
		VariableAST* p = dynamic_cast<VariableAST*>(ptr);
		bool isLocal = !m.getNowFunction().empty();
		return isLocal ?
			CodeManager::get().getTypeCode(p->getId(), true, m.getNowFunction()) :
			CodeManager::get().getTypeCode(p->getId());
	}
	Type getTypeObject() const noexcept {
		CodeManager& m = check();
		VariableAST* p = dynamic_cast<VariableAST*>(ptr);
		bool isLocal = !m.getNowFunction().empty();
		Type t = isLocal ? m.getTypeObject(p->getId(), true, m.getNowFunction()) :
							m.getTypeObject(p->getId());
		return t;
	}
private:
	ExprAST* ptr;
	ExprAST* index;
	CodeManager& check() const noexcept {
		VariableAST* p = dynamic_cast<VariableAST*>(ptr);
		if (p == nullptr) {
			std::cerr << "Pointer dereference must have only one address variable, not complex.\n";
			std::exit(EXIT_FAILURE);
		}
		CodeManager& m = CodeManager::get();
		return m;
	}
};

class FunctionDeclAST : public AST {
public:
	FunctionDeclAST(const string& id, Type returnType, const vector<std::tuple<string, Type>>& argsType) : id(id), returnType(returnType), argsType(argsType) { }
	virtual void codegen();
	string getId() const noexcept { return id; }
	Type getReturnType() const noexcept { return returnType; }
	int argsCount() const noexcept { return argsType.size(); }
	std::pair<string, Type> getArg() {
		static int index = 0;
		string first = std::get<0>(argsType[index]);
		Type second = std::get<1>(argsType[index]);
		index++;
		return std::make_pair(first, second);
	}
	Type getType() const {
		Type t;
		t.kind = TypeKind::FUNC;
		t.ptrDepth = 1;
		t.returnType = new Type(returnType);
		vector<Type> argsTypeIn;
		std::for_each(argsType.begin(), argsType.end(), [&argsTypeIn](const std::tuple<string, Type>& type) {
			argsTypeIn.push_back(std::get<1>(type));
		});
		t.argsType = std::move(argsTypeIn);
		return t;
	}
private:
	string id;
	Type returnType;
	vector<std::tuple<string, Type>> argsType;
};

class VarDefAST : public AST {
public:
	VarDefAST(Type type, const string& id, ExprAST* init) : type(type), id(id), init(init) { }
	virtual void codegen();
private:
	Type type;
	string id;
	ExprAST* init;
};

class BlockAST : public AST {
public:
	BlockAST(const vector<AST*>& blockInternal) : blockInternal(blockInternal) { }
	virtual void codegen();
	int getVariableCount() {
		int count = 0;
		for (int i = 0; i < blockInternal.size(); ++i) {
			if (VarDefAST* vardef = dynamic_cast<VarDefAST*>(blockInternal[i])) {
				count++;	
			} else {
				break;
			}
		}
		return count;
	}
private:
	vector<AST*> blockInternal;
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
	ArrayAST(Type type, const vector<ExprAST*>& inits) : type(type), inits(inits) { }
	virtual void codegen();
	virtual bool isIntegral();
	const vector<ExprAST*>& getInits() const noexcept { return inits; }
	Type getType() const noexcept { return type; }
private:
	Type type;
	vector<ExprAST*> inits;
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

vector<AST*> parse(istringstream& code);

#endif