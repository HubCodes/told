#include "parser.hpp"
#include "includes.hpp"
#include "lexer.hpp"

Parser::Parser()
{
}

vector<ASTData> Parser::start_parse(CompilerOption& opt, shared_ptr<CompilerData> cd)
{
    vector<ASTData> datas;
    Parser ps;
    for (auto iter = cd->getCodes().begin();
         iter != cd->getCodes().end(); iter++) {
        istringstream ss(iter->second);
        string filename = iter->first;
        datas.push_back(ps.parse(filename, ss));
    }

    return datas;
}

ASTData::ASTData(const string& filename)
    : filename(filename)
{
}

void ASTData::pushAST(AST* const ast)
{
    ast_vec.push_back(ast);
}

void ASTData::pushUse(const string& use)
{
    use_vec.push_back(use);
}

const vector<AST*>& ASTData::getASTs() const
{
    return ast_vec;
}

const vector<string>& ASTData::getUses() const
{
    return use_vec;
}

const string& ASTData::getFilename() const
{
    return filename;
}

void Parser::expected(istringstream& code, const char* expected, const char* msg)
{
    stringstream ss;
    lex.unget(code);
    std::cout << "Position: " << code.tellg() << '\n';
    std::cout << "Data: " << (char)code.peek() << '\n';
    ss << "Expected " << expected << ", but " << msg;
    error(ss.str());
}

void Parser::wrong(istringstream& code, const char* wron)
{
    lex.unget(code);
    std::cout << "Position: " << code.tellg() << '\n';
    std::cout << "Data: " << (char)code.peek() << '\n';
    error(wron);
}

void Parser::maybe(istringstream& code, TokenKind kind)
{
    if (lex.getNext(code).tokenKind != kind) {
        lex.unget(code);
        std::cout << "Position: " << code.tellg() << '\n';
        std::cout << "Data: " << (char)code.peek() << '\n';
        error("Wrong kind of token.");
    }
}

