#include <iostream>
#include <string>
#include <stack>
#include <unordered_map>
#include <utility>
#include <cstdlib>

#include "lexer.hpp"
#include "parser.hpp"

static std::string indent;

#ifdef DEBUG_

#define PRINT(x) std::cout << indent << (x) << '\n';
#define DEL		"------------------------"
#define PUSH	indent += "|   "
#define POP		indent = indent.substr(0, indent.size() - 4)
#else
#define PRINT(x)
#define DEL
#define PUSH
#define POP
#endif

#define D(x) static_cast<int>(x)

#ifdef DEBUG_R
#define l(x) manager.push(CodeManager::TEXT, std::string("#   ") + x)
#else
#define l(x)
#endif

static void error(const char* msg) {
	std::cerr << msg << '\n';
	std::exit(EXIT_FAILURE);
}

const char* getMovInstr(Type type) {
	if (type.ptrDepth > 0) {
		return "mov";
	} else if (type.kind == TypeKind::CHAR) {
		return "movb";
	} else if (type.kind == TypeKind::DOUBLE) {
		return "movq";
	} else if (type.kind == TypeKind::INT) {
		return "movl";
	}
}
void printType(Type t) {
	std::string tykind;
	if (t.kind == TypeKind::CHAR) {
		tykind = "char";
	} else if (t.kind == TypeKind::INT) {
		tykind = "int";
	} else if (t.kind == TypeKind::DOUBLE) {
		tykind = "double";
	} else {
		tykind = "func";
	}
}
/*
 * void NumberAST::codegen()
 *
 * 목적 : 
 * 숫자 리터럴을 로드하는 코드를 생성한다.
 */
void NumberAST::codegen() { 
#ifdef DEBUG_
	PRINT("NumericAST");
	PRINT((kind == LiteralKind::CHAR) ? 
			"char data :" + std::to_string(numericData.c) : (kind == LiteralKind::INT) ? 
			"int data: " + std::to_string(numericData.i) : "double data: " + std::to_string(numericData.d));
	PRINT(DEL);
#else
	l("NumberAST::codegen");	
	if (kind == LiteralKind::DOUBLE) {
		uint64_t casted = *((uint64_t*)&(numericData.d));
		manager.getNextLabel();
		manager.push(CodeManager::DATA, manager.getLabel() + ":", true);
		manager.push(CodeManager::DATA, ".quad" + std::to_string(casted), true);
		manager.push(CodeManager::TEXT, "movsd " + manager.getLabel() + ", %xmm0", true);
	} else {
		int data = (kind == LiteralKind::CHAR) ? numericData.c : numericData.i;
		manager.push(CodeManager::TEXT, "mov $" + std::to_string(data) + ", %rax", true);	
	}
#endif
}

void loadVar(VariableAST* var) {
	l("loadVar");	
	const std::string& id = var->getId();
	const std::string& funcId = manager.getNowFunction();
	if (funcId.empty()) {
		switch (manager.getTypeCode(id)) {
			case TypeCode::CHAR:
				manager.push(CodeManager::TEXT, "movzbq " + id + ", %rax", true);
				break;
			case TypeCode::INT:
				manager.push(CodeManager::TEXT, "movl " + id + ", %eax", true);
				break;
			case TypeCode::DOUBLE:
				manager.push(CodeManager::TEXT, "movsd " + id + ", %xmm0", true);
				break;
			case TypeCode::PTR:
				manager.push(CodeManager::TEXT, "movq " + id + ", %rax", true);
				break;
		}
	} else {
		std::string offset = std::to_string(manager.getLocalVarOffset(funcId, id));
		switch (manager.getTypeCode(id, true, funcId)) {
			case TypeCode::CHAR:
				manager.push(CodeManager::TEXT, "movzbq -" + offset + "(%rbp), %rax", true);
				break;
			case TypeCode::INT:
				manager.push(CodeManager::TEXT, "movl -" + offset + "(%rbp), %eax", true);
				manager.push(CodeManager::TEXT, "cltq", true);
				break;
			case TypeCode::DOUBLE:
				manager.push(CodeManager::TEXT, "movsd -" + offset + "(%rbp), %xmm0", true);
				break;
			case TypeCode::PTR:
				manager.push(CodeManager::TEXT, "mov %rbp, %rax", true);
				manager.push(CodeManager::TEXT, "sub $" + offset + ", %rax", true);
				manager.push(CodeManager::TEXT, "mov (%rax), %rax", true);
				break;
		}
	}
}


/*
 * void VariableAST::codegen()
 *
 * 목적 : 
 * 대상 변수 값을 적절한 레지스터에 로드하는 코드를 생성한다.
 */
