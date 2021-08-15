#include "ppsemant.h"
#include <stdlib.h>
#include <string.h>

void transPPList(Pre_preprocessorList ppList, string file)
{
    if(!ppList)
    {
        return;
    }
    for(;ppList != NULL; ppList = ppList -> tail)
    {
        transPP(ppList -> head, file);
    }
    return;
}

void includeFile(string includeStr, string file)
{
    int length = 0, mainLength = 0;
    char *buffer = NULL;
    FILE *include = fopen(includeStr, "rw");
    FILE *mainFile = fopen(file, "r");
    if(include)
    {
        length = getFileSize(include);
        buffer = checked_malloc(length + 1);
        fread(buffer, 1, length + 1, include);
        buffer[length] = '\0';
        // printf("%d %d", length, strlen(buffer));
        buffer = concat(buffer, "\n");
        
        string result, mainStr;
        if(mainFile)
        {
            mainLength = getFileSize(mainFile);
            result = checked_malloc(length + mainLength + 1);
            mainStr = checked_malloc(mainLength);
            fread(mainStr, 1, mainLength + 2, mainFile);
            mainStr[mainLength] = '\0';
            // printf("\n\n%s\n", mainStr);
            result = concat(buffer, mainStr);
            fclose(mainFile);
            mainFile = fopen(file, "w");
            fputs(result, mainFile);
        }
    }
    else
    {
        EM_error(0, "No file called %s", includeStr);
    }
    // free(buffer);
    fclose(include);
    fclose(mainFile);
}
void transPP(Pre_preprocessor pp, string file)
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