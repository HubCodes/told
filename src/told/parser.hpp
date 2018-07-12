#ifndef __PARSER_HEADER__
#define __PARSER_HEADER__

#include "includes.hpp"
#include "lexer.hpp"

#include "compilerdata.hpp"
#include "compileroption.hpp"

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

class AST {
public:
    virtual ~AST() {}
    virtual void codegen() = 0;
};

class ExprAST : public AST {
public:
    virtual void codegen() = 0;
    virtual bool isIntegral() = 0;
};

class NumberAST : public ExprAST {
public:
    NumberAST(LiteralKind kind, NumericData numericData)
        : kind(kind)
        , numericData(numericData)
    {
    }
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
    VariableAST(const string& id)
        : id(id)
    {
    }
    virtual void codegen();
    virtual bool isIntegral();
    const string& getId() const noexcept { return id; }

private:
    string id;
};

class StringAST : public ExprAST {
public:
    StringAST(const string& str)
        : str(str)
    {
    }
    virtual void codegen();
    virtual bool isIntegral();
    const string& getStr() const noexcept { return str; }

private:
    string str;
};

class BinaryExprAST : public ExprAST {
public:
    BinaryExprAST(OpKind kind, ExprAST* lhs, ExprAST* rhs)
        : kind(kind)
        , lhs(lhs)
        , rhs(rhs)
    {
    }
    virtual void codegen();
    virtual bool isIntegral();

private:
    OpKind kind;
    ExprAST* lhs;
    ExprAST* rhs;
};

class UnaryExprAST : public ExprAST {
public:
    UnaryExprAST(OpKind kind, ExprAST* operand)
        : kind(kind)
        , operand(operand)
    {
    }
    virtual void codegen();
    virtual bool isIntegral();

private:
    OpKind kind;
    ExprAST* operand;
};

class CallExprAST : public ExprAST {
public:
    CallExprAST(ExprAST* func, const vector<ExprAST*>& args)
        : func(func)
        , args(args)
    {
    }
    virtual void codegen();
    virtual bool isIntegral();

private:
    ExprAST* func;
    vector<ExprAST*> args;
};

class PointerDerefAST : public ExprAST {
public:
    PointerDerefAST(ExprAST* ptr, ExprAST* index)
        : ptr(ptr)
        , index(index)
    {
    }
    virtual void codegen();
    virtual bool isIntegral();
    ExprAST* getPtr() noexcept { return ptr; }
    ExprAST* getIndex() noexcept { return index; }
    TypeCode getType() noexcept;
    Type getTypeObject() const noexcept;

private:
    ExprAST* ptr;
    ExprAST* index;
    bool check() const noexcept
    {
        VariableAST* p = dynamic_cast<VariableAST*>(ptr);
        if (p == nullptr) {
            std::cerr << "Pointer dereference must have only one address variable, not complex.\n";
            std::exit(EXIT_FAILURE);
            return false;
        }
        return true;
    }
};