void VariableAST::codegen() { 
#ifdef DEBUG_
	PRINT("VariableAST");
	PRINT(id);
	PRINT(DEL);
#else	
	l("VariableAST::codegen");	
	if (manager.isFunction(id)) {
		manager.push(CodeManager::TEXT, "movabsq $" + id + ", %rax", true);	
		return;
	}
	loadVar(this);
#endif
}

/*
 * void StringAST::codegen()
 *
 * 목적 :
 * 문자열 리터럴에 대한 포인터를 레지스터에 적재한다. 
 */
void StringAST::codegen() { 
#ifdef DEBUG_
	PRINT("StringAST");
	PRINT(str);
	PRINT(DEL);
#else
	l("StringAST::codegen");	
	manager.getNextLabel();
	manager.push(CodeManager::DATA, manager.getLabel() + ":");
	manager.push(CodeManager::DATA, ".asciz \"" + str + "\"", true);
	manager.push(CodeManager::TEXT, "movabs $" + manager.getLabel() + ", %rax", true);
#endif
}

// 전제 : 좌측 피연산자는 rbx(xmm1), 우측 피연산자는 rax(xmm0)
void pushBinopExpr(OpKind op, bool isDouble) {
	l("pushBinopExpr");	
	manager.push(CodeManager::TEXT, "pop %rbx", true);
	switch (op) {
		case OpKind::ADD:
			manager.push(CodeManager::TEXT, "add %rax, %rbx", true);	
			break;
		case OpKind::SUB:
			manager.push(CodeManager::TEXT, "sub %rax, %rbx", true);
			break;
		case OpKind::MUL:
			manager.push(CodeManager::TEXT, "imul %rax, %rbx", true);
			break;
		case OpKind::DIV:
		case OpKind::MOD:
			manager.push(CodeManager::TEXT, "mov %rax, %rcx", true);
			manager.push(CodeManager::TEXT, "mov %rbx, %rax", true);
			manager.push(CodeManager::TEXT, "mov %rcx, %rbx", true);
			manager.push(CodeManager::TEXT, "cqto", true);
			manager.push(CodeManager::TEXT, "idiv %rbx", true);
			if (op == OpKind::MOD) {
				manager.push(CodeManager::TEXT, "mov %rdx, %rbx", true);
			} else {
				manager.push(CodeManager::TEXT, "mov %rax, %rbx", true);
			}
			break;
		case OpKind::LT:
		case OpKind::GT:
		case OpKind::EQ:
		case OpKind::NEQ:
			manager.push(CodeManager::TEXT, "cmp %rax, %rbx", true);
			manager.push(CodeManager::TEXT, std::string("set") + (
						(op == OpKind::LT) ? 
						"l" : (op == OpKind::GT) ?
						"g" : (op == OpKind::EQ) ?
						"e" : "ne") + " %bl", true);
			manager.push(CodeManager::TEXT, "movzbq %bl, %rbx", true);
			break;
		case OpKind::AND: {
			manager.getNextLabel();
			std::string label1 = manager.getLabel();
			manager.getNextLabel();
			std::string end = manager.getLabel();
			manager.push(CodeManager::TEXT, "cmp $0, %rbx", true);
			manager.push(CodeManager::TEXT, "je " + label1, true);
			manager.push(CodeManager::TEXT, "cmp $0, %rax", true);	
			manager.push(CodeManager::TEXT, "je " + label1, true);
			manager.push(CodeManager::TEXT, "mov $1, %rbx", true);
			manager.push(CodeManager::TEXT, "jmp " + end, true);
			manager.push(CodeManager::TEXT, label1 + ":");
			manager.push(CodeManager::TEXT, "mov $0, %rbx", true);
			manager.push(CodeManager::TEXT, end + ":");
			break;
		}
		case OpKind::OR: {
			/*
			manager.getNextLabel();
			std::string truee = manager.getLabel();
			manager.getNextLabel();
			std::string end = manager.getLabel();
			manager.push(CodeManager::TEXT, "test %rbx, %rbx", true);
			manager.push(CodeManager::TEXT, "jz " + truee, true);
			manager.push(CodeManager::TEXT, "test %rax, %rax", true);
			manager.push(CodeManager::TEXT, "jz " + truee, true);
			manager.push(CodeManager::TEXT, "mov $0, %rbx", true);
			manager.push(CodeManager::TEXT, "jmp " + end, true);
			manager.push(CodeManager::TEXT, truee + ":");
			manager.push(CodeManager::TEXT, "mov $1, %rbx", true);
			manager.push(CodeManager::TEXT, end + ":");	
			break;
			*/
			manager.getNextLabel();
			std::string label1 = manager.getLabel();
			manager.getNextLabel();
			std::string end = manager.getLabel();
			manager.push(CodeManager::TEXT, "cmp $0, %rbx", true);
			manager.push(CodeManager::TEXT, "jne " + label1, true);
			manager.push(CodeManager::TEXT, "cmp $0, %rax", true);	
			manager.push(CodeManager::TEXT, "jne " + label1, true);
			manager.push(CodeManager::TEXT, "mov $0, %rbx", true);
			manager.push(CodeManager::TEXT, "jmp " + end, true);
			manager.push(CodeManager::TEXT, label1 + ":");
			manager.push(CodeManager::TEXT, "mov $1, %rbx", true);
			manager.push(CodeManager::TEXT, end + ":");
			break;
		}
	}
	manager.push(CodeManager::TEXT, "mov %rbx, %rax", true);
}

