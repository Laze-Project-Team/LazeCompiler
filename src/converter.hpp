#include "errormsg.hpp"
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

void CON_convert(std::string ast, std::string targetLang, std::string fname);
std::string jsonToString(json target, std::string parentRule, const json &rule, const json &keywords, const json &config);