class FunctionDeclAST : public AST {
public:
    FunctionDeclAST(const string& id, Type returnType, const vector<std::tuple<string, Type>>& argsType)
        : id(id)
        , returnType(returnType)
        , argsType(argsType)
    {
    }
    virtual void codegen();
    string getId() const noexcept { return id; }
    Type getReturnType() const noexcept { return returnType; }
    int argsCount() const noexcept { return argsType.size(); }
    std::pair<string, Type> getArg()
    {
        static int index = 0;
        string first = std::get<0>(argsType[index]);
        Type second = std::get<1>(argsType[index]);
        index++;
        return std::make_pair(first, second);
    }
    Type getType() const
    {
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
    VarDefAST(Type type, const string& id, ExprAST* init)
        : type(type)
        , id(id)
        , init(init)
    {
    }
    virtual void codegen();

private:
    Type type;
    string id;
    ExprAST* init;
};

class BlockAST : public AST {
public:
    BlockAST(const vector<AST*>& blockInternal)
        : blockInternal(blockInternal)
    {
    }
    virtual void codegen();
    int getVariableCount()
    {
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
    FunctionDefAST(FunctionDeclAST* prototype, BlockAST* block)
        : prototype(prototype)
        , block(block)
    {
    }
    virtual void codegen();

private:
    FunctionDeclAST* prototype;
    BlockAST* block;
};

class ArrayAST : public ExprAST {
public:
    ArrayAST(Type type, const vector<ExprAST*>& inits)
        : type(type)
        , inits(inits)
    {
    }
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
    IfAST(ExprAST* cond, BlockAST* then, BlockAST* els)
        : cond(cond)
        , then(then)
        , els(els)
    {
    }
    virtual void codegen();

private:
    ExprAST* cond;
    BlockAST* then;
    BlockAST* els;
};

class ForAST : public AST {
public:
    ForAST(ExprAST* cond, BlockAST* then)
        : cond(cond)
        , then(then)
    {
    }
    virtual void codegen();

private:
    ExprAST* cond;
    BlockAST* then;
};

class BreakAST : public AST {
public:
    BreakAST(ExprAST* cond)
        : cond(cond)
    {
    }
    virtual void codegen();

private:
    ExprAST* cond;
};

class ContinueAST : public AST {
public:
    ContinueAST(ExprAST* cond)
        : cond(cond)
    {
    }
    virtual void codegen();

private:
    ExprAST* cond;
};

class ReturnAST : public AST {
public:
    ReturnAST(ExprAST* ret)
        : ret(ret)
    {
    }
    virtual void codegen();

private:
    ExprAST* ret;
};

class ASTData final {
private:
    vector<AST*> ast_vec;
    vector<string> use_vec;

    string filename;
public:
    ASTData(const string& filename);

    void pushAST(AST* const ast);
    void pushUse(const string& use);

    const vector<AST*>& getASTs() const;
    const vector<string>& getUses() const;
    const string& getFilename() const;
};

#define D(x) static_cast<int>(x)

class Parser final {
private:
    unordered_map<int, string> opMap = {
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
    bool isDefinableVar = false;

    LexicalParser lex;
    unordered_map<string, Type> varTypeMap;

    vector<AST*> ast;

private:
    FunctionDefAST* getFunction(istringstream& code);
    VarDefAST* getVardef(istringstream& code);
    BlockAST* getBlock(istringstream& code);
    ExprAST* getExpr(istringstream& code);
    ExprAST* getArray(istringstream& code);
    ExprAST* getAssignExpr(istringstream& code);
    ExprAST* getLogicOrExpr(istringstream& code);
    ExprAST* getLogicAndExpr(istringstream& code);
    ExprAST* getEqualityExpr(istringstream& code);
    ExprAST* getLessBiggerExpr(istringstream& code);
    ExprAST* getAddSubExpr(istringstream& code);
    ExprAST* getMulDivModExpr(istringstream& code);
    ExprAST* getUnaryExpr(istringstream& code);
    ExprAST* getCallDeref(istringstream& code);
    ExprAST* getPrimary(istringstream& code);
    ExprAST* getIdent(istringstream& code);
    ExprAST* getLiteral(istringstream& code);
    Type getType(istringstream& code);
    IfAST* getIf(istringstream& code);
    ForAST* getFor(istringstream& code);
    BreakAST* getBreak(istringstream& code);
    ContinueAST* getContinue(istringstream& code);
    ReturnAST* getReturn(istringstream& code);

    void expected(istringstream& code, const char* expected, const char* msg);
    void wrong(istringstream& code, const char* wron);
    void maybe(istringstream& code, TokenKind kind);
    // void use(const string& module);

    Parser();

    ASTData parse(string& filename, istringstream& code);

    // friend void CodeGenerator::generateCode(Parser& opt);
    // vector<AST*> getAST();

public:
    static vector<ASTData> start_parse(CompilerOption& opt, shared_ptr<CompilerData> cd);

    static TypeCode fromType(Type ty);
    static Type genFuncType(Type returnType, std::initializer_list<Type> argsType);
};

#endif