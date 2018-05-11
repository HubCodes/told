#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <unordered_map>

#include "lexer.hpp"

#define D(x) static_cast<int>(x)

static std::unordered_map<int, std::string> enumMap = {
	{ D(TokenKind::KEYWORD), "Keyword" },
	{ D(TokenKind::IDENT), "Ident" },
	{ D(TokenKind::OPERATOR), "Operator" },
	{ D(TokenKind::LITERAL), "Literal" },
	{ D(TokenKind::OPEN_PAREN), "(" },
	{ D(TokenKind::CLOSE_PAREN), ")" },
	{ D(TokenKind::OPEN_ARR), "[" },
	{ D(TokenKind::CLOSE_ARR), "]" },
	{ D(TokenKind::OPEN_BLOCK), "{" },
	{ D(TokenKind::CLOSE_BLOCK), "}" },
	{ D(TokenKind::COMMA), "," },
	{ D(TokenKind::COLON), ":" },
	{ D(TokenKind::SEMICOLON), ";" },
	{ D(TokenKind::UNKNOWN), "Unknown" },
	{ D(TokenKind::TYPE), "Type" }
};

int main(int argc, char** argv) {
	std::string codeStr;
	std::fstream code(argv[1], std::ios::in);
	if (!code.is_open()) {
		std::cerr << "error: Invalid source code file.\n";
		std::exit(EXIT_FAILURE);
	}
	while (!code.eof())
	{
		codeStr += code.get();
	}	
	std::istringstream is(codeStr);
	
	Token t = getNext(is);
	while (t.tokenKind != TokenKind::UNKNOWN) {
		std::cout << enumMap[D(t.tokenKind)] << '\n';
		if (t.tokenKind == TokenKind::LITERAL)
		{
			switch (t.literal.literalKind)
			{
				case LiteralKind::STRING:
					std::cout << t.literal.s << '\n';
					break;
				case LiteralKind::CHAR:
					std::cout << t.literal.c << '\n';
					break;
				case LiteralKind::DOUBLE:
					std::cout << t.literal.d << '\n';
					break;
				case LiteralKind::INT:
					std::cout << t.literal.i << '\n';
					break;
			}
		}
		t = getNext(is);
	}
	return 0;
}
