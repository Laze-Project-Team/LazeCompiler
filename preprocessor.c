#include "preprocessor.h"

Pre_preprocessor Pre_Include(Pre_argument path)
{
    Pre_preprocessor p = checked_malloc(sizeof(*p));
    p -> kind = Pre_include;
    p -> u.include.path = path;
    return p;
}
Pre_preprocessor Pre_Config(Pre_argument name, Pre_argument setting)
{
    Pre_preprocessor p = checked_malloc(sizeof(*p));
    p -> kind = Pre_config;
    p -> u.config.name = name;
    p -> u.config.setting = setting;
    return p;
}

Pre_argument Pre_String(string val)
{
    Pre_argument p = checked_malloc(sizeof(*p));
    p -> kind = Pre_string;
    p -> u.str = val;
    return p;
}
Pre_argument Pre_Module(string val)
{
    Pre_argument p = checked_malloc(sizeof(*p));
    p -> kind = Pre_module;
    p -> u.module = val;
    return p;
}
Pre_argument Pre_Num(int num)
{
    Pre_argument p = checked_malloc(sizeof(*p));
    p -> kind = Pre_num;
    p -> u.num = num;
    return p;    
}

Pre_preprocessorList Pre_PreprocessorList(Pre_preprocessor head, Pre_preprocessorList tail)
{
    Pre_preprocessorList p = checked_malloc(sizeof(*p));
    p -> head = head;
    p -> tail = tail;
    return p;
}