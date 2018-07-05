#include "parser.hpp"
#include "includes.hpp"
#include "lexer.hpp"

#define DEBUG_
#undef DEBUG_

static void logger(const char* msg)
{
#ifdef DEBUG_
    std::cerr << msg << '\n';
#endif
}

static void pos(istringstream& code)
{
    std::cerr << code.tellg() << '\n';
}

#ifdef DEBUG_
#define l(x) logger(x)
#else
#define l(x)
#endif

static unordered_map<string, Type> varTypeMap;

static FunctionDefAST* getFunction(istringstream& code);
static VarDefAST* getVardef(istringstream& code);
static BlockAST* getBlock(istringstream& code);
static ExprAST* getExpr(istringstream& code);
static ExprAST* getArray(istringstream& code);
static ExprAST* getAssignExpr(istringstream& code);
static ExprAST* getLogicOrExpr(istringstream& code);
static ExprAST* getLogicAndExpr(istringstream& code);
static ExprAST* getEqualityExpr(istringstream& code);
static ExprAST* getLessBiggerExpr(istringstream& code);
static ExprAST* getAddSubExpr(istringstream& code);
static ExprAST* getMulDivModExpr(istringstream& code);
static ExprAST* getUnaryExpr(istringstream& code);
static ExprAST* getCallDeref(istringstream& code);
static ExprAST* getPrimary(istringstream& code);
static ExprAST* getIdent(istringstream& code);
static ExprAST* getLiteral(istringstream& code);
static Type getType(istringstream& code);
static IfAST* getIf(istringstream& code);
static ForAST* getFor(istringstream& code);
static BreakAST* getBreak(istringstream& code);
static ContinueAST* getContinue(istringstream& code);
static ReturnAST* getReturn(istringstream& code);

static void expected(istringstream& code, const char* expected, const char* msg);
static void wrong(istringstream& code, const char* wron);
static void maybe(istringstream& code, TokenKind kind);
static void use(const string& module);

static void expected(istringstream& code, const char* expected, const char* msg)
{
    code.unget();
    std::cout << "Position: " << code.tellg() << '\n';
    std::cout << "Data: " << (char)code.peek() << '\n';
    std::cerr << "Expected " << expected << ", but " << msg << '\n';
    std::exit(EXIT_FAILURE);
}

static void wrong(istringstream& code, const char* wron)
{
    code.unget();
    std::cout << "Position: " << code.tellg() << '\n';
    std::cout << "Data: " << (char)code.peek() << '\n';
    std::cerr << "Wrong " << wron << '\n';
    std::exit(EXIT_FAILURE);
}

static void maybe(istringstream& code, TokenKind kind)
{
    if (getNext(code).tokenKind != kind) {
        code.unget();
        std::cout << "Position: " << code.tellg() << '\n';
        std::cout << "Data: " << (char)code.peek() << '\n';
        std::cerr << "Wrong kind of token.\n";
        std::exit(EXIT_FAILURE);
    }
}

vector<AST*> parse(istringstream& code)
{
    l(__FUNCTION__);
    vector<AST*> result;
    Token token = getNext(code);
    while (token.tokenKind != TokenKind::UNKNOWN) {
        if (token.tokenKind == TokenKind::KEYWORD && token.keywordKind == KeywordKind::DEF) {
            result.push_back(getFunction(code));
        } else if (token.tokenKind == TokenKind::KEYWORD && token.keywordKind == KeywordKind::VAR) {
            result.push_back(getVardef(code));
        } else if (token.tokenKind == TokenKind::KEYWORD && token.keywordKind == KeywordKind::USE) {
            Token usingg = getNext(code);
            use(usingg.ident);
            maybe(code, TokenKind::SEMICOLON);
        } else {
            expected(code, "toplevel definitions", "unknown");
        }
        token = getNext(code);
    }
    return result;
}

static void use(const string& module)
{
    CodeManager& m = CodeManager::get();
    Stdlib& s = Stdlib::get();
    const vector<std::pair<string, Type>>& vec = s.getFuncs(module);
    std::for_each(vec.begin(), vec.end(), [&m](const std::pair<string, Type>& funcId) {
        m.insertFunction(funcId.first, funcId.second);
    });
}

