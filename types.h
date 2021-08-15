/*
 * types.h - 
 *
 * All types and functions declared in this header file begin with "Ty_"
 * Linked list types end with "..list"
 */
#pragma once
#include "symbol.h"

typedef struct Ty_ty_ *Ty_ty;
typedef struct Ty_tyList_ *Ty_tyList;
typedef struct Ty_field_ *Ty_field;
typedef struct Ty_fieldList_ *Ty_fieldList;
typedef struct Ty_member_ *Ty_member;
typedef struct Ty_memberList_ *Ty_memberList;

struct Ty_ty_ {enum {Ty_record, Ty_nil, Ty_int, Ty_short, Ty_string, Ty_array,
		       Ty_name, Ty_void, Ty_real, Ty_bool, Ty_char, Ty_pointer, Ty_poly} kind;
			int size;
	       	union 
			{
				Ty_fieldList record;
				//saves array size and type
				struct {int size; Ty_ty type;} array;
				struct {S_symbol sym;} name;
				//The type the pointer points to.
			  	Ty_ty pointer;
				struct {S_symbol name; Ty_ty typeParam;} poly;
		    } u;
	     };

struct Ty_tyList_ {Ty_ty head; Ty_tyList tail;};
struct Ty_field_ {S_symbol name; Ty_ty ty;};
struct Ty_fieldList_ {Ty_field head; Ty_fieldList tail;};

typedef enum {Ty_public, Ty_private, Ty_protected} Ty_accessSpecifier;

struct Ty_member_ {int offset; Ty_ty ty; Ty_accessSpecifier accessSpecifier;};
struct Ty_memberList_ {Ty_member head; Ty_memberList tail;};

Ty_ty Ty_Nil(void);
Ty_ty Ty_Int(void);
Ty_ty Ty_Short(void);
Ty_ty Ty_String(void);
Ty_ty Ty_Real(void);
Ty_ty Ty_Bool(void);
Ty_ty Ty_Void(void);
Ty_ty Ty_Char(void);

Ty_ty Ty_Record(Ty_fieldList fields);
Ty_ty Ty_Array(Ty_ty ty, int size);
Ty_ty Ty_Name(S_symbol sym, int classSize);
Ty_ty Ty_Pointer(Ty_ty ty);
Ty_ty Ty_Poly(S_symbol name, Ty_ty ty, int size);

Ty_tyList Ty_TyList(Ty_ty head, Ty_tyList tail);
Ty_field Ty_Field(S_symbol name, Ty_ty ty);
Ty_fieldList Ty_FieldList(Ty_field head, Ty_fieldList tail);
Ty_member Ty_Member(int offset, Ty_ty ty, Ty_accessSpecifier specifier);
Ty_memberList Ty_MemberList(Ty_member head, Ty_memberList tail);


void Ty_print(Ty_ty t);
void TyList_print(Ty_tyList list);
