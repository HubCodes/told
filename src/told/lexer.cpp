#include "lexer.hpp"

#include <cctype>
#include <cstdlib>
#include <errno.h>
#include <iostream>
#include <unordered_map>

static std::unordered_map<std::string, KeywordKind> keywordMap = {
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

static std::unordered_map<std::string, TypeKind> typeMap = {
	{ "char", TypeKind::CHAR },
	{ "int", TypeKind::INT },
	{ "double", TypeKind::DOUBLE },
	{ "func", TypeKind::FUNC }
};

static Token getIdent(std::istringstream& code);
static Token getNumber(std::istringstream& code);
static Token getDelimeter(std::istringstream& code);
static Token getString(std::istringstream& code);
static Token getChar(std::istringstream& code);
static Token getOperator(int c);
static char getEscapeSequence(std::istringstream& code);
static bool isDelimeter(int c);

int prev = 0;

Token getNext(std::istringstream& code) {
	while (std::isspace(code.peek())) code.get();
	int c = code.peek();
	Token result;
	prev = code.tellg();

	// 1. 식별자일 경우
	if (std::isalpha(c) || c == '$' || c == '_') 
	{
		result = getIdent(code);	
	}
	
	// 2. 숫자일 경우
	else if (std::isdigit(c))
	{
		result = getNumber(code);	
	}

	// 3. 구별자일 경우
	else if (isDelimeter(c)) 
	{
		result = getDelimeter(code);
	}

	// 4. 문자열의 시작일 경우
	else if (c == '\"')
	{
		result = getString(code);
	}

	// 5. 문자의 시작일 경우
	else if (c == '\'')
	{
		result = getChar(code);
	}

	// 끝일 경우
	else /* if (c == EOF || c is unknown) */
	{
		result.tokenKind = TokenKind::UNKNOWN;
	}
	return result;
};

void unget(std::istringstream& code) {
	code.seekg(prev);
}

static Token getIdent(std::istringstream& code) {
	Token result;
	std::string identStr;
	identStr += code.get();
	while (std::isalnum(code.peek()) || code.peek() == '$' || code.peek() == '_')
	{
		identStr += code.get();
	}
	if (keywordMap.find(identStr) != keywordMap.end())
	{
		result.tokenKind = TokenKind::KEYWORD;
		result.keywordKind = keywordMap[identStr];
	} 
	else if (typeMap.find(identStr) != typeMap.end())
	{
		result.tokenKind = TokenKind::TYPE;
		result.typeKind = typeMap[identStr];
	}
	else
	{
		result.tokenKind = TokenKind::IDENT;
		result.ident = identStr;
	}
	return result;	
}

static Token getNumber(std::istringstream& code) {
	Token result;
	std::string numstr;
	numstr += code.get();
	while (std::isdigit(code.peek()) || code.peek() == '.') 
	{
		numstr += code.get();
	}
	int i;
	double d;
	bool isInt;
	if (std::string::npos == numstr.find('.'))
		isInt = true;
	else
		isInt = false;

	errno = 0;
	char* end;
	i = isInt ? std::strtol(numstr.c_str(), &end, 10) : 0;
	d = !isInt ? std::strtod(numstr.c_str(), &end) : 0.0;
	if (errno) {
		Token unknown;
		unknown.tokenKind = TokenKind::UNKNOWN;
		return unknown;
	}
	result.tokenKind = TokenKind::LITERAL;	
	result.literal.literalKind = isInt ? LiteralKind::INT : LiteralKind::DOUBLE;
	if (isInt)
	{
		result.literal.i = i;
	}
	else
	{
		result.literal.d = d;
	}
	result.ident = numstr;
	return result;
}

static Token getDelimeter(std::istringstream& code) {
	Token result;
	int c = code.get();
	switch (c) {
		case '(':
			result.tokenKind = TokenKind::OPEN_PAREN;
			break;
		case ')':
			result.tokenKind = TokenKind::CLOSE_PAREN;
			break;
		case '[':
			result.tokenKind = TokenKind::OPEN_ARR;
			break;
		case ']':
			result.tokenKind = TokenKind::CLOSE_ARR;
			break;
		case '{':
			result.tokenKind = TokenKind::OPEN_BLOCK;
			break;
		case '}':
			result.tokenKind = TokenKind::CLOSE_BLOCK;
			break;
		case ',':
			result.tokenKind = TokenKind::COMMA;
			break;
		case ':':
			result.tokenKind = TokenKind::COLON;
			break;
		case ';':
			result.tokenKind = TokenKind::SEMICOLON;
			break;
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '&':
		case '|':
		case '!':
		case '=':
		case '<':
		case '>':
		case '^':
		case '~':
			return getOperator(c);
	}
	std::string ident;
	ident += c;
	result.ident = ident;
	return result;
}

static Token getString(std::istringstream& code) {
	Token result;
	std::string str;
	code.get();	// " 문자
	while (code.peek() != '\"')
	{
		str += code.get();
	}
	code.get();	// " 문자
	result.tokenKind = TokenKind::LITERAL;
	result.literal.literalKind = LiteralKind::STRING;
	result.literal.s = str;
	result.ident = str;
	return result;
}

static Token getChar(std::istringstream& code) {
	Token result;
	char c;
	code.get(); // ' 문자
	if ('\\' == code.peek())
	{
		c = getEscapeSequence(code);	
	}
	else 
	{
		c = code.get();	
	}
	code.get(); // ' 문자
	result.tokenKind = TokenKind::LITERAL;
	result.literal.literalKind = LiteralKind::CHAR;
	result.literal.c = c;
	std::string ident;
	ident += c;
	result.ident = ident;
	return result;
}

static Token getOperator(int c) {
	Token result;
	result.tokenKind = TokenKind::OPERATOR;
	switch (c) {
		case '+':
			result.opKind = OpKind::ADD;
			break;
		case '-':
			result.opKind = OpKind::SUB;
			break;
		case '*':
			result.opKind = OpKind::MUL;
			break;
		case '/':
			result.opKind = OpKind::DIV;
			break;
		case '%':
			result.opKind = OpKind::MOD;
			break;
		case '&':
			result.opKind = OpKind::AND;
			break;
		case '|':
			result.opKind = OpKind::OR;
			break;
		case '!':
			result.opKind = OpKind::NOT;
			break;
		case '=':
			result.opKind = OpKind::ASSIGN;
			break;
		case '<':
			result.opKind = OpKind::LT;
			break;
		case '>':
			result.opKind = OpKind::GT;
			break;
		case '~':
			result.opKind = OpKind::EQ;
			break;
		case '^':
			result.opKind = OpKind::NEQ;
			break;
	}
	std::string ident;
	ident += c;
	result.ident = ident;
	return result;
}

static char getEscapeSequence(std::istringstream& code) {
	static std::unordered_map<char, char> escMap = {
		{ '\'', '\'' },
		{ '\"', '\"' },
		{ '\?',	'\?' },
		{ '\\', '\\' },
		{ 'a', '\a' },
		{ 'b', '\b' },
		{ 'f', '\f' },
		{ 'n', '\n' },
		{ 'r', '\r' },
		{ 't', '\t' },
		{ 'v', '\v' },
		{ '0', '\0' }
	};
	code.get();	// / 문자 스킵
	int c = code.get();
	if (escMap.find(c) == escMap.end())
	{
		std::cerr << "error: Escape sequence character is needed.\n";
		std::exit(EXIT_FAILURE);
	}
	else 
	{
		c = escMap[c];
	}
	return c;
}

static bool isDelimeter(int c) {
	switch (c) {
		case '(':
		case ')':
		case '[':
		case ']':
		case '{':
		case '}':
		case ',':
		case ':':
		case ';':
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '&':
		case '|':
		case '!':
		case '=':
		case '<':
		case '>':
		case '~':
		case '^':
			return true;
		default:
			return false;
	}
}
