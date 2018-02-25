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

Token* getToken(FILE* fp, buffer b, int k, Set* keywords);

Set* get_keywords();

void print_token(Token* t);

token_id token_to_id(char* token);

char* id_to_token(token_id id);

List* all_tokens(FILE* fp, buffer b, int k, Set* keywords);

#endif