#ifndef __LEXOR_HEADER__
#define __LEXOR_HEADER__

#include "includes.hpp"

enum class TokenKind {
    KEYWORD,
    TYPE,
    IDENT,
    OPERATOR,
    LITERAL,
    OPEN_PAREN,
    CLOSE_PAREN,
    OPEN_ARR,
    CLOSE_ARR,
    OPEN_BLOCK,
    CLOSE_BLOCK,
    COMMA,
    COLON,
    SEMICOLON,
    UNKNOWN,
};

enum class TypeKind {
    CHAR,
    INT,
    DOUBLE,
    FUNC
};

enum class OpKind {
    ADD, // +
    SUB, // -
    MUL, // *
    DIV, // /
    MOD, // %
    AND, // &
    OR, // |
    NOT, // !
    ASSIGN, // =
    LT, // <
    GT, // >
    EQ, // ~
    NEQ, // ^
};

enum class KeywordKind {
    DEF,
    VAR,
    IF,
    ELSE,
    FOR,
    BREAK,
    CONTINUE,
    RETURN,
    USE
};

enum class LiteralKind {
    CHAR,
    INT,
    DOUBLE,
    STRING
};

struct Literal {
    LiteralKind literalKind;
    char c;
    int i;
    double d;
    string s;
};

struct Token {
    TokenKind tokenKind;
    string ident;
    TypeKind typeKind;
    OpKind opKind;
    KeywordKind keywordKind;
    Literal literal;
};

class LexicalParser final {
private:
    Token getIdent(istringstream& code);
    Token getNumber(istringstream& code);
    Token getDelimeter(istringstream& code);
    Token getString(istringstream& code);
    Token getChar(istringstream& code);
    Token getOperator(int c);
    char getEscapeSequence(istringstream& code);
    bool isDelimeter(int c);

public:
    Token getNext(istringstream& stream);
    void unget(istringstream& stream);

private:
    unordered_map<string, KeywordKind> keywordMap = {
        { "def", KeywordKind::DEF },
        { "var", KeywordKind::VAR },
        { "if", KeywordKind::IF },
        { "else", KeywordKind::ELSE },
        { "for", KeywordKind::FOR },
        { "break", KeywordKind::BREAK },
        { "continue", KeywordKind::CONTINUE },
        { "return", KeywordKind::RETURN },
        { "use", KeywordKind::USE }
    };

    unordered_map<string, TypeKind> typeMap = {
        { "char", TypeKind::CHAR },
        { "int", TypeKind::INT },
        { "double", TypeKind::DOUBLE },
        { "func", TypeKind::FUNC }
    };

    int prev = 0;
};

#endif