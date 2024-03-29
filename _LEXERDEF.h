/*
    Name: Shyamal Vaderia
    ID: 2015A7PS0048P
*/

#include <stdlib.h>

#ifndef _LEXERDEF
#define _LEXERDEF

typedef char* buffer;

extern char* token_class[42];
#define NUM_TERMINAL 42

enum token_id{
ASSIGNOP,
COMMENT,
FUNID,
ID,
NUM,
RNUM,
STR,
END,
INT,
REAL,
STRING,
MATRIX,
MAIN,
SQO,
SQC,
OP,
CL,
SEMICOLON,
COMMA,
IF,
ELSE,
ENDIF,
READ,
PRINT,
FUNCTION,
PLUS,
MINUS,
MUL,
DIV,
SIZE,
AND,
OR,
NOT,
LT,
LE,
EQ,
GT,
GE,
NE,
DOLLAR, 
ERROR,
EPSILON
};

typedef enum token_id token_id;

struct token{
    token_id id;
    char* value;
    int lineNo;
}typedef Token;

#endif