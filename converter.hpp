#include "errormsg.h"
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

void CON_convert(char *ast, char *targetLang);
std::string jsonToString(json target, const json &rule, const json &keywords);
