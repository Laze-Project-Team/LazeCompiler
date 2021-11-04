#include "ppsemant.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

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