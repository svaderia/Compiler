/*
    Name: Shyamal Vaderia
    ID: 2015A7PS0048P
*/

#include <stdlib.h>
#include "_LEXERDEF.h"
#include "_PARSERDEF.h"

#ifndef _STACK
#define _STACK

typedef struct node{ 
    G_Value ele;
    token_type tp;
    struct node* next;
}Node;

typedef struct stack{
    Node* head;
    int size;
}Stack;

Stack* initialize_stack();

Stack* push_stack(Stack* s, G_Ele r );

Stack* pop_stack(Stack* s);

Node* top_stack(Stack* s);

Stack* push_rule(Stack* s, G_Ele G[][MAX_RULE_LENGTH], int i);

#endif