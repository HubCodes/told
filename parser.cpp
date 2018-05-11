#include "parser.hpp"
#include "lexer.hpp"

#include <cstdlib>
#include <iostream>

static FunctionDefAST* getFunction(std::istringstream& code);
static VarDefAST* getVardef(std::istringstream& code);
static BlockAST* getBlock(std::istringstream& code);
static ExprAST* getExpr(std::istringstream& code);

static Type getType(std::istringstream& code);

std::vector<AST*> parse(std::istringstream& code) {
	std::vector<AST*> result;
	Token token = getNext(code);
	while (token.tokenKind != TokenKind::UNKNOWN) {
		if (token.tokenKind == TokenKind::KEYWORD &&
			token.keywordKind == KeywordKind::DEF) {
			result.push_back(getFunction(code));			
		} else if (token.tokenKind == TokenKind::KEYWORD &&
			token.keywordKind == KeywordKind::VAR) {
			result.push_back(getVardef(code));	
		} else {
			std::cerr << "Cannot parse toplevel.\n";
			std::exit(EXIT_FAILURE);
		}
		token = getNext(code);
	}
	return result;
}

static FunctionDefAST* getFunction(std::istringstream& code) {
	Token functionId = getNext(code);
	getNext(code);	// skip (
	Token lookahead = getNext(code);
	std::vector<std::tuple<std::string, Type>> args;
	if (lookahead.tokenKind != CLOSE_PAREN) {
		while (lookahead.tokenKind != CLOSE_PAREN) {
			Token id = getNext(code);
			getNext(code); 	// skip :
			Type type = getType(code);
			if (id.tokenKind != TokenKind::IDENT) {
				std::cerr << "Expected identifier.\n";
				std::exit(EXIT_FAILURE);
			}
			args.emplace_back(id.ident, type);
			lookahead = getNext(code);
		}
	} 
	getNext(code); // skip :	
	Type returnType = getType(code);
	auto&& proto = new FunctionDeclAST(returnType, args);
	auto&& block = getBlock(code);
	return new FunctionDefAST(proto, block);
}

static VarDefAST* getVardef(std::istringstream& code) {
	Token identTok = getNext(code);
	if (identTok.tokenKind != TokenKind::IDENT) {
		std::cerr << "Expected identifier.\n";
		std::exit(EXIT_FAILURE);
	}
	std::string id = identTok.ident;
	getNext(code);	// skip :
	Type type = getType(code);
	Token lookahead = getNext(code);
	if (lookahead.tokenKind == TokenKind::SEMICOLON) {
		return new VarDefAST(type, id, nullptr);
	} else if (lookahead.tokenKind == TokenKind::OPERATOR &&
			lookahead.opKind == OpKind::ASSIGN) {
		return new VarDefAST(type, id, getExpr(code));	
	} else {
		std::cerr << "Wrong token.\n";
		std::exit(EXIT_FAILURE);
	}
}

static BlockAST* getBlock(std::istringstream& code) {
	// TODO implementation
}

static BinaryExprAST* getAssignExpr(std::istringstream& code);
static BinaryExprAST* getLogicOrExpr(std::istringstream& code);
static ExprAST* getExpr(std::istringstream& code) {
	return static_cast<ExprAST*>(getAssignExpr(code));
}

static BinaryExprAST* getAssignExpr(std::istringstream& code) {
	BinaryExprAST* high = getLogicOrExpr(code);
	Token t = getNext(code);
	while (t.tokenKind == TokenKind::OPERATOR &&
			t.opKind == OpKind::ASSIGN) {
		high = new BinaryExprAST(OpKind::ASSIGN, high, getLogicalOrExpr(code);
		t = getNext(code);
	}
	unget(code);
	return high;
}

static Type getType(std::istringstream& code) {
	Token t = getNext(code);
	int ptrDepth = 0;
	if (t.tokenKind == TokenKind::OPERATOR &&
		t.opKind == OpKind::MUL) {
		ptrDepth++;
		t = getNext(code);
		while (t.tokenKind == TokenKind::OPERATOR &&
				t.opKind == OpKind::MUL) {
			ptrDepth++;
			t = getNext(code);
		}
	} 
	if (t.tokenKind != TokenKind::TYPE) {
		std::cerr << "Expected type.\n";
		std::exit(EXIT_FAILURE);
	}
	Type result;
	TypeKind tk = t.typeKind;
	result.kind = tk;
	if (tk == TypeKind::FUNC) {
		result.ptrDepth = 1 + ptrDepth;
		getNext(code);	// skip (
		Token lookahead = getNext(code);
		while (lookahead.tokenKind != TokenKind::COLON) {
			result.argsType.push_back(getType(code));
		}
		result.returnType = getType(code);	
		getNext(code); 	// skip )
	}	
	return result;
}