static bool isDefinableVar = false;
static FunctionDefAST* getFunction(istringstream& code)
{
    l(__FUNCTION__);
    Token functionId = getNext(code);
    if (functionId.tokenKind != TokenKind::IDENT) {
        wrong(code, "identifier");
    }
    maybe(code, TokenKind::OPEN_PAREN);
    Token lookahead = getNext(code);
    vector<std::tuple<string, Type>> args;
    if (lookahead.tokenKind != TokenKind::CLOSE_PAREN) {
        while (true) {
            // Token id = getNext(code);
            maybe(code, TokenKind::COLON);
            Type type = getType(code);
            if (lookahead.tokenKind != TokenKind::IDENT) {
                expected(code, "identifier", "unknown");
            }
            args.emplace_back(lookahead.ident, type);
            lookahead = getNext(code);
            if (lookahead.tokenKind == TokenKind::CLOSE_PAREN) {
                break;
            } else if (lookahead.tokenKind == TokenKind::COMMA) {
                lookahead = getNext(code);
            } else {
                wrong(code, "delimiter in prototype");
            }
        }
    }
    maybe(code, TokenKind::COLON);
    Type returnType = getType(code);
    auto&& proto = new FunctionDeclAST(functionId.ident, returnType, args);
    isDefinableVar = true;
    auto&& block = getBlock(code);
    isDefinableVar = false;
    return new FunctionDefAST(proto, block);
}

static VarDefAST* getVardef(istringstream& code)
{
    l(__FUNCTION__);
    Token identTok = getNext(code);
    if (identTok.tokenKind != TokenKind::IDENT) {
        expected(code, "identifier", "unknown");
    }
    string id = identTok.ident;
    maybe(code, TokenKind::COLON);
    Type type = getType(code);
    Token lookahead = getNext(code);
    varTypeMap[id] = type;
    if (lookahead.tokenKind == TokenKind::SEMICOLON) {
        return new VarDefAST(type, id, nullptr);
    } else if (lookahead.tokenKind == TokenKind::OPERATOR && lookahead.opKind == OpKind::ASSIGN) {
        VarDefAST* result = new VarDefAST(type, id, getExpr(code));
        maybe(code, TokenKind::SEMICOLON);
        return result;
    }

    wrong(code, "token");
    return nullptr;
}

static BlockAST* getBlock(istringstream& code)
{
    vector<AST*> block;
    maybe(code, TokenKind::OPEN_BLOCK);
    Token lookahead = getNext(code);
    while (lookahead.tokenKind != TokenKind::CLOSE_BLOCK) {
        l(__FUNCTION__);
        if (lookahead.tokenKind == TokenKind::KEYWORD) {
            switch (lookahead.keywordKind) {
            case KeywordKind::VAR:
                if (isDefinableVar)
                    block.push_back(getVardef(code));
                else
                    expected(code, "none of var definition", "var definition");
                break;
            case KeywordKind::IF:
                isDefinableVar = false;
                block.push_back(getIf(code));
                break;
            case KeywordKind::FOR:
                isDefinableVar = false;
                block.push_back(getFor(code));
                break;
            case KeywordKind::BREAK:
                isDefinableVar = false;
                block.push_back(getBreak(code));
                break;
            case KeywordKind::CONTINUE:
                isDefinableVar = false;
                block.push_back(getContinue(code));
                break;
            case KeywordKind::RETURN:
                isDefinableVar = false;
                block.push_back(getReturn(code));
                break;
            default:
                expected(code, "expression or statement", "def or else");
            }
        } else {
            if (lookahead.tokenKind == TokenKind::CLOSE_BLOCK) {
                break;
            }
            unget(code);
            block.push_back(getExpr(code));
            maybe(code, TokenKind::SEMICOLON);
        }
        lookahead = getNext(code);
        l("\n");
    }
    return new BlockAST(block);
}

