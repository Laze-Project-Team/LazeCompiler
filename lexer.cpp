#include "lexer.hpp"

using json = nlohmann::json;

static std::deque<std::string> tokenNames;
std::deque<std::string> L_getTokenNames(){
    return tokenNames;
}
// static std::string tokenNames[] = {"char","string","real","hex","int",",",":",";","(",")","[","]","{","}",".","->","<-","=>","+","-","*","/","==","!=","<=","<",">=",">","&&","||","=","if","then","else","from","to","break","inttype","realtype","continue","return","type","void","nul","true","false","boolean","chartype","%","&","shorttype","function","loop","jsload","sizeof","class","private","public","protected","repeat","jsexport","id","uminus","lower_than_else"};
static std::map<std::string, unsigned> operatorsPrecedence;
std::map<std::string, unsigned> L_getOperators(){
    return operatorsPrecedence;
}

std::vector<std::vector<int>> lettersInLines;

L_errorPos L_getErrorPos(int cursorPos){
    L_errorPos result;
    std::cout << cursorPos << std::endl;
    for(int i = 0; i < lettersInLines.size(); i++){
        for(int j = 0; j < lettersInLines.at(i).size(); j++){
            if(cursorPos - lettersInLines.at(i).at(j) > 0){
                cursorPos -= lettersInLines.at(i).at(j);
            }
            else{
                result.fileNum = i;
                result.fileName = PP_getFilename(i);
                // std::cout << i << " line55"<< std::endl;
                result.lineNum = PP_getLinesInFile(j + 1).lineNum;
                result.columnNum = cursorPos + 1;
                return result;
                break;
            }
        }
    }
}

std::vector<std::pair<std::string, std::regex>> L_genTokenNames(const std::string &inputFname){
    std::ifstream input(inputFname);
    json j;
    input >> j;
    json keywords = j["tokens"]["keywords"];
    json ops = j["tokens"]["ops"];
    std::string charRegex = j["tokens"]["char"].get<std::string>();
    std::string intRegex = j["tokens"]["int"].get<std::string>();
    std::string separatorRegex = "[^" + charRegex.substr(1, charRegex.size() - 2) + intRegex.substr(1, intRegex.size() - 2) + "]";
    std::vector<std::pair<std::string, std::regex>> regexMap;
    regexMap.push_back(std::make_pair("comment", std::regex(j["tokens"]["comment"].get<std::string>())));
    for(const auto &item: keywords.items()){
        std::string reg = item.value().get<std::string>();
        regexMap.push_back(std::make_pair(item.key(), std::regex("^("+reg+")(?:$|"+separatorRegex+")")));
        tokenNames.push_back(item.key());
    }
    for(const auto &item: ops.items()){
        std::string reg = item.value()["regex"];
        // std::cout << reg << std::endl;
        regexMap.push_back(std::make_pair(item.value()["name"], std::regex(reg)));
        tokenNames.push_back(item.value()["name"]);
        operatorsPrecedence[item.value()["name"]] = item.value()["precedence"];
    }
    // token hardcode
    {
        regexMap.push_back(std::make_pair("char", std::regex("^('(...|..|.)')")));
        tokenNames.push_back("char");

        regexMap.push_back(std::make_pair("real", std::regex("^((" + intRegex + "+\\." + intRegex + "+f?))")));
        tokenNames.push_back("real");

        std::string stringRegex = "^(" + j["tokens"]["string"].get<std::string>() + ")";
        regexMap.push_back(std::make_pair("string", std::regex(stringRegex)));
        tokenNames.push_back("string");

        std::string hexRegex = j["tokens"]["hex"].get<std::string>();
        regexMap.push_back(std::make_pair("int", std::regex("^((" + hexRegex + "|" + intRegex + "+))")));
        tokenNames.push_back("int");

        std::string idRegex = "^((" + charRegex + "[" + charRegex.substr(1, charRegex.size() - 2) + intRegex.substr(1, intRegex.size() - 2) + "]*))";
        regexMap.push_back(std::make_pair("id", std::regex(idRegex)));
        tokenNames.push_back("id");

        tokenNames.push_back("eof");
        tokenNames.push_back("");
    }
    return regexMap;
}

L_tokenList L_Lexer(const char* filename1, const char* filename2)
{
    L_tokenList tokenList;
    std::vector<std::pair<std::string, std::regex>> regexMap = L_genTokenNames(filename2);

    std::ifstream programFile(filename1);
    std::string programLine;
    std::regex space("^((?: |   )+)");
    std::smatch match;
    std::size_t cursor = 0;
    std::size_t linePos = 0;
    while(std::getline(programFile, programLine)){
        linePos += 1;
        std::size_t cursorBefore = cursor;
        while(programLine.size() > 0){
            std::size_t size = 0;
            // std::cout << programLine << std::endl;
            if(std::regex_search(programLine, match, space)){
                size = match[1].length();
                cursor += size;
                programLine = programLine.substr(size);
                continue;
            }
            for(const auto &regex: regexMap){
                if(std::regex_search(programLine, match, regex.second)){
                    const char *matchCstr = match[1].str().c_str();
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
                    // std::cout << length << std::endl;
                    L_token token = std::make_shared<L_token_>();
                    token -> start = cursor;
                    cursor += length;
                    // std::cout << cursor << std::endl;
                    token -> end = cursor;
                    token -> kind = regex.first;
                    size = match[1].length();
                    if(regex.first == "char"){
                        std::strcpy(token -> u.charr, match[2].str().c_str());
                    }
                    else if(regex.first == "int"){
                        if(std::stoll(match[2].str()) == 0 && match[2].str() != "0"){
                            token -> u.intt = std::stoul(match[2].str(), nullptr, 16);
                        }
                        else{
                            token -> u.intt = std::stoll(match[2].str());
                        }
                        // std::cout << match[2].str() << ": " << token -> u.intt << std::endl;
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
                    if(token -> kind != "comment"){
                        tokenList.push_back(token);
                    }
                    break;
                }
            }
            programLine = programLine.substr(size);
        }
        if(PP_getLinesInFile(linePos).fileNum == lettersInLines.size()){
            lettersInLines.push_back(std::vector<int>());
        }
        // std::cout << "newline" << PP_getLinesInFile(linePos).fileNum << std::endl;
        lettersInLines.at(PP_getLinesInFile(linePos).fileNum).push_back(cursor - cursorBefore);
    }
    // std::cout << tokenList.size() << std::endl;
    L_token eof = std::make_shared<L_token_>();
    eof -> kind = "eof";
    eof -> start = cursor;
    eof -> end = cursor;
    tokenList.push_back(eof);
    return tokenList;
}

L_token L_delimToken(delimDataTy data){
    L_token p = std::make_shared<L_token_>();
    p -> kind = "^^^";
    p -> delimData = data;
    return p;
}