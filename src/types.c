/*
 * types.c - 
 *
 * All types and functions declared in this header file begin with "Ty_"
 * Linked list types end with "..list"
 */

#include <stdio.h>
#include "util.h"
#include "symbol.h"
#include "types.h"

static struct Ty_ty_ tynil = {Ty_nil, 0};
Ty_ty Ty_Nil(void) {return &tynil;}

static struct Ty_ty_ tyint = {Ty_int, 8};
Ty_ty Ty_Int(void) {return &tyint;}

static struct Ty_ty_ tyshort = {Ty_short, 4};
Ty_ty Ty_Short(void) {return &tyshort;}

static struct Ty_ty_ tyreal = {Ty_real, 8};
Ty_ty Ty_Real(void) {return &tyreal;}

static struct Ty_ty_ tybool = {Ty_bool, 4};
Ty_ty Ty_Bool(void) {return &tybool;}

static struct Ty_ty_ tystring = {Ty_string, 4};
Ty_ty Ty_String(void) {return &tystring;}

static struct Ty_ty_ tyvoid = {Ty_void, 0};
Ty_ty Ty_Void(void) {return &tyvoid;}

static struct Ty_ty_ tychar = {Ty_char, 4};
Ty_ty Ty_Char(void) {return &tychar;}

Ty_ty Ty_Record(Ty_fieldList fields)
{Ty_ty p = (Ty_ty)checked_malloc(sizeof(*p));
 p->kind=Ty_record;
 p->u.record=fields;
 return p;
}

Ty_ty Ty_Array(Ty_ty ty, int size)
{
  Ty_ty p = (Ty_ty)checked_malloc(sizeof(*p));
  p->kind=Ty_array;
  p -> size = ty -> size * (size);
  // printf("%d size array\n", p -> size);
  p->u.array.type=ty;
  p -> u.array.size = size;
  return p;
}

Ty_ty Ty_Name(S_symbol sym, int classSize)
{Ty_ty p = (Ty_ty)checked_malloc(sizeof(*p));
 p->kind=Ty_name;
 p -> size = classSize;
 p->u.name.sym=sym;
 return p;
}

Ty_ty Ty_Pointer(Ty_ty ty)
{
  Ty_ty p = (Ty_ty)checked_malloc(sizeof(*p));
  p -> kind = Ty_pointer;
  p -> size = 4;
  p -> u.pointer = ty;
  return p;
}

Ty_ty Ty_Poly(S_symbol name, Ty_ty typeParam, int size)
{
  Ty_ty p = (Ty_ty)checked_malloc(sizeof(*p));
  p -> kind = Ty_poly;
  p -> size = size;
  p -> u.poly.name = name;
  p -> u.poly.typeParam = typeParam;
  return p;
}

Ty_ty Ty_Func(Ty_tyList params, Ty_ty result, int typeIndex)
{
  Ty_ty p = (Ty_ty)checked_malloc(sizeof(*p));
  p -> kind = Ty_func;
  p -> size = 4;
  p -> u.func.params = params;
  p -> u.func.result = result;
  p -> u.func.typeIndex = typeIndex;
  return p;
}

Ty_tyList Ty_TyList(Ty_ty head, Ty_tyList tail)
{Ty_tyList p = (Ty_tyList)checked_malloc(sizeof(*p));
 p->head=head;
 p->tail=tail;
 return p;
}

Ty_field Ty_Field(S_symbol name, Ty_ty ty)
{Ty_field p = (Ty_field)checked_malloc(sizeof(*p));
 p->name=name;
 p->ty=ty;
 return p;
}

Ty_fieldList Ty_FieldList(Ty_field head, Ty_fieldList tail)
{Ty_fieldList p = (Ty_fieldList)checked_malloc(sizeof(*p));
 p->head=head;
 p->tail=tail;
 return p;
}

Ty_member Ty_Member(int offset, Ty_ty ty, Ty_accessSpecifier specifier)
{
  Ty_member p = (Ty_member)checked_malloc(sizeof(*p));
  p -> offset = offset;
  p -> ty = ty;
  p -> accessSpecifier = specifier;
  return p;
}

Ty_memberList Ty_MemberList(Ty_member head, Ty_memberList tail)
{
  Ty_memberList p = (Ty_memberList)checked_malloc(sizeof(*p));
  p -> head = head;
  p -> tail = tail;
  return p;
}

/* printing functions - used for debugging */
static char str_ty[][12] = {
   "ty_record", "ty_nil", "ty_int", "ty_string", 
   "ty_array", "ty_name", "ty_void"};

/* This will infinite loop on mutually recursive types */
void Ty_print(Ty_ty t)
{
  if (t == NULL) printf("null");
  else { printf("%s", str_ty[t->kind]);
         if (t->kind == Ty_name) {
	 printf(", %s", S_name(t->u.name.sym)); }
       }
}

void TyList_print(Ty_tyList list)
{
  if (list == NULL) printf("null");
  else {
  //printf("TyList( ");
    Ty_print(list->head);
  //printf(", ");
    TyList_print(list->tail);
  //printf(")");
  }
}

