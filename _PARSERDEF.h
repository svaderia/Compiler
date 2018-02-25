#include <stdlib.h>
#include "_LEXERDEF.h"

#ifndef _PARSERDEF
#define _PARSERDEF

#define MAX_RULE_LENGTH 13
#define MAX_RULE 87
#define NUM_NONTERMINAL 45


typedef enum nt{
mainFunction,
stmtsAndFunctionDefs,
recStmtsAndFunctionDefs,
stmtOrFunctionDef,
stmt,
functionDef,
parameter_list,
type,
remainingList,
declarationStmt,
var_list,
more_ids,
assignmentStmt_type1,
assignmentStmt_type2,
leftHandSide_singleVar,
leftHandSide_listVar,
rightHandSide_type1,
rightHandSide_type2,
sizeExpression,
ifStmt,
elseStmt,
otherStmts,
ioStmt,
funCallStmt,
inputParameterList,
listVar,
arithmeticExpression,
recArithmeticExpression,
arithmeticTerm,
recArithmeticTerm,
factor,
operator_lowPrecedence,
operator_highPrecedence,
booleanExpression,
constrainedVars,
var,
matrix,
rows,
recRows,
row,
recRow,
remainingColElements,
matrixElement,
logicalOp,
relationalOp
}non_terminal;

typedef token_id terminal;

typedef union data{
    non_terminal nt;
    terminal t;
}G_Value;

typedef enum type{
    T,
    NT
}token_type;

typedef struct rule_ele{
    G_Value value;
    token_type tp;
    int length;
}G_Ele;

#endif