static IfAST* getIf(istringstream& code)
{
    l(__FUNCTION__);
    maybe(code, TokenKind::OPEN_PAREN);
    ExprAST* cond = getExpr(code);
    maybe(code, TokenKind::CLOSE_PAREN);
    BlockAST* then = getBlock(code);
    Token lookahead = getNext(code);

    // else문이 있을 경우
    if (lookahead.tokenKind == TokenKind::KEYWORD && lookahead.keywordKind == KeywordKind::ELSE) {
        Token isIf = getNext(code);

        // 1. else if일 경우
        if (isIf.tokenKind == TokenKind::KEYWORD & isIf.keywordKind == KeywordKind::IF) {
            vector<AST*> elifblock;
            elifblock.push_back(getIf(code));
            return new IfAST(cond, then, new BlockAST(elifblock));
        }

        // 2. else일 경우
        else if (isIf.tokenKind == TokenKind::OPEN_BLOCK) {
            unget(code);
            return new IfAST(cond, then, getBlock(code));
        }

        // else문이 없을 경우
    }

    unget(code);
    return new IfAST(cond, then, nullptr);
}

static ForAST* getFor(istringstream& code)
{
    l(__FUNCTION__);
    maybe(code, TokenKind::OPEN_PAREN);
    ExprAST* cond = getExpr(code);
    maybe(code, TokenKind::CLOSE_PAREN);
    BlockAST* then = getBlock(code);
    return new ForAST(cond, then);
}

static BreakAST* getBreak(istringstream& code)
{
    l(__FUNCTION__);
    Token lookahead = getNext(code);
    if (lookahead.tokenKind == TokenKind::OPEN_PAREN) {
        ExprAST* cond = getExpr(code);
        maybe(code, TokenKind::CLOSE_PAREN);
        maybe(code, TokenKind::SEMICOLON);
        return new BreakAST(cond);
    } else {
        unget(code);
        maybe(code, TokenKind::SEMICOLON);
        return new BreakAST(nullptr);
    }
}

static ContinueAST* getContinue(istringstream& code)
{
    l(__FUNCTION__);
    Token lookahead = getNext(code);
    if (lookahead.tokenKind == TokenKind::OPEN_PAREN) {
        ExprAST* cond = getExpr(code);
        maybe(code, TokenKind::CLOSE_PAREN);
        maybe(code, TokenKind::SEMICOLON);
        return new ContinueAST(cond);
    } else {
        unget(code);
        maybe(code, TokenKind::SEMICOLON);
        return new ContinueAST(nullptr);
    }
}

static ReturnAST* getReturn(istringstream& code)
{
    l(__FUNCTION__);
    ExprAST* ret = getExpr(code);
    maybe(code, TokenKind::SEMICOLON);
    return new ReturnAST(ret);
}

static ExprAST* getExpr(istringstream& code)
{
    l(__FUNCTION__);
    Token t = getNext(code);
    if (t.tokenKind == TokenKind::OPEN_ARR) {
        return getArray(code);
    }
    unget(code);
    return getAssignExpr(code);
}

static ExprAST* getArray(istringstream& code)
{
    Type t = getType(code);
    maybe(code, TokenKind::CLOSE_ARR);
    maybe(code, TokenKind::OPEN_BLOCK);
    Token next = getNext(code);
    if (next.tokenKind == TokenKind::CLOSE_BLOCK) {
        return new ArrayAST(t, vector<ExprAST*>());
    }
    unget(code);
    vector<ExprAST*> inits;
    do {
        inits.push_back(getAssignExpr(code));
        next = getNext(code);
    } while (next.tokenKind == TokenKind::COMMA);
    if (next.tokenKind != TokenKind::CLOSE_BLOCK) {
        expected(code, "close block symbol }", "unexpected token");
    }
    return new ArrayAST(t, inits);
}

static ExprAST* getAssignExpr(istringstream& code)
{
    l(__FUNCTION__);
    ExprAST* lhs = getLogicOrExpr(code);
    Token t = getNext(code);
    while (t.tokenKind == TokenKind::OPERATOR && t.opKind == OpKind::ASSIGN) {
        lhs = new BinaryExprAST(OpKind::ASSIGN, lhs, getLogicOrExpr(code));
        t = getNext(code);
    }
    unget(code);
    return lhs;
}

