#ifndef __INCLUDES_HEADER__
#define __INCLUDES_HEADER__

#include <tuple>
#include <stack>
#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <initializer_list>

using std::vector;
using std::unordered_map;

using std::istringstream;
using std::stringstream;

using std::string;

using std::shared_ptr;
using std::unique_ptr;

void error(const string& message) noexcept;
void print_help(const vector<string>& opt, const bool simple = true, const bool exit = true) noexcept;

#endif