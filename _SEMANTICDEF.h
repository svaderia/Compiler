#include "_LEXERDEF.h"
#include "_PARSERDEF.h"


#ifndef _SEMANTICDEF
#define _SEMANTICDEF

typedef struct astnode{
    char* name;
    Token* token;
    struct astnode* parent;
    struct astnode* sibling;
    struct astnode* child;
}Anode;

typedef struct SymTableEntry{
    token_id type;
    char* value;
    int size;
    Anode* astnode;
    int dim[2];
}SymTableEntry;

typedef struct SymTableNode{
    SymTableEntry* data;
    struct SymTableNode* next;
}SymTableNode;

typedef struct SymTable{
    char* name;
    SymTableNode* head;
    SymTableNode* tail;
    int size;
}SymTable;

typedef struct SymTree{
    SymTable* table;
    struct SymTree* child;
    struct SymTree* parent;
    struct SymTree* sibling;
}SymTree;

#endif