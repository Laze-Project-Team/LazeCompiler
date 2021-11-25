#include "errormsg.hpp"
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

void CON_convert(std::string ast, std::string targetLang, std::string fname);
std::string jsonToString(json target, const json &rule, const json &keywords);
