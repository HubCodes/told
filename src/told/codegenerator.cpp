#include "codegenerator.hpp"
#include "includes.hpp"
#include "parser.hpp"

CodeGenerator::CodeGenerator(CompilerOption& opt)
    : opt(opt)
{
}

static CodeManager& manager = CodeManager::create();

void CodeGenerator::generateCode(vector<ASTData>& datas)
{

    for (ASTData data : datas) {
        manager = CodeManager::create();

        const vector<std::pair<string, Type>>& vec = getPairs(data.getUses());
        std::for_each(vec.begin(), vec.end(), [](const std::pair<string, Type>& funcId) {
            manager.insertFunction(funcId.first, funcId.second);
        });
        delete &vec;

        for (auto&& ast : data.getASTs()) {
            ast->codegen();
        }

        string out_obj;

        if (opt.isOutputToFile()) {
            std::string::size_type idx(data.getFilename().rfind('.'));

            if (idx != std::string::npos) {
                out_obj = data.getFilename().substr(0, idx);
                out_obj += ".s";
            } else {
                out_obj = data.getFilename();
                out_obj += ".s";
            }

            std::ofstream fos(out_obj);

            if (!fos.is_open()) {
                string e("Cannot open file : ");
                e += out_obj;
                error(e);
            }
            writeCode(fos, manager);
        } else {
            std::cout << "\n----------[ " << data.getFilename() << "]----------\n";
            writeCode(std::cout, manager);
            std::cout << "\n";
            continue;
        }

        assemble(out_obj);
    }
}

void CodeGenerator::assemble(const string& filename)
{
    string comand("ld -s");
}

const vector<std::pair<string, Type>>& CodeGenerator::getPairs(vector<string> uses)
{
    vector<std::pair<string, Type>> *pairs = new vector<std::pair<string, Type>>();

    

    return *pairs;
}
void CodeGenerator::writeCode(std::ostream& stream, CodeManager& manager)
{
    stream << ".section .data\n";
    for (auto&& str : manager.getDataSeg()) {
        stream << str << '\n';
    }
    stream << ".section .text\n";
    for (auto&& str : manager.getTextSeg()) {
        stream << str << '\n';
    }
}

#define D(x) static_cast<int>(x)