TypeCode Parser::fromType(Type ty)
{
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

Type Parser::genFuncType(Type returnType, vector<Type> argsType)
{
    Type t;
    t.kind = TypeKind::FUNC;
    t.ptrDepth = 1;
    t.returnType = new Type(returnType);
    t.argsType = argsType;
    return t;
}

ASTData Parser::parse(string& filename, istringstream& code)
{
    ASTData result(filename);
    Token token = lex.getNext(code);
    while (token.tokenKind != TokenKind::UNKNOWN) {
        if (token.tokenKind == TokenKind::KEYWORD && token.keywordKind == KeywordKind::DEF) {
            result.pushAST(getFunction(code));
        } else if (token.tokenKind == TokenKind::KEYWORD && token.keywordKind == KeywordKind::VAR) {
            result.pushAST(getVardef(code));
        } else if (token.tokenKind == TokenKind::KEYWORD && token.keywordKind == KeywordKind::USE) {
            Token usingg = lex.getNext(code);
            result.pushUse(usingg.ident);
            maybe(code, TokenKind::SEMICOLON);
        } else {
            expected(code, "toplevel definitions", "unknown");
        }
        token = lex.getNext(code);
    }
    return result;
}

FunctionDefAST* Parser::getFunction(istringstream& code)
{
    Token functionId = lex.getNext(code);
    if (functionId.tokenKind != TokenKind::IDENT) {
        wrong(code, "identifier");
    }
    maybe(code, TokenKind::OPEN_PAREN);
    Token lookahead = lex.getNext(code);
    vector<std::tuple<string, Type>> args;
    if (lookahead.tokenKind != TokenKind::CLOSE_PAREN) {
        while (true) {
            // Token id = lex.getNext(code);
            maybe(code, TokenKind::COLON);
            Type type = getType(code);
            if (lookahead.tokenKind != TokenKind::IDENT) {
                expected(code, "identifier", "unknown");
            }
            args.emplace_back(lookahead.ident, type);
            lookahead = lex.getNext(code);
            if (lookahead.tokenKind == TokenKind::CLOSE_PAREN) {
                break;
            } else if (lookahead.tokenKind == TokenKind::COMMA) {
                lookahead = lex.getNext(code);
            } else {
                wrong(code, "delimiter in prototype");
            }
        }
    }
    maybe(code, TokenKind::COLON);
    Type returnType = getType(code);
    auto&& proto = new FunctionDeclAST(functionId.ident, returnType, args);
    isDefinableVar = true;
    BlockAST* block;
    if (lex.getNext(code).tokenKind != TokenKind::SEMICOLON) {
        block = nullptr;
    } else {
        block = getBlock(code);
    }
    isDefinableVar = false;
    return new FunctionDefAST(proto, block);
}

VarDefAST* Parser::getVardef(istringstream& code)
{
    Token identTok = lex.getNext(code);
    if (identTok.tokenKind != TokenKind::IDENT) {
        expected(code, "identifier", "unknown");
    }
    string id = identTok.ident;
    maybe(code, TokenKind::COLON);
    Type type = getType(code);
    Token lookahead = lex.getNext(code);
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

BlockAST* Parser::getBlock(istringstream& code)
{
    vector<AST*> block;
    maybe(code, TokenKind::OPEN_BLOCK);
    Token lookahead = lex.getNext(code);
    while (lookahead.tokenKind != TokenKind::CLOSE_BLOCK) {
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
            lex.unget(code);
            block.push_back(getExpr(code));
            maybe(code, TokenKind::SEMICOLON);
        }
        lookahead = lex.getNext(code);
    }
    return new BlockAST(block);
}

IfAST* Parser::getIf(istringstream& code)
{
    maybe(code, TokenKind::OPEN_PAREN);
    ExprAST* cond = getExpr(code);
    maybe(code, TokenKind::CLOSE_PAREN);
    BlockAST* then = getBlock(code);
    Token lookahead = lex.getNext(code);

    // else문이 있을 경우
    if (lookahead.tokenKind == TokenKind::KEYWORD && lookahead.keywordKind == KeywordKind::ELSE) {
        Token isIf = lex.getNext(code);

        // 1. else if일 경우
        if (isIf.tokenKind == TokenKind::KEYWORD & isIf.keywordKind == KeywordKind::IF) {
            vector<AST*> elifblock;
            elifblock.push_back(getIf(code));
            return new IfAST(cond, then, new BlockAST(elifblock));
        }

        // 2. else일 경우
        else if (isIf.tokenKind == TokenKind::OPEN_BLOCK) {
            lex.unget(code);
            return new IfAST(cond, then, getBlock(code));
        }

        // else문이 없을 경우
    }

    lex.unget(code);
    return new IfAST(cond, then, nullptr);
}

ForAST* Parser::getFor(istringstream& code)
{
    maybe(code, TokenKind::OPEN_PAREN);
    ExprAST* cond = getExpr(code);
    maybe(code, TokenKind::CLOSE_PAREN);
    BlockAST* then = getBlock(code);
    return new ForAST(cond, then);
}

BreakAST* Parser::getBreak(istringstream& code)
{
    Token lookahead = lex.getNext(code);
    if (lookahead.tokenKind == TokenKind::OPEN_PAREN) {
        ExprAST* cond = getExpr(code);
        maybe(code, TokenKind::CLOSE_PAREN);
        maybe(code, TokenKind::SEMICOLON);
        return new BreakAST(cond);
    } else {
        lex.unget(code);
        maybe(code, TokenKind::SEMICOLON);
        return new BreakAST(nullptr);
    }
}

ContinueAST* Parser::getContinue(istringstream& code)
{
    Token lookahead = lex.getNext(code);
    if (lookahead.tokenKind == TokenKind::OPEN_PAREN) {
        ExprAST* cond = getExpr(code);
        maybe(code, TokenKind::CLOSE_PAREN);
        maybe(code, TokenKind::SEMICOLON);
        return new ContinueAST(cond);
    } else {
        lex.unget(code);
        maybe(code, TokenKind::SEMICOLON);
        return new ContinueAST(nullptr);
    }
}

ReturnAST* Parser::getReturn(istringstream& code)
{
    ExprAST* ret = getExpr(code);
    maybe(code, TokenKind::SEMICOLON);
    return new ReturnAST(ret);
}

ExprAST* Parser::getExpr(istringstream& code)
{
    Token t = lex.getNext(code);
    if (t.tokenKind == TokenKind::OPEN_ARR) {
        return getArray(code);
    }
    lex.unget(code);
    return getAssignExpr(code);
}

ExprAST* Parser::getArray(istringstream& code)
{
    Type t = getType(code);
    maybe(code, TokenKind::CLOSE_ARR);
    maybe(code, TokenKind::OPEN_BLOCK);
    Token next = lex.getNext(code);
    if (next.tokenKind == TokenKind::CLOSE_BLOCK) {
        return new ArrayAST(t, vector<ExprAST*>());
    }
    lex.unget(code);
    vector<ExprAST*> inits;
    do {
        inits.push_back(getAssignExpr(code));
        next = lex.getNext(code);
    } while (next.tokenKind == TokenKind::COMMA);
    if (next.tokenKind != TokenKind::CLOSE_BLOCK) {
        expected(code, "close block symbol }", "unexpected token");
    }
    return new ArrayAST(t, inits);
}

ExprAST* Parser::getAssignExpr(istringstream& code)
{
    ExprAST* lhs = getLogicOrExpr(code);
    Token t = lex.getNext(code);
    while (t.tokenKind == TokenKind::OPERATOR && t.opKind == OpKind::ASSIGN) {
        lhs = new BinaryExprAST(OpKind::ASSIGN, lhs, getLogicOrExpr(code));
        t = lex.getNext(code);
    }
    lex.unget(code);
    return lhs;
}

ExprAST* Parser::getLogicOrExpr(istringstream& code)
{
    ExprAST* lhs = getLogicAndExpr(code);
    Token t = lex.getNext(code);
    while (t.tokenKind == TokenKind::OPERATOR && t.opKind == OpKind::OR) {
        lhs = new BinaryExprAST(OpKind::OR, lhs, getLogicAndExpr(code));
        t = lex.getNext(code);
    }
    lex.unget(code);
    return lhs;
}

ExprAST* Parser::getLogicAndExpr(istringstream& code)
{
    ExprAST* lhs = getEqualityExpr(code);
    Token t = lex.getNext(code);
    while (t.tokenKind == TokenKind::OPERATOR && t.opKind == OpKind::AND) {
        lhs = new BinaryExprAST(OpKind::AND, lhs, getEqualityExpr(code));
        t = lex.getNext(code);
    }
    lex.unget(code);
    return lhs;
}

ExprAST* Parser::getEqualityExpr(istringstream& code)
{
    ExprAST* lhs = getLessBiggerExpr(code);
    Token t = lex.getNext(code);
    if (t.tokenKind == TokenKind::OPERATOR && t.opKind == OpKind::EQ) {
        lhs = new BinaryExprAST(OpKind::EQ, lhs, getLessBiggerExpr(code));
    } else if (t.tokenKind == TokenKind::OPERATOR && t.opKind == OpKind::NEQ) {
        lhs = new BinaryExprAST(OpKind::NEQ, lhs, getLessBiggerExpr(code));
    } else {
        lex.unget(code);
    }
    return lhs;
}

ExprAST* Parser::getLessBiggerExpr(istringstream& code)
{
    ExprAST* lhs = getAddSubExpr(code);
    Token t = lex.getNext(code);
    if (t.tokenKind == TokenKind::OPERATOR && t.opKind == OpKind::LT) {
        lhs = new BinaryExprAST(OpKind::LT, lhs, getAddSubExpr(code));
    } else if (t.tokenKind == TokenKind::OPERATOR && t.opKind == OpKind::GT) {
        lhs = new BinaryExprAST(OpKind::GT, lhs, getAddSubExpr(code));
    } else {
        lex.unget(code);
    }
    return lhs;
}

ExprAST* Parser::getAddSubExpr(istringstream& code)
{
    ExprAST* lhs = getMulDivModExpr(code);
    Token t = lex.getNext(code);
    while (t.tokenKind == TokenKind::OPERATOR && (t.opKind == OpKind::ADD || t.opKind == OpKind::SUB)) {
        lhs = new BinaryExprAST(t.opKind, lhs, getMulDivModExpr(code));
        t = lex.getNext(code);
    }
    lex.unget(code);
    return lhs;
}

ExprAST* Parser::getMulDivModExpr(istringstream& code)
{
    ExprAST* lhs = getUnaryExpr(code);
    Token t = lex.getNext(code);
    while (t.tokenKind == TokenKind::OPERATOR && (t.opKind == OpKind::MUL || t.opKind == OpKind::DIV || t.opKind == OpKind::MOD)) {
        lhs = new BinaryExprAST(t.opKind, lhs, getUnaryExpr(code));
        t = lex.getNext(code);
    }
    lex.unget(code);
    return lhs;
}

ExprAST* Parser::getUnaryExpr(istringstream& code)
{
    Token t = lex.getNext(code);
    if (t.tokenKind == TokenKind::OPERATOR && (t.opKind == OpKind::NOT || t.opKind == OpKind::MUL || t.opKind == OpKind::AND)) {
        ExprAST* rhs = (t.opKind == OpKind::AND) ? getCallDeref(code) : getUnaryExpr(code);
        return new UnaryExprAST(t.opKind, rhs);
    } else {
        lex.unget(code);
        return getCallDeref(code);
    }
}

ExprAST* Parser::getCallDeref(istringstream& code)
{
    ExprAST* callee = getPrimary(code);
    Token t = lex.getNext(code);

    // 함수 호출
    if (t.tokenKind == TokenKind::OPEN_PAREN) {
        Token next = lex.getNext(code);
        if (next.tokenKind == TokenKind::CLOSE_PAREN) {
            return new CallExprAST(callee, vector<ExprAST*>());
        }
        vector<ExprAST*> args;
        lex.unget(code);
        while (true) {
            args.push_back(getExpr(code));
            next = lex.getNext(code);
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
        lex.unget(code);
        return callee;
    }
}

ExprAST* Parser::getPrimary(istringstream& code)
{
    Token t = lex.getNext(code);
    if (t.tokenKind == TokenKind::OPEN_PAREN) {
        ExprAST* recursive = getExpr(code);
        maybe(code, TokenKind::CLOSE_PAREN);
        return recursive;
    } else {
        lex.unget(code);
        switch (t.tokenKind) {
        case TokenKind::IDENT:
            return getIdent(code);
        case TokenKind::LITERAL:
            if (t.literal.literalKind == LiteralKind::STRING) {
                lex.getNext(code);
                return new StringAST(t.literal.s);
            }
            return getLiteral(code);
        default:
            lex.unget(code);
        }
    }
    return nullptr;
}

ExprAST* Parser::getIdent(istringstream& code)
{
    Token t = lex.getNext(code);
    if (t.tokenKind == TokenKind::IDENT) {
        return new VariableAST(t.ident);
    }
    std::cout << t.ident << '\n';
    expected(code, "identifier", "unknown");
    return nullptr;
}

ExprAST* Parser::getLiteral(istringstream& code)
{
    Token t = lex.getNext(code);
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

Type Parser::getType(istringstream& code)
{
    Token t = lex.getNext(code);
    int ptrDepth = 0;
    if (t.tokenKind == TokenKind::OPERATOR && t.opKind == OpKind::MUL) {
        ptrDepth++;
        t = lex.getNext(code);
        while (t.tokenKind == TokenKind::OPERATOR && t.opKind == OpKind::MUL) {
            ptrDepth++;
            t = lex.getNext(code);
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
        Token lookahead = lex.getNext(code);
        while (lookahead.tokenKind != TokenKind::COLON) {
            lex.unget(code);
            result.argsType.push_back(getType(code));
            lookahead = lex.getNext(code);
            if (lookahead.tokenKind == TokenKind::COMMA)
                lookahead = lex.getNext(code);
        }
        result.returnType = new Type(getType(code));
        maybe(code, TokenKind::CLOSE_PAREN);
    } else {
        result.ptrDepth = ptrDepth;
    }
    return result;
}
