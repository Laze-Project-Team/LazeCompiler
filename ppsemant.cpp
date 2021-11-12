#include "ppsemant.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

std::vector<int> linesInFile;

PP_cursor PP_getLinesInFile(int linePos){
    PP_cursor result;
    for(int i = 0; i < linesInFile.size(); i++){
        if(linePos - linesInFile.at(i) > 0){
            linePos -= linesInFile.at(i);
        }
        else{
            result.fileNum = i;
            result.lineNum = linePos;
            return result;
        }
    }
    result.fileNum = linesInFile.size();
    result.lineNum = linePos;
    return result;
}

void transPPList(Pre_preprocessorList ppList, char* file)
{
    if(!ppList) {
        return;
    }
    for(;ppList; ppList = ppList -> tail) {
        transPP(ppList -> head, file);
    }
    return;
}

void includeFile(char* includeStr, char* file)
{
    std::ifstream originalFile(file, std::ifstream::in);
    std::ifstream inputFile(includeStr, std::ifstream::in);
    std::stringstream originalStrStr, inputStrStr;
    originalStrStr << originalFile.rdbuf();
    inputStrStr << inputFile.rdbuf();
    std::string originalStr = originalStrStr.str();
    std::string inputStr = inputStrStr.str();
    std::ofstream outputFile(file, std::ofstream::out);
    outputFile << inputStr << "\n" << originalStr;
    int lines = std::count(inputStr.begin(), inputStr.end(), '\n') + 1;
    std::cout << lines << std::endl;
    linesInFile.push_back(lines);
    outputFile.close();
    originalFile.close();
    outputFile.close();
}
void transPP(Pre_preprocessor pp, char* file)
{
    if(pp)
    {
        switch(pp -> kind)
        {
            case Pre_include:
            {
              //printf("Pre_include\n");
                switch(pp -> u.include.path -> kind)
                {
                    case Pre_module:
                    {
                        break;
                    }
                    case Pre_string:
                    {
                        includeFile(pp -> u.include.path -> u.str, file);
                        break;
                    }
                }
                break;
            }
            case Pre_config:
            {
              //printf("Pre_config\n");
                break;
            }
        }
    }
    else
    {
        return;
    }
}