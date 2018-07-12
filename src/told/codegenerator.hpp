#ifndef __CODE_GENERATOR_HEADER__
#define __CODE_GENERATOR_HEADER__

#include "compileroption.hpp"
#include "includes.hpp"
#include "parser.hpp"

class CodeManager final {
public:
    enum Section {
        DATA,
        TEXT,
    };

public:
    static CodeManager& create()
    {
        return *new CodeManager();
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

    void insertFunction(const string& funcId, Type t);

    bool isFunction(const string& funcId);

    const vector<string>& getDataSeg();

    const vector<string>& getTextSeg();

private:
    CodeManager()
        : data()
        , text()
        , label(".temp0")
        , nowFunction()
        , localVars()
        , globalVars()
        , nowOffset()
        , functions()
    {
    }
    vector<string> data;
    vector<string> text;
    string label;
    string nowFunction;
    unordered_map<string, vector<std::tuple<string, int, Type>>> localVars;
    unordered_map<string, Type> globalVars;
    unordered_map<string, int> nowOffset;
    unordered_map<string, Type> functions;
};

class CodeGenerator final {
private:
    CompilerOption& opt;

private:
    void writeCode(std::ostream& os, CodeManager& manager);
    const vector<std::pair<string, Type>>& getPairs(vector<string> uses);
    void assemble(const string& filename);

public:
    CodeGenerator(CompilerOption& opt);
    void generateCode(vector<ASTData>& opt);
};

// class Stdlib final {
// public:
//     ~Stdlib() {}
//     Stdlib(const Stdlib& other) = delete;
//     Stdlib& operator=(const Stdlib& other) = delete;
//     static Stdlib& get()
//     {
//         static Stdlib lib;
//         return lib;
//     }
//     const vector<std::pair<string, Type>>& getFuncs(const string& module)
//     {
//         if (libs.find(module) == libs.end()) {
//             std::cerr << "Cannot found module " << module << '\n';
//             std::exit(EXIT_FAILURE);
//         }
//         return libs[module];
//     }

// private:
//     Stdlib()
//         : libs()
//     {
//         libs["fio"].push_back({ "opend", Parser::genFuncType({ TypeKind::INT, .ptrDepth = 0 }, { { TypeKind::CHAR, .ptrDepth = 1 }, { TypeKind::INT, .ptrDepth = 0 }, { TypeKind::INT, .ptrDepth = 0 } }) });
//         libs["fio"].push_back({ "closed", Parser::genFuncType({ TypeKind::INT, .ptrDepth = 0 }, { { TypeKind::INT, .ptrDepth = 0 } }) });
//         libs["fio"].push_back({ "readd", Parser::genFuncType({ TypeKind::INT, .ptrDepth = 0 }, { { TypeKind::INT, .ptrDepth = 0 }, { TypeKind::CHAR, .ptrDepth = 1 }, { TypeKind::INT, .ptrDepth = 0 } }) });
//         libs["fio"].push_back({ "writed", Parser::genFuncType({ TypeKind::INT, .ptrDepth = 0 }, { { TypeKind::INT, .ptrDepth = 0 }, { TypeKind::CHAR, .ptrDepth = 1 }, { TypeKind::INT, .ptrDepth = 0 } }) });
//         libs["fio"].push_back({ "getstr", Parser::genFuncType({ TypeKind::INT, .ptrDepth = 0 }, { { TypeKind::CHAR, .ptrDepth = 1 }, { TypeKind::INT, .ptrDepth = 0 } }) });
//         libs["fio"].push_back({ "putstr", Parser::genFuncType({ TypeKind::INT, .ptrDepth = 0 }, { { TypeKind::CHAR, .ptrDepth = 1 }, { TypeKind::INT, .ptrDepth = 0 } }) });
//         libs["fio"].push_back({ "putc", Parser::genFuncType({ TypeKind::INT, .ptrDepth = 0 }, { { TypeKind::INT, .ptrDepth = 0 } }) });

//         libs["memory"].push_back({ "balloc", Parser::genFuncType({ TypeKind::CHAR, .ptrDepth = 1 }, { { TypeKind::INT, .ptrDepth = 0 } }) });
//         libs["memory"].push_back({ "bfree", Parser::genFuncType({ TypeKind::INT, .ptrDepth = 0 }, { { TypeKind::CHAR, .ptrDepth = 1 }, { TypeKind::INT, .ptrDepth = 0 } }) });

//         libs["string"].push_back({ "strsize", Parser::genFuncType({ TypeKind::CHAR, .ptrDepth = 1 }, { { TypeKind::INT, .ptrDepth = 0 } }) });
//     }
//     unordered_map<string, vector<std::pair<string, Type>>> libs;
// };

#endif