#include "_SEMANTICDEF.h"

#ifndef _SEMANTIC
#define _SEMANTIC

Anode* get_ast( Tnode* root, Anode* parent);

void print_ast(Anode* root);

SymTree* get_sym_tree(Anode* root, SymTree* tree);

void print_sym_tree(SymTree* root);

SymTree* make_tree_node();

SymTable* create_table(char* name);

#endif