static ExprAST* getLogicOrExpr(istringstream& code)
{
    l(__FUNCTION__);
    ExprAST* lhs = getLogicAndExpr(code);
    Token t = getNext(code);
    while (t.tokenKind == TokenKind::OPERATOR && t.opKind == OpKind::OR) {
        lhs = new BinaryExprAST(OpKind::OR, lhs, getLogicAndExpr(code));
        t = getNext(code);
    }
    unget(code);
    return lhs;
}

static ExprAST* getLogicAndExpr(istringstream& code)
{
    l(__FUNCTION__);
    ExprAST* lhs = getEqualityExpr(code);
    Token t = getNext(code);
    while (t.tokenKind == TokenKind::OPERATOR && t.opKind == OpKind::AND) {
        lhs = new BinaryExprAST(OpKind::AND, lhs, getEqualityExpr(code));
        t = getNext(code);
    }
    unget(code);
    return lhs;
}

static ExprAST* getEqualityExpr(istringstream& code)
{
    l(__FUNCTION__);
    ExprAST* lhs = getLessBiggerExpr(code);
    Token t = getNext(code);
    if (t.tokenKind == TokenKind::OPERATOR && t.opKind == OpKind::EQ) {
        lhs = new BinaryExprAST(OpKind::EQ, lhs, getLessBiggerExpr(code));
    } else if (t.tokenKind == TokenKind::OPERATOR && t.opKind == OpKind::NEQ) {
        lhs = new BinaryExprAST(OpKind::NEQ, lhs, getLessBiggerExpr(code));
    } else {
        unget(code);
    }
    return lhs;
}

static ExprAST* getLessBiggerExpr(istringstream& code)
{
    l(__FUNCTION__);
    ExprAST* lhs = getAddSubExpr(code);
    Token t = getNext(code);
    if (t.tokenKind == TokenKind::OPERATOR && t.opKind == OpKind::LT) {
        lhs = new BinaryExprAST(OpKind::LT, lhs, getAddSubExpr(code));
    } else if (t.tokenKind == TokenKind::OPERATOR && t.opKind == OpKind::GT) {
        lhs = new BinaryExprAST(OpKind::GT, lhs, getAddSubExpr(code));
    } else {
        unget(code);
    }
    return lhs;
}

static ExprAST* getAddSubExpr(istringstream& code)
{
    l(__FUNCTION__);
    ExprAST* lhs = getMulDivModExpr(code);
    Token t = getNext(code);
    while (t.tokenKind == TokenKind::OPERATOR && (t.opKind == OpKind::ADD || t.opKind == OpKind::SUB)) {
        lhs = new BinaryExprAST(t.opKind, lhs, getMulDivModExpr(code));
        t = getNext(code);
    }
    unget(code);
    return lhs;
}

static ExprAST* getMulDivModExpr(istringstream& code)
{
    l(__FUNCTION__);
    ExprAST* lhs = getUnaryExpr(code);
    Token t = getNext(code);
    while (t.tokenKind == TokenKind::OPERATOR && (t.opKind == OpKind::MUL || t.opKind == OpKind::DIV || t.opKind == OpKind::MOD)) {
        lhs = new BinaryExprAST(t.opKind, lhs, getUnaryExpr(code));
        t = getNext(code);
    }
    unget(code);
    return lhs;
}

static ExprAST* getUnaryExpr(istringstream& code)
{
    l(__FUNCTION__);
    Token t = getNext(code);
    if (t.tokenKind == TokenKind::OPERATOR && (t.opKind == OpKind::NOT || t.opKind == OpKind::MUL || t.opKind == OpKind::AND)) {
        ExprAST* rhs = (t.opKind == OpKind::AND) ? getCallDeref(code) : getUnaryExpr(code);
        return new UnaryExprAST(t.opKind, rhs);
    } else {
        unget(code);
        return getCallDeref(code);
    }
}

