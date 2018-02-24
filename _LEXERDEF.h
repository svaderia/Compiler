#include <stdlib.h>

#ifndef _LEXERDEF
#define _LEXERDEF

typedef char* buffer;

extern char* token_class[41];

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
ENDFILE, 
ERROR
};

typedef enum token_id token_id;

struct token{
    token_id id;
    char* value;
    int lineNo;
}typedef Token;

#endif