// 전제 : rbx 혹은 xmm1에 값이 들어있다.
void pushAssignExpr(ExprAST* lhs, bool leftInt, bool rightInt) {
	l("pushAssignExpr");	
	VariableAST* test1 = dynamic_cast<VariableAST*>(lhs);
	PointerDerefAST* test2 = dynamic_cast<PointerDerefAST*>(lhs);
	std::string nowFunc = manager.getNowFunction();	

	// 1. 변수라면
	if (test1 != nullptr) {
		const std::string& id = test1->getId();		
		int offset = manager.getLocalVarOffset(manager.getNowFunction(), id);
		TypeCode t;
		if (manager.isLocalVar(nowFunc, id)) {
			manager.push(CodeManager::TEXT, "mov %rbp, %rdx", true);
			manager.push(CodeManager::TEXT, "sub $" + std::to_string(offset) + ", %rdx", true);
			t = manager.getTypeCode(id, true, nowFunc); 
		} else if (manager.isGlobalVar(id)) {
			manager.push(CodeManager::TEXT, "movq $" + id + ", %rdx", true);
			t = manager.getTypeCode(id);	
		}
		std::string instr = (t == TypeCode::CHAR) ? 
				"movb" : (t == TypeCode::INT) ?
				"movl" : "movq";
		std::string reg = (t == TypeCode::CHAR) ?
				"bl" : (t == TypeCode::INT) ?
				"ebx" : "rbx";
		if (!rightInt) {
			manager.push(CodeManager::TEXT, "movsd (%rsp), %xmm0", true);
			manager.push(CodeManager::TEXT, "add $8, %rsp", true);
		} else {
			manager.push(CodeManager::TEXT, "pop %rbx", true);
		}
		if (leftInt) {
			if (!rightInt) {
				manager.push(CodeManager::TEXT, "cvttsd2si %xmm1, %rbx", true);
			}
			manager.push(CodeManager::TEXT, instr + " %" + reg + ", (%rdx)", true);
		} else {
			if (rightInt) {
				manager.push(CodeManager::TEXT, "cvtsi2sdl %rbx, %xmm1", true);
			}
			manager.push(CodeManager::TEXT, "movsd %xmm1, (%rdx)", true);
		}
	} 
	
	// 포인터 역참조라면
	else if (test2 != nullptr) {
		test2->getPtr()->codegen();
		manager.push(CodeManager::TEXT, "push %rax", true);
		// manager.push(CodeManager::TEXT, "mov %rax, %rcx", true);
		test2->getIndex()->codegen();
		manager.push(CodeManager::TEXT, "mov %rax, %rcx", true);
		manager.push(CodeManager::TEXT, "pop %rax", true);
		manager.push(CodeManager::TEXT, "pop %rbx", true);
		manager.push(CodeManager::TEXT, "add %rcx, %rax", true);
		// 현재 시점 : rbx = 대입될 값, rax = 포인터 값(확정)
		Type to = test2->getTypeObject();
		to.ptrDepth--;
		TypeCode t = fromType(to);
		std::string instr = 
			(t == TypeCode::CHAR) ? 
			"movb" : (t == TypeCode::INT) ?
			"movl" : "movq";
		std::string reg = (t == TypeCode::CHAR) ?
			"bl" : (t == TypeCode::INT) ?
			"ebx" : "rbx";
		if (leftInt) {
			if (!rightInt) {
				manager.push(CodeManager::TEXT, "cvttsd2si %xmm1, %rbx", true);
			}
			manager.push(CodeManager::TEXT, instr + " %" + reg + ", (%rax)", true);
		} else {
			if (rightInt) {
				manager.push(CodeManager::TEXT, "cvtsi2sdl %rbx, %xmm1", true);
			}
			manager.push(CodeManager::TEXT, "movsd %xmm1, (%rax)", true);
		}
	} else {
		error("Expected variable or pointer expression, but unknown");
	}
}

