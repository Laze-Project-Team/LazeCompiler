#include "frame.h"
#include "wasm_frame.h"

const int F_wordSize = 4;
extern int funcs;
int memorySize;

static F_access InFrame(int offset, Ty_ty type, bool isParam)
{
    F_access p = (F_access)checked_malloc(sizeof(*p));
    p -> kind = inFrame;
    p -> type = type;
    if(isParam)
        p -> u.index = offset;
    else
        p -> u.offset = offset;
    
    p -> isParam = isParam;
    return p;
}
static F_access InLocal(int index, Ty_ty type, bool isParam)
{
    F_access p = (F_access)checked_malloc(sizeof(*p));
    p -> kind = inLocal;
    p -> u.index = index;
    p -> type = type;
    p -> isParam = isParam;
    return p;
}
static F_access InGlobal(int index)
{
    F_access p = (F_access)checked_malloc(sizeof(*p));
    p -> kind = inGlobal;
    p -> u.index = index;
    p -> isParam = FALSE;
    return p;
}
void toInFrame(F_access access, F_frame frame)
{
    if(access -> kind != inFrame)
    {
        access -> u.offset = frame -> frameSize;
        frame -> frameSize += access -> type -> size;
        memorySize += access -> type -> size;
        access -> kind = inFrame;
    }
    else
    {
        // printf("%d address toInframe", access -> u.offset);
    }
}
F_accessList boolToF_access(F_frame f, U_boolList boolList, Ty_tyList types)
{
    // f -> locals = 0;
    F_accessList list = (F_accessList)checked_malloc(sizeof(*list));
    F_accessList retValue = list;
    list -> head =  NULL;
    list -> tail = NULL;
    int i = f ->locals;
    if(boolList != NULL && types != NULL)
    {
        for(;boolList -> tail != NULL && types -> tail != NULL; types = types -> tail, boolList = boolList -> tail)
        {
            bool isParam = TRUE;
            if(types -> head -> kind == Ty_name || types -> head -> kind == Ty_poly)
            {
                // printf("%s types -> headddddddddddddddddddddddddddddddddddddddddd\n", S_name(types -> head -> u.name.sym));
                isParam = FALSE;
                f -> locals++;
                boolList -> head = TRUE;
                // printf("%d f -> locals\n", f -> locals);
            }
            list -> head = F_allocLocal(f, boolList -> head, types -> head, FALSE, isParam);
            if(types -> head -> kind != Ty_void)
            {
                list -> head -> paramIndex = i;
                i++;
            }
            list -> tail = (F_accessList)checked_malloc(sizeof(*list));
            list = list -> tail;
            list -> head = NULL;
            list -> tail = NULL;
        }
    }
    return retValue;
}
F_frame F_newFrame(Temp_label name, U_boolList list, Ty_tyList params, bool isMethod)
{
    F_frame frame = (F_frame)checked_malloc(sizeof(*frame));
    frame -> name = name;
    if(isMethod){
    //   printf("%s %d wasm_frame.ccccccccccccccccccc\n", S_name(frame->name), isMethod);
        frame -> locals = 1;
    }
    else
        frame -> locals = 0;
    frame -> offset = memorySize;
    frame -> frameSize = 0;
    frame -> localsTypeTemp = (Ty_tyList)checked_malloc(sizeof(*(frame -> localsTypeTemp)));
    frame -> localsTypeTemp -> head = NULL;
    frame -> localsTypeTemp -> tail = NULL;
    frame -> localsType = frame -> localsTypeTemp;
    frame -> accessList = boolToF_access(frame, list, params);
    return frame;
}
Temp_label F_name(F_frame f)
{
    return f -> name;
}
F_accessList F_formals(F_frame f)
{
    return f -> accessList;
}
F_access F_allocLocal(F_frame f, bool escape, Ty_ty type, bool isLocal, bool isParam)
{
    F_access access;
    // printf("escappeeeeeeeeeeee %d %d %d %s\n", type -> kind, escape, isParam, S_name(f -> name));
    if(escape == TRUE)
    {
        if(isParam && type -> kind != Ty_void)
        {
            access = InFrame(f -> locals, type, isParam);
            f -> locals += 1;
        }
        else
        {
            access = InFrame(f -> frameSize, type, isParam);
        }
        // printf("escaped %d\n", access -> u.offset);
        if(type -> kind != Ty_array)
        {
          //printf("%d memorySize\n", memorySize);
            f -> frameSize += type -> size;
            memorySize += type -> size;
        }
    }
    else if(escape == FALSE)
    {
        access = InLocal(f -> locals, type, isParam);
        f -> locals+=1;
        // if(!(f -> localsTypeTemp ))
            // printf("debug\n");
        if(isLocal)
        {
            f -> localsTypeTemp -> head = access -> type;
            f -> localsTypeTemp -> tail = (Ty_tyList)checked_malloc(sizeof(*(f -> localsTypeTemp)));
            f -> localsTypeTemp = f -> localsTypeTemp -> tail;
            f -> localsTypeTemp -> head = NULL;
            f -> localsTypeTemp -> tail = NULL;
        }
        // printf("inLocal\n");s
    }
    return access;
}