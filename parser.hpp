#pragma once
#include "absyn.h"
#include "lexer.hpp"
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <deque>
#include <set>
#include <vector>
#include <list>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <regex> 
#include <sstream>
#include <cstring>

using grammarListTy = std::unordered_map<std::string, std::deque<std::string>>;
using grammarTy = std::deque<std::string>;
using itemSetTy = std::vector<std::pair<std::string, std::deque<std::string>>>;
using firstSetTy = std::map<std::string, std::set<std::string>>;
using tokensTy = std::set<std::string>;
using tableTy = std::map<std::string, std::vector<std::string>>;

void P_generateGrammarList(const std::string inputFname, grammarListTy &grammarList, grammarListTy &originalGrammarList);
tableTy P_generateParseTable(const grammarListTy &grammarList, const grammarListTy &originalGrammarListTy);
void P_generateParseFile(const std::string inputFname, const std::string outputFname);
A_decList P_parse(L_tokenList list, const char *filename1);
A_decList P_parseWithFile(L_tokenList list, const std::string filename1, const std::string parserFile);