void BinaryExprAST::codegen() { 
#ifdef DEBUG_
	PRINT("BinaryExprAST");
	PRINT(opMap[static_cast<int>(kind)]);
	PUSH;
	lhs->codegen();
	rhs->codegen();
	POP;
	PRINT(DEL);
#else
	l("BinaryExprAST::codegen");	
	bool leftInt = lhs->isIntegral();
	bool rightInt = rhs->isIntegral();
	if (kind == OpKind::ASSIGN) {
		rhs->codegen();
		if (rhs->isIntegral()) {
			manager.push(CodeManager::TEXT, "push %rax", true);
			// manager.push(CodeManager::TEXT, "mov %rax, %rbx", true);
		} else {
			manager.push(CodeManager::TEXT, "sub $8, %rsp", true);
			manager.push(CodeManager::TEXT, "vmovsd %xmm0, (%rsp)", true);
			// manager.push(CodeManager::TEXT, "movapd %xmm0, %xmm1", true);
		}
		pushAssignExpr(lhs, leftInt, rightInt);
		return;
	}
	// 정수 우위
	if (leftInt && rightInt) {
		lhs->codegen();
		manager.push(CodeManager::TEXT, "push %rax", true);
		// manager.push(CodeManager::TEXT, "mov %rax, %rbx", true);
		rhs->codegen();
	} else if (!leftInt && rightInt) {
		lhs->codegen();
		manager.push(CodeManager::TEXT, "cvttsd2si %xmm0, %rax", true);
		manager.push(CodeManager::TEXT, "push %rax", true);
		rhs->codegen();
	} else if (leftInt && !rightInt) {
		lhs->codegen();
		manager.push(CodeManager::TEXT, "push %rax", true);
		// manager.push(CodeManager::TEXT, "mov %rax, %rbx", true);
		rhs->codegen();
		manager.push(CodeManager::TEXT, "cvttsd2si %xmm0, %rax", true);
	} else {
		lhs->codegen();
		manager.push(CodeManager::TEXT, "sub $8, %rsp", true);
		manager.push(CodeManager::TEXT, "vmovsd %xmm0, %rsp", true);
		// manager.push(CodeManager::TEXT, "movapd %xmm0, %xmm1", true);
		rhs->codegen();
	}
	pushBinopExpr(kind, !leftInt && !rightInt);
#endif
}

void UnaryExprAST::codegen() { 
#ifdef DEBUG_
	PRINT("UnaryExprAST");
	PRINT(opMap[static_cast<int>(kind)]);
	PUSH;
	operand->codegen();
	POP;
	PRINT(DEL);
#else
	l("UnaryExprAST::codegen");	
	if (kind == OpKind::NOT) {
		operand->codegen();
		manager.push(CodeManager::TEXT, "cmp $0, %rax", true);
		manager.push(CodeManager::TEXT, "sete %al", true);
		manager.push(CodeManager::TEXT, "movzbq %al, %rax", true);
	} else if (kind == OpKind::AND) {
		VariableAST* test = dynamic_cast<VariableAST*>(operand);
		if (test == nullptr) {
			error("Expected variable.");
		} else {
			const std::string& func = manager.getNowFunction();
			if (func.empty()) {
				manager.push(CodeManager::TEXT, "movabsq $" + test->getId() + ", %rax", true);
			} else {
				int offset = manager.getLocalVarOffset(func, test->getId());
				manager.push(CodeManager::TEXT, "mov %rbp, %rax", true);
				manager.push(CodeManager::TEXT, "sub $" + std::to_string(offset) + ", %rax", true);
			}
		}
	}
#endif
}

void CallExprAST::codegen() { 
#ifdef DEBUG_
	PRINT("CallExprAST");
	PUSH;
	func->codegen();
	for (auto&& a : args) {
		a->codegen();
	}
	POP;
	PRINT(DEL);
#else
	l("CallExprAST::codegen");	
	func->codegen();
	// manager.push(CodeManager::TEXT, "mov %rax, %r10", true);
	manager.push(CodeManager::TEXT, "push %rax", true);
	for (int i = args.size() - 1; i >= 0; --i) {
		args[i]->codegen();
		manager.push(CodeManager::TEXT, "push %rax", true);
	}
	manager.push(CodeManager::TEXT, "mov " + std::to_string(args.size() * 8) + "(%rsp), %r10", true);
	manager.push(CodeManager::TEXT, "call *%r10", true);
	manager.push(CodeManager::TEXT, "add $" + std::to_string(8 * args.size() + 8) + ", %rsp", true);
#endif
}

