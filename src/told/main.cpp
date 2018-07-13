#include "codegenerator.hpp"
#include "compilerdata.hpp"
#include "compileroption.hpp"
#include "includes.hpp"
#include "lexer.hpp"
#include "parser.hpp"

int main(int argc, char** argv)
{
    CompilerOption options = CompilerOption::parse(argc, argv);
    shared_ptr<CompilerData> datas = CompilerData::get(options);

    vector<ASTData> ps = Parser::start_parse(options, datas);

    CodeGenerator generator(options);
    generator.generateCode(ps);

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
                  << "\t-i, --include [lib]\tinclude library folder or file (use space to multiple files)\n"
                  << "\t-l, --library\t\tcompile for library\n"
                  << "\t-p, --print\t\tprint assembly code to standard output\n"
                  << "\t-d, --debug\t\tcompiler debugging\n"
                  << std::endl;
    }
    if (exit)
        std::exit(0);
}