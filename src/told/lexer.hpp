#pragma once

#include <string>
#include <sstream>

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
	ADD,	// +
	SUB,	// -
	MUL,	// *
	DIV,	// /
	MOD,	// %
	AND,	// &
	OR,		// |
	NOT,	// !
	ASSIGN,	// = 
	LT,		// <
	GT,		// >
	EQ,		// ~
	NEQ,	// ^
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
	std::string s;
};

struct Token {
	TokenKind tokenKind;
	std::string ident;
	TypeKind typeKind;
	OpKind opKind;
	KeywordKind keywordKind;
	Literal literal;	
};

Token getNext(std::istringstream& stream);
void unget(std::istringstream& stream);
