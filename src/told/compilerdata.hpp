#ifndef __COMPILER_DATA_HEADER__
#define __COMPILER_DATA_HEADER__

#include "compileroption.hpp"
#include "includes.hpp"

class CompilerData final{
private:
    unordered_map<string, string> codeStrs;

private:
    CompilerData();

public:
    const unordered_map<string, string> getCodes();
    static shared_ptr<CompilerData> get(CompilerOption& opt);
};

#endif