void PointerDerefAST::codegen() { 
#ifdef DEBUG_
	PRINT("PointerDerefAST");
	PUSH;
	ptr->codegen();
	index->codegen();
	POP;
	PRINT(DEL);
#else
	l("PointerDerefAST::codegen");	
	TypeCode t; 
	Type to = getTypeObject();
	to.ptrDepth--;
	t = fromType(to);
	ptr->codegen();
	manager.push(CodeManager::TEXT, "push %rax", true);
	// manager.push(CodeManager::TEXT, "mov %rax, %rbx", true);
	index->codegen();
	manager.push(CodeManager::TEXT, "pop %rbx", true);
	manager.push(CodeManager::TEXT, "add %rax, %rbx", true);
	if (t == TypeCode::CHAR) {
		manager.push(CodeManager::TEXT, "movzbq (%rbx), %rax", true);	
	} else if (t == TypeCode::INT) {
		manager.push(CodeManager::TEXT, "movl (%rbx), %eax", true);
	} else if (t == TypeCode::DOUBLE) {
		manager.push(CodeManager::TEXT, "movsd (%rbx), %xmm0", true);
	} else {
		manager.push(CodeManager::TEXT, "movq (%rbx), %rax", true);
	}
#endif
}

void FunctionDeclAST::codegen() { 
#ifdef DEBUG_
	PRINT("FunctionDeclAST");
	PRINT("function name: " + id);
	PRINT(DEL);
#else
	
#endif
}

void BlockAST::codegen() { 
#ifdef DEBUG_
	PRINT("BlockAST");
	PUSH;
	for (auto&& a : blockInternal) {
		a->codegen();
	}
	POP;
	PRINT(DEL);
#else
	l("BlockAST::codegen");	
	for (auto&& a : blockInternal) {
		a->codegen();
	}
#endif
}

void FunctionDefAST::codegen() { 
#ifdef DEBUG_
	PRINT("FunctionDefAST");
	PUSH;
	prototype->codegen();
	block->codegen();
	POP;
	PRINT(DEL);
#else
	l("FunctionDefAST::codegen");	
	auto&& id = prototype->getId();
	manager.insertFunction(id, prototype->getType());
	manager.setNowFunction(id);
	manager.push(CodeManager::TEXT, ".global " + id); 
	manager.push(CodeManager::TEXT, id + ":");
	
	// 함수 프롤로그 
	// Told calling convention: 
	// 3. 기존 rbp를 스택에 백업
	manager.push(CodeManager::TEXT, "push %rbp", true);
	// 4. rbp = rsp로 한 프레임 증가
	manager.push(CodeManager::TEXT, "mov %rsp, %rbp", true);
	// 5. local 변수는 스택에 쌓는다.
	int stackTo = (block->getVariableCount() + prototype->argsCount()) * 8;
	manager.push(CodeManager::TEXT, "sub $" + std::to_string(stackTo) + ", %rsp", true);
	for (int i = 0, j = 8; i < prototype->argsCount(); i++, j += 8) {
		manager.push(CodeManager::TEXT, "mov " + std::to_string(8+j) + "(%rbp), %rax", true);
		auto&& idAndType = prototype->getArg();
		manager.insertLocalVar(id, idAndType.first, idAndType.second); 
		manager.push(CodeManager::TEXT, "mov %rax, -" + std::to_string(j) + "(%rbp)", true);
	}

	// 함수 본문
	block->codegen();	

	if (manager.getTextSeg().back() == "\tret") {
		manager.setNowFunction();
		return;
	}
	// 함수 에필로그
	// 6. rsp = rbp로 프레임 해체 시작
	// 7. rbp = caller's rbp로 프레임 해체 진행
	manager.push(CodeManager::TEXT, "leave", true);
	// 8. rip = caller's next instruction
	// 9. jmp rip
	manager.push(CodeManager::TEXT, "ret", true);
	manager.setNowFunction();
#endif
}

