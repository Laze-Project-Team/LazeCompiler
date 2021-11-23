#include "ppsemant.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

std::deque<std::string> fileNames;
std::vector<int> linesInFile;

PP_cursor PP_getLinesInFile(int linePos){
    PP_cursor result;
    for(int i = 0; i < linesInFile.size(); i++){
        if(linePos - linesInFile.at(i) > 0){
            linePos -= linesInFile.at(i);
        }
        else{
            result.fileName = fileNames.at(i);
            result.lineNum = linePos;
            result.fileNum = i;
            return result;
        }
    }
    result.fileName = fileNames.back();
    result.lineNum = linePos;
    result.fileNum = linesInFile.size();
    return result;
}

const char * PP_getFilename(int index){
    return fileNames.at(index).c_str();
}

void transPPList(Pre_preprocessorList ppList, const char* file)
{
    fileNames.push_front(std::string(file));
    if(!ppList) {
        return;
    }
    for(;ppList; ppList = ppList -> tail) {
        transPP(ppList -> head, file);
    }
    return;
}

void includeFile(const char* includeStr, const char* file)
{
    fileNames.push_front(includeStr);
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
    // std::cout << lines << std::endl;
    linesInFile.push_back(lines);
    outputFile.close();
    originalFile.close();
    outputFile.close();
}
void transPP(Pre_preprocessor pp, const char* file)
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