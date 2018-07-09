#ifndef __COMPILER_DATA_HEADER__
#define __COMPILER_DATA_HEADER__

#include "compileroption.hpp"
#include "includes.hpp"

class CompilerData {
private:
    unordered_map<string, string> codeStrs;

private:
    CompilerData();

public:
    static shared_ptr<CompilerData> get(CompilerOption& opt);
};

#endif