void pushGlobalVar(ExprAST* top) {
	l("pushGlobalVar");	
	enum TypeCode {
		NUMBER,
		STRING,
		ARRAY, 
		NONE
	};
	NumberAST* test1 = dynamic_cast<NumberAST*>(top);
	StringAST* test2 = dynamic_cast<StringAST*>(top);
	ArrayAST* test3 = dynamic_cast<ArrayAST*>(top);
	TypeCode notNull = (test1 != nullptr) ? NUMBER :
						(test2 != nullptr) ? STRING :
						(test3 != nullptr) ? ARRAY :
						NONE;
	if (notNull == NONE) {
		error("Expected Number, String, or Array but expression.");		
	}
	if (notNull == NUMBER) {
		NumericData data = test1->getData();
		LiteralKind kind = test1->getKind();
		std::string size = (kind == LiteralKind::CHAR) ? 
			".byte" : (kind == LiteralKind::INT) ?
			".long" : ".quad";
		manager.push(CodeManager::DATA, size + " " + std::to_string(
					(kind == LiteralKind::CHAR) ? 
					data.c : (kind == LiteralKind::INT) ?
					data.i : *(uint64_t*)(&data.d)), true);
	} else if (notNull == STRING) {
		const std::string& str = test2->getStr();
		manager.push(CodeManager::DATA, ".asciz \"" + str + "\"", true);
	} else if (notNull == ARRAY) {
		const std::vector<ExprAST*>& inits = test3->getInits();
		NumberAST* dtest1 = nullptr;
		StringAST* dtest2 = nullptr;
		for (int i = 0; i < inits.size(); ++i) {
			dtest1 = dynamic_cast<NumberAST*>(inits[i]);	
			std::string elemsize = (dtest1 != nullptr) ?
				((dtest1->getKind() == LiteralKind::CHAR) ? 
				".byte" : (dtest1->getKind() == LiteralKind::INT) ?
				".long" : ".quad") : "e";
			if (elemsize == "e") {
				error("Expected Number");
			}
			NumericData data = dtest1->getData();
			manager.push(CodeManager::DATA, elemsize + " " + 
					std::to_string((elemsize == ".byte") ?
						data.c : (elemsize == ".long") ?
						data.i : data.d), true);
		}
	} 
}

void VarDefAST::codegen() { 
#ifdef DEBUG_
	PRINT("VarDefAST");
	PRINT("Variable name: " + id);
	PUSH;
	if (init)
		init->codegen();
	POP;
	PRINT(DEL);
#else
	l("VarDefAST::codegen");	
	auto&& nowFunc = manager.getNowFunction();

	// 지역 변수일 때
	if (!nowFunc.empty()) {
		manager.insertLocalVar(nowFunc, id, type);
		int offset = manager.getLocalVarOffset(nowFunc, id);
		if (init) {
			init->codegen();
			if (init->isIntegral()) {
				std::string instr(getMovInstr(type));
				std::string reg = (instr == "movb") ? 
					"al" : (instr == "movl") ?
					"eax" : "rax";
				manager.push(CodeManager::TEXT, instr + " %" + reg + ", -" + std::to_string(offset) + "(%rbp)", true);
			} else {
				manager.push(CodeManager::TEXT, "movsd %xmm0, -" + std::to_string(offset) + "(%rbp)", true);
			}
		}
	} 

	// 전역 변수일 때
	else {	
		manager.push(CodeManager::DATA, ".global " + id);
		manager.push(CodeManager::DATA, id + ":");
		pushGlobalVar(init);
		manager.insertGlobalVar(id, type); 
	}
#endif
}

void pushArrayString(const std::vector<ExprAST*>& inits) {
	l("pushArrayString");	
	std::vector<std::string> labels;
	for (auto&& a : inits) {
		StringAST* str = dynamic_cast<StringAST*>(a);
		if (str == nullptr) {
			error("Need string literal but unknown");
		}
		manager.getNextLabel();
		manager.push(CodeManager::DATA, manager.getLabel() + ":");
		labels.push_back(manager.getLabel());
		manager.push(CodeManager::DATA, ".asciz \"" + str->getStr() + "\"", true);
	}
	for (auto&& b : labels) {
		manager.push(CodeManager::DATA, ".quad " + b, true);
	}
}

void ArrayAST::codegen() {
#ifdef DEBUG_
	PRINT("ArrayAST");
	PUSH;
	for (auto&& a : inits) {
		a->codegen();
	}
	POP;
	PRINT(DEL);
#else
	l("ArrayAST::codegen");	
	TypeCode t = fromType(getType());
	std::string datastr;
	if (t == TypeCode::CHAR) {
		datastr = ".byte";	
	} else if (t == TypeCode::INT) {
		datastr = ".long";
	} else {
		datastr = ".quad";
	}  
	manager.getNextLabel();
	std::string arrLabel = manager.getLabel();
	manager.push(CodeManager::TEXT, "movabsq $" + arrLabel + ", %rax", true);
	manager.push(CodeManager::DATA, arrLabel + ":");
	for (auto&& a : inits) {
		NumberAST* code = dynamic_cast<NumberAST*>(a);
		StringAST* str = dynamic_cast<StringAST*>(a);
		if (code == nullptr && str == nullptr) {
			error("Array can initialized with literals.");
		}
		if (code != nullptr) {
			NumericData data = code->getData();
			uint64_t real = (t == TypeCode::CHAR) ?
				data.c : (t == TypeCode::INT) ?
				data.i : *(uint64_t*)(&data.d);
			manager.push(CodeManager::DATA, datastr + " " + std::to_string(real), true);
		} else {
			pushArrayString(inits);
			break;
		}
	}
#endif
}

