#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "util.h"

typedef struct Pre_preprocessor_ *Pre_preprocessor;
typedef struct Pre_argument_ *Pre_argument;

typedef struct Pre_preprocessorList_ *Pre_preprocessorList;
enum Pre_preprocessorType
{
    Pre_js,
    Pre_include,
    Pre_config
};
struct Pre_preprocessor_
{
    enum Pre_preprocessorType kind;
    union 
    {
        struct {
            Pre_argument path;
        } include;
        struct
        {
            Pre_argument name, setting;
        } config;
    } u;
};
enum Pre_argumentType
{
    Pre_string,
    Pre_module,
    Pre_num
};
struct Pre_argument_
{
    enum Pre_argumentType kind;
    union 
    {
        string str;
        string module;
        int num;
    } u;
};

struct Pre_preprocessorList_
{
    Pre_preprocessor head;
    Pre_preprocessorList tail;
};

Pre_preprocessorList Pre_PreprocessorList(Pre_preprocessor head, Pre_preprocessorList tail);

Pre_preprocessor Pre_Include(Pre_argument path);
Pre_preprocessor Pre_Config(Pre_argument name, Pre_argument setting);

Pre_argument Pre_String(string val);
Pre_argument Pre_Module(string val);
Pre_argument Pre_Num(int num);

#ifdef __cplusplus
}
#endif