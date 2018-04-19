#include "_LEXERDEF.h"
#include "_PARSERDEF.h"


#ifndef _SEMANTICDEF
#define _SEMANTICDEF

typedef struct astnode{
    char* name;
    token_type tp;
    Token* token;
    struct astnode* parent;
    struct astnode* sibling;
    struct astnode* child;
}Anode;

#endif