void IfAST::codegen() { 
#ifdef DEBUG_
	PRINT("IfAST");
	PUSH;
	if (cond) {
		cond->codegen();
	}
	if (then) {
		then->codegen();
	}
	if (els) {
		els->codegen();
	}
	POP;
	PRINT(DEL);
#else
	l("IfAST::codegen");	
	if (cond) {
		manager.getNextLabel();
		std::string elseLabel = manager.getLabel();
		cond->codegen();
		manager.push(CodeManager::TEXT, "cmp $0, %rax", true);
		manager.push(CodeManager::TEXT, "je " + elseLabel, true);
		then->codegen();
		manager.push(CodeManager::TEXT, elseLabel + ":");
		if (els) {
			els->codegen();
		}
	} else {
		then->codegen();
	}
#endif
}

static std::stack<std::pair<std::string, std::string>> forFlow;

void ForAST::codegen() { 
#ifdef DEBUG_
	PRINT("ForAST");
	PUSH;
	if (cond) {
		cond->codegen();
	}
	then->codegen();
	POP;
	PRINT(DEL);
#else
	l("ForAST::codegen");	
	manager.getNextLabel();
	std::string begin = manager.getLabel();
	manager.getNextLabel();
	std::string end = manager.getLabel();
	forFlow.emplace(begin, end);
	manager.push(CodeManager::TEXT, begin + ":");
	cond->codegen();
	manager.push(CodeManager::TEXT, "cmp $0, %rax", true);
	manager.push(CodeManager::TEXT, "je " + end, true);
	then->codegen();
	manager.push(CodeManager::TEXT, "jmp " + begin, true);
	manager.push(CodeManager::TEXT, end + ":");
	forFlow.pop();
#endif
}

void BreakAST::codegen() { 
#ifdef DEBUG_
	PRINT("BreakAST");
	PUSH;
	if (cond) {
		cond->codegen();
	}
	POP;
	PRINT(DEL);
#else
	l("BreakAST::codegen");	
	if (!forFlow.empty()) {
		auto&& pair = forFlow.top();	
		if (cond) {
			cond->codegen();
			manager.push(CodeManager::TEXT, "cmp $0, %rax", true);
			manager.push(CodeManager::TEXT, "jne " + pair.second, true);
		} else {
			manager.push(CodeManager::TEXT, "jmp " + pair.second, true);
		}
	} else {
		error("Cannot insert break statement out of for statement.\n");
	}
#endif
}

void ContinueAST::codegen() { 
#ifdef DEBUG_
	PRINT("ContinueAST");
	PUSH;
	if (cond) {
		cond->codegen();
	}
	POP;
	PRINT(DEL);
#else
	l("ContinueAST::codegen");	
	if (!forFlow.empty()) {
		auto&& pair = forFlow.top();
		if (cond) {
			cond->codegen();
			manager.push(CodeManager::TEXT, "cmp $0, %rax", true);
			manager.push(CodeManager::TEXT, "jne " + pair.second, true);

		} else {
			manager.push(CodeManager::TEXT, "jmp " + pair.second, true);
		}
	} else {
		error("Cannot insert continue statement out of for statement.\n");
	}
#endif
}

void ReturnAST::codegen() { 
#ifdef DEBUG_
	PRINT("ReturnAST");
	PUSH;
	ret->codegen();
	POP;
	PRINT(DEL);
#else
	l("BreakAST::codegen");	
	ret->codegen();
	manager.push(CodeManager::TEXT, "leave", true);
	manager.push(CodeManager::TEXT, "ret", true);
#endif
}

bool NumberAST::isIntegral() {
	return kind != LiteralKind::DOUBLE;
}

bool VariableAST::isIntegral() {
	auto&& func = manager.getNowFunction();
	if (manager.isFunction(id)) {
		return true;
	}
	if (!func.empty()) {
		return manager.isVarIntegral(id, true, func);
	} else {
		return manager.isVarIntegral(id);
	}
}

bool StringAST::isIntegral() {
	return true;
}

bool BinaryExprAST::isIntegral() {
	// 정수 전염성
	return lhs->isIntegral() || rhs->isIntegral();
}

bool UnaryExprAST::isIntegral() {
	// 단항 정수 확정
	return true;
}

