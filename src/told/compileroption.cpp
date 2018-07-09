#include "compileroption.hpp"

CompilerOption::CompilerOption(const bool toFile, const bool isDebug,
    const bool compileForLibrary, const string& outputFile, const vector<string> inputFiles,
    const vector<string> args, const vector<string> libs)
    : toFile(toFile)
    , debug(isDebug)
    , compileForLibrary(compileForLibrary)
    , outputFile(outputFile)
    , inputFiles(inputFiles)
    , args(args)
    , libs(libs)
{
}

CompilerOption CompilerOption::parse(int argc, char** argv) noexcept
{
    int i;
    bool toFile = false;
    bool isDebug = false;
    bool compileForLibrary = false;
    string outputFile;
    vector<string> inputFiles;
    vector<string> args(argv, argv + argc);
    vector<string> libs;

    if(argc == 1){
        print_help(args);
    }

    for (i = 1; i < argc; i++) {
        if (args[i] == "-o" || args[i] == "--output") {
            if (i + 1 < argc) {
                error("Please input output file");
            }
            toFile = true;
            outputFile = args[++i];
        } else if (args[i] == "-d" || args[i] == "--debug") {
            isDebug = true;
        } else if (args[i] == "-l" || args[i] == "--library") {
            compileForLibrary = true;
        } else if (args[i] == "-i" || args[i] == "--include") {
            if (i + 1 < argc) {
                error("Please input include library file");
            }
            string buf;
            stringstream ss(args[++i]);

            while (ss >> buf)
                libs.push_back(buf);
        } else if (args[i] == "-h" || args[i] == "--help") {
            print_help(args, false);
        } else {
            inputFiles.push_back(args[i]);
        }
    }

    if (toFile && outputFile.empty()) {
        error("\nerror: Please give output file if you want write output to file.");
    }

    if (inputFiles.size() == 0) {
        error("\nPlease give input file!");
    }

    return CompilerOption(toFile, isDebug, compileForLibrary, outputFile, inputFiles, args, libs);
}

const bool CompilerOption::isOutputToFile() const noexcept
{
    return toFile;
}
const string& CompilerOption::getOutputFile() const noexcept
{
    return outputFile;
}
const vector<string>& CompilerOption::getInputFiles() const noexcept
{
    return inputFiles;
}

const vector<string>& CompilerOption::getArgs() const noexcept
{
    return args;
}

const bool CompilerOption::isDebug() const noexcept
{
    return debug;
}
const bool CompilerOption::isCompileForLibrary() const noexcept
{
    return compileForLibrary;
}
const vector<string>& CompilerOption::getLibs() const noexcept
{
    return libs;
}