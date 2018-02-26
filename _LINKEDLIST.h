#include "_LEXERDEF.h"

#ifndef _LINKEDLIST
#define _LINKEDLIST

typedef struct LNode{
	Token* ele;
	struct LNode* next;
}LNode;

struct List{
	LNode* head;
    LNode* tail;
	int size;
};
typedef struct List List;

List* create_list();

List* add_to_list(List* list, Token* t);

void print_list(List* lst);

#endif