bool CallExprAST::isIntegral() {
	if (VariableAST* casted = dynamic_cast<VariableAST*>(func)) {
		Type t;
		if (!manager.getNowFunction().empty()) 
			t = manager.getTypeObject(casted->getId(), true, manager.getNowFunction());
		else
			t = manager.getTypeObject(casted->getId());
		return t.returnType->kind != TypeKind::DOUBLE;
	}
	// 모르는 함수는 일단 int를 반환하는 것으로 간주
	return true;
}

bool PointerDerefAST::isIntegral() {
	return ptr->isIntegral();
}

bool ArrayAST::isIntegral() {
	return true;
}

void CodeManager::clear() {
	data.clear();
	text.clear();
}

void CodeManager::getNextLabel() {
	int counter = std::stoi(label.substr(5, label.size() - 5));
	label = label.substr(0, 5) + std::to_string(counter + 1);
}	

std::string CodeManager::getLabel() {
	return label;
}

void CodeManager::push(Section section, std::string code, bool isIndent) {
	if (isIndent) {
		code = "\t" + code;
	}
	switch (section) {
		case DATA:
			data.push_back(code);
			break;
		case TEXT:
			text.push_back(code);
			break;
	}
}

void CodeManager::setNowFunction(const std::string& id) {
	nowFunction = id;
}

void CodeManager::setNowFunction() {
	nowFunction.clear();
}

const std::string& CodeManager::getNowFunction() {
	return nowFunction;
}

void CodeManager::insertGlobalVar(const std::string& id, Type ty) {
	globalVars[id] = ty;
}

bool CodeManager::isGlobalVar(const std::string& id) {
	return globalVars.find(id) != globalVars.end();
}

bool CodeManager::isLocalVar(const std::string& funcId, const std::string& varId) {
	if (funcId.empty()) return false;
	const std::vector<std::tuple<std::string, int, Type>>& local = localVars[funcId];
	for (int i = 0; i < local.size(); ++i) {
		if (std::get<0>(local[i]) == varId) 
			return true;
	}
	return false;
}

void CodeManager::insertLocalVar(const std::string& funcId, const std::string& varId, Type ty) {
	if (nowOffset.find(funcId) == nowOffset.end()) {
		nowOffset[funcId] = 8;
	}
	localVars[funcId].push_back(std::make_tuple(varId, nowOffset[funcId], ty));
	nowOffset[funcId] += 8;
}

int CodeManager::getLocalVarOffset(const std::string& funcId, const std::string& varId) {
	const std::vector<std::tuple<std::string, int, Type>>& local = localVars[funcId];
	for (int i = 0; i < local.size(); ++i) {
		if (std::get<0>(local[i]) == varId) {
			return std::get<1>(local[i]);
		}
	}
	std::cerr << "Cannot fount variable " << varId << '\n';
	std::exit(EXIT_FAILURE);
}

Type CodeManager::getTypeObject(const std::string& varId, bool isLocal, const std::string& funcId) {
	if (isLocal) {
		if (localVars.find(funcId) != localVars.end()) {
			const std::vector<std::tuple<std::string, int, Type>>& local = localVars[funcId];
			for (int i = 0; i < local.size(); ++i) {
				if (std::get<0>(local[i]) == varId) {
					return std::get<2>(local[i]);	
				}
			}
			if (globalVars.find(varId) != globalVars.end()) {
				return globalVars[funcId];
			} else if (functions.find(varId) != functions.end()) {
				return functions[varId];
			}
			std::cerr << "Cannot found variable " << varId << " on function " << funcId << '\n';
			std::exit(EXIT_FAILURE);
		} else {
			std::cerr << "Cannot found function " << funcId << '\n';
			std::exit(EXIT_FAILURE);
		}
	} else {
		if (globalVars.find(varId) != globalVars.end()) {
			return globalVars[funcId];
		} else if (functions.find(varId) != functions.end()) {
			return functions[varId];
		} else {
			std::cerr << "Cannot found variable on global namespace : " << varId << '\n';
			std::exit(EXIT_FAILURE);
		}
	}
}

TypeCode CodeManager::getTypeCode(const std::string& varId, bool isLocal, const std::string& funcId) {
	return fromType(getTypeObject(varId, isLocal, funcId));
}

bool CodeManager::isVarIntegral(const std::string& varId, bool isLocal, const std::string& funcId) {
	TypeCode tc = getTypeCode(varId, isLocal, funcId);
	return tc != TypeCode::DOUBLE;
}

void CodeManager::insertFunction(const std::string& funcId, Type t) {
	functions[funcId] = t;
}

bool CodeManager::isFunction(const std::string& funcId) {
	return functions.find(funcId) != functions.end();
}

const std::vector<std::string>& CodeManager::getDataSeg() { return data; }
	
const std::vector<std::string>& CodeManager::getTextSeg() { return text; }