const char* getMovInstr(Type type)
{
    if (type.ptrDepth > 0) {
        return "mov";
    } else if (type.kind == TypeKind::CHAR) {
        return "movb";
    } else if (type.kind == TypeKind::DOUBLE) {
        return "movq";
    } else if (type.kind == TypeKind::INT) {
        return "movl";
    } else {
        return "";
    }
}
void printType(Type t)
{
    string tykind;
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
void NumberAST::codegen()
{
    if (kind == LiteralKind::DOUBLE) {
        uint64_t casted = *((uint64_t*)&(numericData.d));
        manager.getNextLabel();
        manager.push(CodeManager::DATA, manager.getLabel() + ":", true);
        manager.push(CodeManager::DATA, ".quad" + std::to_string(casted), true);
        manager.push(CodeManager::TEXT, "movsd " + manager.getLabel() + ", %xmm0",
            true);
    } else {
        int data = (kind == LiteralKind::CHAR) ? numericData.c : numericData.i;
        manager.push(CodeManager::TEXT, "mov $" + std::to_string(data) + ", %rax",
            true);
    }
}

void loadVar(VariableAST* var)
{
    const string& id = var->getId();
    const string& funcId = manager.getNowFunction();
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
        string offset = std::to_string(manager.getLocalVarOffset(funcId, id));
        switch (manager.getTypeCode(id, true, funcId)) {
        case TypeCode::CHAR:
            manager.push(CodeManager::TEXT, "movzbq -" + offset + "(%rbp), %rax",
                true);
            break;
        case TypeCode::INT:
            manager.push(CodeManager::TEXT, "movl -" + offset + "(%rbp), %eax",
                true);
            manager.push(CodeManager::TEXT, "cltq", true);
            break;
        case TypeCode::DOUBLE:
            manager.push(CodeManager::TEXT, "movsd -" + offset + "(%rbp), %xmm0",
                true);
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
void VariableAST::codegen()
{
    if (manager.isFunction(id)) {
        manager.push(CodeManager::TEXT, "movabsq $" + id + ", %rax", true);
        return;
    }
    loadVar(this);
}

/*
 * void StringAST::codegen()
 *
 * 목적 :
 * 문자열 리터럴에 대한 포인터를 레지스터에 적재한다.
 */
void StringAST::codegen()
{
    manager.getNextLabel();
    manager.push(CodeManager::DATA, manager.getLabel() + ":");
    manager.push(CodeManager::DATA, ".asciz \"" + str + "\"", true);
    manager.push(CodeManager::TEXT, "movabs $" + manager.getLabel() + ", %rax",
        true);
}

// 전제 : 좌측 피연산자는 rbx(xmm1), 우측 피연산자는 rax(xmm0)
void pushBinopExpr(OpKind op, bool isDouble)
{
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
        manager.push(
            CodeManager::TEXT,
            string("set") + ((op == OpKind::LT) ? "l" : (op == OpKind::GT) ? "g" : (op == OpKind::EQ) ? "e" : "ne") + " %bl",
            true);
        manager.push(CodeManager::TEXT, "movzbq %bl, %rbx", true);
        break;
    case OpKind::AND: {
        manager.getNextLabel();
        string label1 = manager.getLabel();
        manager.getNextLabel();
        string end = manager.getLabel();
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
        manager.getNextLabel();
        string truee = manager.getLabel();
        manager.getNextLabel();
        string end = manager.getLabel();
        manager.push(CodeManager::TEXT, "test %rbx, %rbx", true);
        manager.push(CodeManager::TEXT, "jz " + truee, true);
        manager.push(CodeManager::TEXT, "test %rax, %rax", true);
        manager.push(CodeManager::TEXT, "jz " + truee, true);
        manager.push(CodeManager::TEXT, "mov $0, %rbx", true);
        manager.push(CodeManager::TEXT, "jmp " + end, true);
        manager.push(CodeManager::TEXT, truee + ":");
        manager.push(CodeManager::TEXT, "mov $1, %rbx", true);
        manager.push(CodeManager::TEXT, end + ":");
    }
    default: {
    }
    }
    manager.push(CodeManager::TEXT, "mov %rbx, %rax", true);
}

// 전제 : rbx 혹은 xmm1에 값이 들어있다.
void pushAssignExpr(ExprAST* lhs, bool leftInt, bool rightInt)
{
    VariableAST* test1 = dynamic_cast<VariableAST*>(lhs);
    PointerDerefAST* test2 = dynamic_cast<PointerDerefAST*>(lhs);
    string nowFunc = manager.getNowFunction();

    // 1. 변수라면
    if (test1 != nullptr) {
        const string& id = test1->getId();
        int offset = manager.getLocalVarOffset(manager.getNowFunction(), id);
        TypeCode t;
        if (manager.isLocalVar(nowFunc, id)) {
            manager.push(CodeManager::TEXT, "mov %rbp, %rdx", true);
            manager.push(CodeManager::TEXT,
                "sub $" + std::to_string(offset) + ", %rdx", true);
            t = manager.getTypeCode(id, true, nowFunc);
        } else if (manager.isGlobalVar(id)) {
            manager.push(CodeManager::TEXT, "movq $" + id + ", %rdx", true);
            t = manager.getTypeCode(id);
        }
        string instr = (t == TypeCode::CHAR) ? "movb" : (t == TypeCode::INT) ? "movl" : "movq";
        string reg = (t == TypeCode::CHAR) ? "bl" : (t == TypeCode::INT) ? "ebx" : "rbx";
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
        TypeCode t = Parser::fromType(to);
        string instr = (t == TypeCode::CHAR) ? "movb" : (t == TypeCode::INT) ? "movl" : "movq";
        string reg = (t == TypeCode::CHAR) ? "bl" : (t == TypeCode::INT) ? "ebx" : "rbx";
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

void BinaryExprAST::codegen()
{
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
}

void UnaryExprAST::codegen()
{
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
            const string& func = manager.getNowFunction();
            if (func.empty()) {
                manager.push(CodeManager::TEXT, "movabsq $" + test->getId() + ", %rax",
                    true);
            } else {
                int offset = manager.getLocalVarOffset(func, test->getId());
                manager.push(CodeManager::TEXT, "mov %rbp, %rax", true);
                manager.push(CodeManager::TEXT,
                    "sub $" + std::to_string(offset) + ", %rax", true);
            }
        }
    }
}

void CallExprAST::codegen()
{
    func->codegen();
    manager.push(CodeManager::TEXT, "mov %rax, %r10", true);
    for (int i = args.size() - 1; i >= 0; --i) {
        args[i]->codegen();
        manager.push(CodeManager::TEXT, "push %rax", true);
    }
    manager.push(CodeManager::TEXT, "call *%r10", true);
    manager.push(CodeManager::TEXT,
        "add $" + std::to_string(8 * args.size()) + ", %rsp", true);
}

void PointerDerefAST::codegen()
{
    TypeCode t;
    Type to = getTypeObject();
    to.ptrDepth--;
    t = Parser::fromType(to);
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
}

void FunctionDeclAST::codegen()
{
}

void BlockAST::codegen()
{
    for (auto&& a : blockInternal) {
        a->codegen();
    }
}

void FunctionDefAST::codegen()
{
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
    manager.push(CodeManager::TEXT, "sub $" + std::to_string(stackTo) + ", %rsp",
        true);
    for (int i = 0, j = 8; i < prototype->argsCount(); i++, j += 8) {
        manager.push(CodeManager::TEXT,
            "mov " + std::to_string(8 + j) + "(%rbp), %rax", true);
        auto&& idAndType = prototype->getArg();
        manager.insertLocalVar(id, idAndType.first, idAndType.second);
        manager.push(CodeManager::TEXT,
            "mov %rax, -" + std::to_string(j) + "(%rbp)", true);
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
}

void pushGlobalVar(ExprAST* top)
{
    enum TypeCode { NUMBER,
        STRING,
        ARRAY,
        NONE };
    NumberAST* test1 = dynamic_cast<NumberAST*>(top);
    StringAST* test2 = dynamic_cast<StringAST*>(top);
    ArrayAST* test3 = dynamic_cast<ArrayAST*>(top);
    TypeCode notNull = (test1 != nullptr)
        ? NUMBER
        : (test2 != nullptr) ? STRING : (test3 != nullptr) ? ARRAY : NONE;
    if (notNull == NONE) {
        error("Expected Number, String, or Array but expression.");
    }
    if (notNull == NUMBER) {
        NumericData data = test1->getData();
        LiteralKind kind = test1->getKind();
        string size = (kind == LiteralKind::CHAR)
            ? ".byte"
            : (kind == LiteralKind::INT) ? ".long" : ".quad";
        manager.push(CodeManager::DATA,
            size + " " + std::to_string((kind == LiteralKind::CHAR) ? data.c : (kind == LiteralKind::INT) ? data.i : *(uint64_t*)(&data.d)),
            true);
    } else if (notNull == STRING) {
        const string& str = test2->getStr();
        manager.push(CodeManager::DATA, ".asciz \"" + str + "\"", true);
    } else if (notNull == ARRAY) {
        const vector<ExprAST*>& inits = test3->getInits();
        NumberAST* dtest1 = nullptr;
        StringAST* dtest2 = nullptr;
        for (int i = 0; i < inits.size(); ++i) {
            dtest1 = dynamic_cast<NumberAST*>(inits[i]);
            string elemsize = (dtest1 != nullptr)
                ? ((dtest1->getKind() == LiteralKind::CHAR)
                          ? ".byte"
                          : (dtest1->getKind() == LiteralKind::INT) ? ".long"
                                                                    : ".quad")
                : "e";
            if (elemsize == "e") {
                error("Expected Number");
            }
            NumericData data = dtest1->getData();
            manager.push(
                CodeManager::DATA,
                elemsize + " " + std::to_string((elemsize == ".byte") ? data.c : (elemsize == ".long") ? data.i : data.d),
                true);
        }
    }
}

void VarDefAST::codegen()
{
    auto&& nowFunc = manager.getNowFunction();

    // 지역 변수일 때
    if (!nowFunc.empty()) {
        manager.insertLocalVar(nowFunc, id, type);
        int offset = manager.getLocalVarOffset(nowFunc, id);
        if (init) {
            init->codegen();
            if (init->isIntegral()) {
                string instr(getMovInstr(type));
                string reg = (instr == "movb") ? "al" : (instr == "movl") ? "eax" : "rax";
                manager.push(
                    CodeManager::TEXT,
                    instr + " %" + reg + ", -" + std::to_string(offset) + "(%rbp)",
                    true);
            } else {
                manager.push(CodeManager::TEXT,
                    "movsd %xmm0, -" + std::to_string(offset) + "(%rbp)",
                    true);
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
}

void pushArrayString(const vector<ExprAST*>& inits)
{
    vector<string> labels;
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

void ArrayAST::codegen()
{
    TypeCode t = Parser::fromType(getType());
    string datastr;
    if (t == TypeCode::CHAR) {
        datastr = ".byte";
    } else if (t == TypeCode::INT) {
        datastr = ".long";
    } else {
        datastr = ".quad";
    }
    manager.getNextLabel();
    string arrLabel = manager.getLabel();
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
            uint64_t real = (t == TypeCode::CHAR)
                ? data.c
                : (t == TypeCode::INT) ? data.i : *(uint64_t*)(&data.d);
            manager.push(CodeManager::DATA, datastr + " " + std::to_string(real),
                true);
        } else {
            pushArrayString(inits);
            break;
        }
    }
}

void IfAST::codegen()
{
    if (cond) {
        manager.getNextLabel();
        string elseLabel = manager.getLabel();
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
}

static std::stack<std::pair<string, string>> forFlow;

void ForAST::codegen()
{
    manager.getNextLabel();
    string begin = manager.getLabel();
    manager.getNextLabel();
    string end = manager.getLabel();
    forFlow.emplace(begin, end);
    manager.push(CodeManager::TEXT, begin + ":");
    cond->codegen();
    manager.push(CodeManager::TEXT, "cmp $0, %rax", true);
    manager.push(CodeManager::TEXT, "je " + end, true);
    then->codegen();
    manager.push(CodeManager::TEXT, "jmp " + begin, true);
    manager.push(CodeManager::TEXT, end + ":");
    forFlow.pop();
}

void BreakAST::codegen()
{
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
}

void ContinueAST::codegen()
{
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
}

void ReturnAST::codegen()
{
    ret->codegen();
    manager.push(CodeManager::TEXT, "leave", true);
    manager.push(CodeManager::TEXT, "ret", true);
}

bool NumberAST::isIntegral()
{
    return kind != LiteralKind::DOUBLE;
}

bool VariableAST::isIntegral()
{
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

bool StringAST::isIntegral()
{
    return true;
}

bool BinaryExprAST::isIntegral()
{
    // 정수 전염성
    return lhs->isIntegral() || rhs->isIntegral();
}

bool UnaryExprAST::isIntegral()
{
    // 단항 정수 확정
    return true;
}

bool CallExprAST::isIntegral()
{
    if (VariableAST* casted = dynamic_cast<VariableAST*>(func)) {
        Type t;
        if (!manager.getNowFunction().empty())
            t = manager.getTypeObject(casted->getId(), true,
                manager.getNowFunction());
        else
            t = manager.getTypeObject(casted->getId());
        return t.returnType->kind != TypeKind::DOUBLE;
    }
    // 모르는 함수는 일단 int를 반환하는 것으로 간주
    return true;
}

bool PointerDerefAST::isIntegral()
{
    return ptr->isIntegral();
}

TypeCode PointerDerefAST::getType() noexcept
{
    check();
    VariableAST* p = dynamic_cast<VariableAST*>(ptr);
    bool isLocal = !manager.getNowFunction().empty();
    return isLocal ? manager.getTypeCode(p->getId(), true, manager.getNowFunction()) : manager.getTypeCode(p->getId());
}

Type PointerDerefAST::getTypeObject() const noexcept
{
    check();
    VariableAST* p = dynamic_cast<VariableAST*>(ptr);
    bool isLocal = !manager.getNowFunction().empty();
    Type t = isLocal ? manager.getTypeObject(p->getId(), true, manager.getNowFunction()) : manager.getTypeObject(p->getId());
    return t;
}

bool ArrayAST::isIntegral()
{
    return true;
}

void CodeManager::clear()
{
    data.clear();
    text.clear();
}

void CodeManager::getNextLabel()
{
    int counter = std::stoi(label.substr(5, label.size() - 5));
    label = label.substr(0, 5) + std::to_string(counter + 1);
}

string CodeManager::getLabel()
{
    return label;
}

void CodeManager::push(Section section, string code, bool isIndent)
{
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

void CodeManager::setNowFunction(const string& id)
{
    nowFunction = id;
}

void CodeManager::setNowFunction()
{
    nowFunction.clear();
}

const string& CodeManager::getNowFunction()
{
    return nowFunction;
}

void CodeManager::insertGlobalVar(const string& id, Type ty)
{
    globalVars[id] = ty;
}

bool CodeManager::isGlobalVar(const string& id)
{
    return globalVars.find(id) != globalVars.end();
}

bool CodeManager::isLocalVar(const string& funcId, const string& varId)
{
    if (funcId.empty())
        return false;
    const vector<std::tuple<string, int, Type>>& local = localVars[funcId];
    for (int i = 0; i < local.size(); ++i) {
        if (std::get<0>(local[i]) == varId)
            return true;
    }
    return false;
}

void CodeManager::insertLocalVar(const string& funcId,
    const string& varId,
    Type ty)
{
    if (nowOffset.find(funcId) == nowOffset.end()) {
        nowOffset[funcId] = 8;
    }
    localVars[funcId].push_back(std::make_tuple(varId, nowOffset[funcId], ty));
    nowOffset[funcId] += 8;
}

int CodeManager::getLocalVarOffset(const string& funcId, const string& varId)
{
    const vector<std::tuple<string, int, Type>>& local = localVars[funcId];
    for (int i = 0; i < local.size(); ++i) {
        if (std::get<0>(local[i]) == varId) {
            return std::get<1>(local[i]);
        }
    }
    std::cerr << "Cannot fount variable " << varId << '\n';
    std::exit(EXIT_FAILURE);
}

Type CodeManager::getTypeObject(const string& varId,
    bool isLocal,
    const string& funcId)
{
    if (isLocal) {
        if (localVars.find(funcId) != localVars.end()) {
            const vector<std::tuple<string, int, Type>>& local = localVars[funcId];
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
            std::cerr << "Cannot found variable " << varId << " on function "
                      << funcId << '\n';
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
            std::cerr << "Cannot found variable on global namespace : " << varId
                      << '\n';
            std::exit(EXIT_FAILURE);
        }
    }
}

TypeCode CodeManager::getTypeCode(const string& varId,
    bool isLocal,
    const string& funcId)
{
    return Parser::fromType(getTypeObject(varId, isLocal, funcId));
}

bool CodeManager::isVarIntegral(const string& varId,
    bool isLocal,
    const string& funcId)
{
    TypeCode tc = getTypeCode(varId, isLocal, funcId);
    return tc != TypeCode::DOUBLE;
}

void CodeManager::insertFunction(const string& funcId, Type t)
{
    functions[funcId] = t;
}

bool CodeManager::isFunction(const string& funcId)
{
    return functions.find(funcId) != functions.end();
}

const vector<string>& CodeManager::getDataSeg()
{
    return data;
}

const vector<string>& CodeManager::getTextSeg()
{
    return text;
}
