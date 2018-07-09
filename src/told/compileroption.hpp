#ifndef __COMIPLER_OPTION_HEADER__
#define __COMIPLER_OPTION_HEADER__

#include "includes.hpp"

class CompilerOption final {
private:
    CompilerOption(const bool toFile, const bool isDebug, const bool compileForLibrary,
        const string& outputFile, const vector<string> inputFiles,
        const vector<string> args, const vector<string> libs);

public:
    static CompilerOption parse(int argc, char** argv) noexcept;

    const bool isOutputToFile() const noexcept;
    const bool isDebug() const noexcept;
    const bool isCompileForLibrary() const noexcept;
    const string& getOutputFile() const noexcept;
    const vector<string>& getInputFiles() const noexcept;
    const vector<string>& getArgs() const noexcept;
    const vector<string>& getLibs() const noexcept;

private:
    bool toFile;
    bool debug;
    bool compileForLibrary;
    string outputFile;
    vector<string> inputFiles;
    vector<string> args;
    vector<string> libs;
};

#endif