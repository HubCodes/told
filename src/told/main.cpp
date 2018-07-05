#include "includes.hpp"
#include "lexer.hpp"
#include "parser.hpp"

int main(int argc, char** argv)
{
    string codeStr;
    std::fstream code(argv[1], std::ios::in);
    if (!code.is_open()) {
        std::cerr << "error: Invalid source code file.\n";
        std::exit(EXIT_FAILURE);
    }
    while (!code.eof()) {
        codeStr += code.get();
    }
    istringstream is(codeStr);
    vector<AST*> tree = parse(is);
    for (auto&& ast : tree) {
        ast->codegen();
        std::cout << '\n';
    }

    CodeManager& m = CodeManager::get();
    std::cout << ".section .data\n";
    for (auto&& str : m.getDataSeg()) {
        std::cout << str << '\n';
    }
    std::cout << ".section .text\n";
    for (auto&& str : m.getTextSeg()) {
        std::cout << str << '\n';
    }
    return 0;
}

void error(const string& message) noexcept {
    std::cerr << message << std::endl;
    std::exit(-1);
}