static ExprAST* getCallDeref(istringstream& code)
{
    l(__FUNCTION__);
    ExprAST* callee = getPrimary(code);
    Token t = getNext(code);

    // 함수 호출
    if (t.tokenKind == TokenKind::OPEN_PAREN) {
        Token next = getNext(code);
        if (next.tokenKind == TokenKind::CLOSE_PAREN) {
            return new CallExprAST(callee, vector<ExprAST*>());
        }
        vector<ExprAST*> args;
        unget(code);
        while (true) {
            args.push_back(getExpr(code));
            next = getNext(code);
            if (next.tokenKind != TokenKind::COMMA) {
                if (next.tokenKind == TokenKind::CLOSE_PAREN) {
                    return new CallExprAST(callee, args);
                }
                expected(code, "comma", "unknown");
            }
        }

        // 포인터 역참조
    } else if (t.tokenKind == TokenKind::OPEN_ARR) {
        ExprAST* index = getExpr(code);
        maybe(code, TokenKind::CLOSE_ARR);
        return new PointerDerefAST(callee, index);

        // 아무 것도 아닐 때
    } else {
        unget(code);
        return callee;
    }
}

static ExprAST* getPrimary(istringstream& code)
{
    l(__FUNCTION__);
    Token t = getNext(code);
    if (t.tokenKind == TokenKind::OPEN_PAREN) {
        ExprAST* recursive = getExpr(code);
        maybe(code, TokenKind::CLOSE_PAREN);
        return recursive;
    } else {
        unget(code);
        switch (t.tokenKind) {
        case TokenKind::IDENT:
            return getIdent(code);
        case TokenKind::LITERAL:
            if (t.literal.literalKind == LiteralKind::STRING) {
                getNext(code);
                return new StringAST(t.literal.s);
            }
            return getLiteral(code);
        default:
            unget(code);
        }
    }
    return nullptr;
}

static ExprAST* getIdent(istringstream& code)
{
    l(__FUNCTION__);
    Token t = getNext(code);
    if (t.tokenKind == TokenKind::IDENT) {
        return new VariableAST(t.ident);
    }
    std::cout << t.ident << '\n';
    expected(code, "identifier", "unknown");
    return nullptr;
}

static ExprAST* getLiteral(istringstream& code)
{
    l(__FUNCTION__);
    Token t = getNext(code);
    NumericData data;
    if (t.tokenKind == TokenKind::LITERAL) {
        switch (t.literal.literalKind) {
        case LiteralKind::CHAR:
            data.c = t.literal.c;
            break;
        case LiteralKind::INT:
            data.i = t.literal.i;
            break;
        case LiteralKind::DOUBLE:
            data.d = t.literal.d;
            break;
        case LiteralKind::STRING:
            return new StringAST(t.literal.s);
        default: {
        }
        }
        return new NumberAST(t.literal.literalKind, data);
    }

    expected(code, "numeric literal", "unknown");
    return nullptr;
}

static Type getType(istringstream& code)
{
    l(__FUNCTION__);
    Token t = getNext(code);
    int ptrDepth = 0;
    if (t.tokenKind == TokenKind::OPERATOR && t.opKind == OpKind::MUL) {
        ptrDepth++;
        t = getNext(code);
        while (t.tokenKind == TokenKind::OPERATOR && t.opKind == OpKind::MUL) {
            ptrDepth++;
            t = getNext(code);
        }
    }
    if (t.tokenKind != TokenKind::TYPE) {
        wrong(code, "type");
    }
    Type result;
    TypeKind tk = t.typeKind;
    result.kind = tk;
    if (tk == TypeKind::FUNC) {
        result.ptrDepth = 1 + ptrDepth;
        maybe(code, TokenKind::OPEN_PAREN);
        Token lookahead = getNext(code);
        while (lookahead.tokenKind != TokenKind::COLON) {
            unget(code);
            result.argsType.push_back(getType(code));
            lookahead = getNext(code);
            if (lookahead.tokenKind == TokenKind::COMMA)
                lookahead = getNext(code);
        }
        result.returnType = new Type(getType(code));
        maybe(code, TokenKind::CLOSE_PAREN);
    } else {
        result.ptrDepth = ptrDepth;
    }
    return result;
}
