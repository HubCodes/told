#include "compilerdata.hpp"
#include "compileroption.hpp"
#include "includes.hpp"
#include "lexer.hpp"
#include "parser.hpp"

int main(int argc, char** argv)
{
    CompilerOption options = CompilerOption::parse(argc, argv);
    shared_ptr<CompilerData> datas = CompilerData::get(options);

    // vector<AST*> tree = parse(is);
    // for (auto&& ast : tree) {
    //     ast->codegen();
    //     std::cout << '\n';
    // }

    // CodeManager& m = CodeManager::get();
    // std::cout << ".section .data\n";
    // for (auto&& str : m.getDataSeg()) {
    //     std::cout << str << '\n';
    // }
    // std::cout << ".section .text\n";
    // for (auto&& str : m.getTextSeg()) {
    //     std::cout << str << '\n';
    // }
    return 0;
}

void error(const string& message) noexcept
{
    std::cerr << message << std::endl;
    std::exit(-1);
}

void print_help(const vector<string>& opt, const bool simple, const bool exit) noexcept
{
    std::cout << "Told: The native friendly language\n\n"
              << "Usage: "
              << "\t" << opt[0] << " -h --help\n"
              << "\t" << opt[0] << " [options...] source\n"
              << std::endl;
    if (!simple) {
        std::cout << "Options: \n"
                  << "\t-o, --output [file]\tset output file\n"
                  << "\t-i, --include [libfile]\tcompile with library(use space to multiple files)\n"
                  << "\t-l, --library\t\tcompile for library\n"
                  << "\t-d, --debug\t\tcompiler debugging\n"
                  << std::endl;
    }
    if (exit)
        std::exit(0);
}