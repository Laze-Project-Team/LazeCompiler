#include "lexer.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include <cstring>

using json = nlohmann::json;

static std::string tokenNames[] = {"char","string","real","int",",",":",";","(",")","[","]","{","}",".","->","<-","=>","+","-","*","/","==","!=","<=","<",">=",">","&&","||","=","if","then","else","from","to","break","inttype","realtype","continue","return","type","void","nul","true","false","boolean","chartype","%","&","shorttype","function","loop","jsload","sizeof","class","private","public","protected","repeat","jsexport","id","uminus","lower_than_else"};
static std::string operators = 
"{ \
    \",\":\"^(,|、)\", \
    \":\":\"^(:|：)\", \
    \";\":\"^(;|；)\", \
    \"(\":\"^(\\\\(|（)\", \
    \")\":\"^(\\\\)|）)\", \
    \"[\":\"^(\\\\[)\", \
    \"]\":\"^(\\\\])\", \
    \"{\":\"^(\\\\{|｛)\", \
    \"}\":\"^(\\\\}|｝)\", \
    \".\":\"^(\\\\.)\", \
    \"+\":\"^(\\\\+|＋)\", \
    \"-\":\"^(\\\\-)\", \
    \"*\":\"^(\\\\*|＊)\", \
    \"/\":\"^(\\\\/|／)\", \
    \"&\":\"^(&|＆)\", \
    \"%\":\"^(%|％)\", \
    \"=>\":\"^(=\\\\>|＝＞)\", \
    \"<-\":\"^(\\\\<\\\\-)\", \
    \"->\":\"^(\\\\-\\\\>)\", \
    \"==\":\"^(==|＝＝)\", \
    \"!=\":\"^(!=|！＝)\", \
    \"<=\":\"^(\\\\<=|＜＝)\", \
    \">=\":\"^(\\\\>=|＞＝)\", \
    \"<\":\"^(\\\\<|＜)\", \
    \">\":\"^(\\\\>|＞)\", \
    \"&&\":\"^(&&|＆＆)\", \
    \"||\":\"^(\\\\|\\\\||｜)\", \
    \"=\":\"^(=|＝)\" \
}";

L_tokenList L_Lexer(const char* filename1, const char* filename2)
{
    L_tokenList tokenList;
    std::ifstream input(filename2);
    json j;
    input >> j;
    json keywords = j["tokens"]["keywords"];
    json ops = json::parse(operators);
    std::string charRegex = j["tokens"]["char"].get<std::string>();
    std::string intRegex = j["tokens"]["int"].get<std::string>();
    std::string separatorRegex = "[^" + charRegex.substr(1, charRegex.size() - 2) + intRegex.substr(1, intRegex.size() - 2) + "]";
    std::vector<std::pair<std::string, std::regex>> regexMap;
    
    for(const auto &tokenName:tokenNames){
        json obj;
        if((obj = keywords[tokenName])!=nullptr){
            std::string reg = obj.get<std::string>();
            regexMap.push_back(std::make_pair(tokenName, std::regex("^("+reg+")(?:$|"+separatorRegex+")")));
        }
        else if((obj = ops[tokenName])!=nullptr){
            std::string reg = obj.get<std::string>();
            regexMap.push_back(std::make_pair(tokenName, std::regex(reg)));
        }
        else if(tokenName == "char"){
            regexMap.push_back(std::make_pair(tokenName, std::regex("^('(...|..|.)')")));
        }
        else if(tokenName == "int"){
            std::string hexRegex = j["tokens"]["hex"].get<std::string>();
            regexMap.push_back(std::make_pair(tokenName, std::regex("^((" + intRegex + "+|" + hexRegex + "))")));
        }
        else if(tokenName == "real"){
            regexMap.push_back(std::make_pair(tokenName, std::regex("^((" + intRegex + "+\\." + intRegex + "+f?))")));
            std::cout << "^((" + intRegex + "+\\." + intRegex + "+f?))" << std::endl;
        }
        else if(tokenName == "string"){
            std::string stringRegex = "^(" + j["tokens"]["string"].get<std::string>() + ")";
            regexMap.push_back(std::make_pair(tokenName, std::regex(stringRegex)));
        }
        else if(tokenName == "id"){
            std::string idRegex = "^((" + charRegex + "[" + charRegex.substr(1, charRegex.size() - 2) + intRegex.substr(1, intRegex.size() - 2) + "]*))";
            regexMap.push_back(std::make_pair(tokenName, std::regex(idRegex)));
        }
    }
    std::ifstream programFile(filename1);
    std::string programLine;
    std::regex space("^((?: |   )+)");
    std::smatch match;
    std::size_t cursor;
    while(std::getline(programFile, programLine)){
        while(programLine.size() > 0){
            std::size_t size;
            if(std::regex_search(programLine, match, space)){
                size = match[1].length();
                cursor += size;
                programLine = programLine.substr(size);
                continue;
            }
            for(const auto &regex: regexMap){
                if(std::regex_search(programLine, match, regex.second)){
                    const char *matchCstr = match[1].str().c_str();
                    std::cout << regex.first << " ";
                    std::size_t length = 0;
                    while(*matchCstr != '\0'){
                        if(*matchCstr < 0){
                            matchCstr += 3;
                        }
                        else if(*matchCstr > 0){
                            matchCstr++;
                        }
                        length++;
                    }
                    L_token token = std::make_shared<L_token_>();
                    token -> start = cursor;
                    cursor += length;
                    token -> end = cursor;
                    token -> kind = regex.first;
                    size = match[1].length();
                    if(regex.first == "char"){
                        std::strcpy(token -> u.charr, match[2].str().c_str());
                    }
                    else if(regex.first == "int"){
                        // std::cout << match[2].str() << std::endl;
                        token -> u.intt = std::stoll(match[2].str());
                    }
                    else if(regex.first == "real"){
                        token -> u.real = std::stod(match[2].str());
                    }
                    else if(regex.first == "string"){
                        token -> u.stringg = (char *)malloc(match[2].str().size() + 1);
                        std::strcpy(token -> u.stringg, match[2].str().c_str());
                    }
                    else if(regex.first == "id"){ 
                        token -> u.id = (char *)malloc(match[2].str().size() + 1);
                        std::strcpy(token -> u.id, match[2].str().c_str());
                    }
                    tokenList.push_back(token);
                    break;
                } 
            }
            programLine = programLine.substr(size);
        }
        std::cout << std::endl;
    }
    std::cout << tokenList.size() << std::endl;
    L_token eof = std::make_shared<L_token_>();
    eof -> kind = "eof";
    tokenList.push_back(eof);
    return tokenList;
}

L_token L_delimToken(delimDataTy data){
    L_token p = std::make_shared<L_token_>();
    p -> kind = "^^^";
    p -> delimData = data;
    return p;
}