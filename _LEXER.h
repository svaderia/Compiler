/*
    Name: Shyamal Vaderia
    ID: 2015A7PS0048P
*/

#include <stdio.h>
#include <stdbool.h>
#include "_LEXERDEF.h"
#include "_SET.h"
#include "_LINKEDLIST.h"


#ifndef _LEXER
#define _LEXER

extern int offset;
extern int lineNo;
extern bool eof;
extern bool lexical_error;

Set* get_keywords();

List* getTokens(FILE* fp, buffer b, int k, Set* keywords);

void print_token(Token* t);

token_id token_to_id(char* token);

char* id_to_token(token_id id);

void remove_comments(FILE* fp, buffer b, int k);

#endif