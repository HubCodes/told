#include "compilerdata.hpp"

CompilerData::CompilerData()
{
}

const unordered_map<string, string> CompilerData::getCodes() {
    return codeStrs;
}

shared_ptr<CompilerData> CompilerData::get(CompilerOption& opt)
{

    shared_ptr<CompilerData> data(new CompilerData());
    for (string name : opt.getInputFiles()) {
        string codeStr;
        std::fstream code(name, std::ios::in);
        if (!code.is_open()) {
            error("error: Invalid source code file.");
        }
        while (!code.eof()) {
            codeStr += code.get();
        }
        data->codeStrs[name] = codeStr;
    }
    return data;
}