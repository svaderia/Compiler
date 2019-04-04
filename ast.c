#include "_LEXERDEF.h"
#include "_PARSERDEF.h"
#include "_LEXER.h"
#include "_SEMANTICDEF.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define next_nt(child) while(child -> tp != NT){ child = child -> sibling; }
#define next_sib(nchild) while(nchild -> sibling != NULL){ nchild = nchild -> sibling; }

 
Anode* make_node(Tnode* root, char* name, Anode* parent){
    Anode* t = (Anode*) malloc(sizeof(Anode));
    t -> parent = parent;
    t -> sibling = NULL;
    t -> child = NULL;
    t -> token = root -> token;
    t -> name = (char*) malloc( (strlen(name) + 1) * sizeof(char));
    strcpy(t -> name, name);
    return t;
}

Anode* get_ast( Tnode* root, Anode* parent){
    if(root == NULL){
        return NULL;
    }

    if(root -> tp == NT){
        Tnode* child = root -> child;
        Anode* node;
        if(root -> ele.nt == mainFunction){
            node = make_node(root, "MAIN", parent);
            next_nt(child)
            node -> child = get_ast(child, node);
        }else if(root -> ele.nt == stmtsAndFunctionDefs){
            node = get_ast(child, parent);
            Anode* nb = node;
            Anode* sib = get_ast(child -> sibling, parent);
            next_sib(nb)
            nb -> sibling = sib;
        }else if(root -> ele.nt == recStmtsAndFunctionDefs){
            node = get_ast(child, parent);
        }else if( root -> ele.nt == stmtOrFunctionDef){
            node = get_ast(child, parent);
        }else if( root -> ele.nt == stmt){
            node = get_ast(child, parent);
        }else if( root -> ele.nt == functionDef){
            node = make_node(root, "FUNCTION_DEF", parent);
            next_nt(child);
            node -> child = make_node(child, "PARA_LIST", node);
            node -> child -> child = get_ast(child, node -> child);
            Anode* nchild = node -> child;
            while(child -> ele.t != FUNID){
                child = child -> sibling;
            }
            next_sib(nchild)
            nchild -> sibling = make_node(child, "FUNID", node);
            nchild = nchild -> sibling;
            child = child -> sibling;
            next_nt(child)
            nchild -> sibling = make_node(child, "PARA_LIST", node);
            nchild -> sibling -> child = get_ast(child, nchild -> sibling);
            child = child -> sibling;
            next_nt(child)
            next_sib(nchild)
            nchild -> sibling = get_ast(child, node);
        }else if( root -> ele.nt == parameter_list){
            node = get_ast(child, parent);
            Anode* nd = node;
            next_sib(nd)
            nd -> sibling = make_node(child -> sibling, "ID", parent);
            next_sib(nd)
            nd -> sibling = get_ast(child -> sibling -> sibling, parent);
        }else if(root -> ele.nt == type){
            node = make_node(child, "TYPE", parent);
        }else if( root -> ele.nt == remainingList){
            if(child -> tp == T && child -> ele.t == COMMA){
                child = child -> sibling;
                node = get_ast(child, parent);
            }else
                return NULL;
        }else if( root -> ele.nt == declarationStmt){
            node = make_node(root, "DEC_STMT", parent);
            node -> child = get_ast(child, node);
            Anode* nchild = node -> child;
            next_sib(nchild)
            child = child -> sibling;
            next_nt(child)
            nchild -> sibling = get_ast(child, node);
        }else if( root -> ele.nt == var_list){
            node = make_node(child, "ID", parent);
            node -> sibling = get_ast(child -> sibling, parent);
        }else if( root -> ele.nt == more_ids){
            if(child -> tp == T && child -> ele.t == COMMA){
                child = child -> sibling;
                node = get_ast(child, parent);
            }else
                return NULL;
        }else if( root -> ele.nt == assignmentStmt_type1){
            node = make_node(child, "ID_ASSIGN", parent);
            Anode* nchild = node -> child = get_ast(child, node);
            next_sib(nchild)
            child = child -> sibling;
            next_nt(child)
            nchild -> sibling = get_ast(child, node);
        }else if( root -> ele.nt == assignmentStmt_type2){
            node = make_node(child, "LIST_ASSIGN", parent);
            Anode* nchild = node -> child = get_ast(child, node);
            next_sib(nchild)
            child = child -> sibling;
            next_nt(child)
            nchild -> sibling = get_ast(child, node);
        }else if( root -> ele.nt == leftHandSide_singleVar){
            node = make_node(child, "ID", parent);
        }else if( root -> ele.nt == leftHandSide_listVar){
            next_nt(child)
            node = get_ast(child, parent);
        }else if( root -> ele.nt == rightHandSide_type1){
            node = get_ast(child, parent);
        }else if( root -> ele.nt == rightHandSide_type2){
            node = get_ast(child, parent);
        }else if( root -> ele.nt == sizeExpression){
            node = make_node(child -> sibling, "SIZE", parent);
        }else if( root -> ele.nt == ifStmt){
            node = make_node(child, "IF", parent);
            next_nt(child)
            Anode* nchild = node -> child = get_ast(child, node);
            child = child -> sibling;
            next_nt(child)
            next_sib(nchild)
            nchild -> sibling = get_ast(child, node);
            child = child -> sibling;
            next_nt(child)
            next_sib(nchild)
            nchild -> sibling = get_ast(child, node);
            child = child -> sibling;
            next_nt(child)
            next_sib(nchild)
            nchild -> sibling = get_ast(child, node);
        }else if( root -> ele.nt == elseStmt){
            if(child -> tp == T && child -> ele.t == EPSILON) return NULL;
            node = make_node(child, "ELSE", parent);
            next_nt(child)
            node -> child = get_ast(child, parent);
            child = child -> sibling;
            next_nt(child)
            Anode* sib = get_ast(child, parent);
            Anode* nd = node -> child;
            next_sib(nd)
            nd -> sibling = sib;
        }else if( root -> ele.nt == otherStmts){
            if(child -> tp == T && child -> ele.t == EPSILON) return NULL;
            next_nt(child)
            node = get_ast(child, parent);
            child = child -> sibling;
            next_nt(child)
            Anode* sib = get_ast(child, parent);
            Anode* nd = node;
            next_sib(nd)
            nd -> sibling = sib;
        }else if( root -> ele.nt == ioStmt){
            node = make_node(child, "IO", parent);
            node -> child = make_node(child -> sibling -> sibling , "ID", node);
        }else if( root -> ele.nt == funCallStmt){
            node = make_node(child, "FUN_CALL", parent) ;
            next_nt(child)
            node -> child = get_ast(child, node);
        }else if( root -> ele.nt == inputParameterList){
            if(child -> tp == T && child -> ele.t == EPSILON) return NULL;
            node = get_ast(child, parent);
            child = child -> sibling;
            next_nt(child)
            Anode* nd = node;
            next_sib(nd)
            nd -> sibling = get_ast(child, parent);
        }else if( root -> ele.nt == listVar){
            if(child -> tp == T && child -> ele.t == EPSILON) return NULL;
            next_nt(child)
            node = get_ast(child, parent);
        }else if( root -> ele.nt == arithmeticExpression){
            Anode* temp = get_ast(child, parent);
            node = get_ast(child -> sibling , parent);
            if(node != NULL){
                temp -> sibling = node -> child;
                temp -> parent = node;
                node -> child = temp;
            }else{
                node = temp;
            }
        }else if( root -> ele.nt == recArithmeticExpression){
            if(child -> tp == T && child -> ele.t == EPSILON) return NULL;            
            node = get_ast(child, parent);
            node -> child = get_ast(child -> sibling, node);
        }else if( root -> ele.nt == arithmeticTerm){
            Anode* temp = get_ast(child, parent);
            node = get_ast(child -> sibling , parent);
            if(node != NULL){
                temp -> sibling = node -> child;
                temp -> parent = node;
                node -> child = temp;
            }else{
                node = temp;
            }
        }else if( root -> ele.nt == recArithmeticTerm){
            if(child -> tp == T && child -> ele.t == EPSILON) return NULL;            
            node = get_ast(child, parent);
            node -> child = get_ast(child -> sibling, node);
        }else if( root -> ele.nt == factor){
            next_nt(child)
            node = get_ast(child, parent);
        }else if( root -> ele.nt == operator_lowPrecedence){
            node = make_node(child, "OP_LOW", parent);
        }else if( root -> ele.nt == operator_highPrecedence){
            node = make_node(child, "OP_HIGH", parent);
        }else if( root -> ele.nt == booleanExpression){
            if(child -> tp == T && child -> ele.t == NOT){
                node = make_node(child, "NOT", parent);
                next_nt(child)
                node -> child = get_ast(child, node);
            }
            next_nt(child)
            child = child -> sibling;
            next_nt(child)
            node = get_ast(child, parent);
            child = root -> child;
            next_nt(child)
            Anode* nchild = node -> child = get_ast(child, node);
            child = child -> sibling;
            next_nt(child)
            child = child -> sibling;
            next_nt(child)
            next_sib(nchild)
            nchild -> sibling = get_ast(child, node);
        }else if( root -> ele.nt == constrainedVars){
            if(child -> tp == T && child -> ele.t == ID){
                node = make_node(child, "CONST_VAR", parent);
                next_nt(child)
                node -> child = get_ast(child, node);
            }else
                node = make_node(child, "CONST_VAR", parent);
        }else if( root -> ele.nt == logicalOp){
            node = make_node(child, "LOG_OP", parent);
        }else if( root -> ele.nt == relationalOp){
            node = make_node(child, "RELATIONAL_OP", parent);
        }else if( root -> ele.nt == var){
            if(child -> tp == T && child -> ele.t == ID){
                node = make_node(child, "ID", parent);
                next_nt(child)
                node -> child = get_ast(child, node);
            }else if( child -> tp == T && (child -> ele.t == NUM || child -> ele.t == STR || child -> ele.t == RNUM)){
                node = make_node(child, "VAR", parent);
            }else{
                node = get_ast(child, parent);
            }
        }else if( root -> ele.nt == matrixElement){
            if(child -> tp == T && child -> ele.t == EPSILON) return NULL;            
            node = make_node(child -> sibling, "MATRIX_ELE_NUM", parent) ;
            node -> sibling = make_node(child -> sibling -> sibling -> sibling, "MATRIX_ELE_NUM", parent);
        }else if( root -> ele.nt == matrix){
            node = make_node(child, "MATRIX", parent);
            next_nt(child)
            node -> child = get_ast(child, node);
        }else if( root -> ele.nt == rows){
            node = make_node(child, "ROW", parent);
            node -> child = get_ast(child, node);
            child = child -> sibling;
            next_nt(child)
            node -> sibling = get_ast(child, parent);
        }else if( root -> ele.nt == recRows){
            if(child -> tp == T && child -> ele.t == EPSILON) return NULL;
            next_nt(child)
            node = get_ast(child, parent);
        }else if( root -> ele.nt == row){
            node = make_node(child, "NUM", parent);
            next_nt(child)
            node -> sibling = get_ast(child, parent);
        }else if( root -> ele.nt == recRow){
            node = get_ast(child, parent);
        }else if( root -> ele.nt == remainingColElements){
            if(child -> tp == T && child -> ele.t == EPSILON) return NULL;
            node = make_node(child -> sibling, "NUM", parent);
            next_nt(child)
            node -> sibling = get_ast(child, parent);
        }
        return node;
    }
    return NULL;
}

void print_ast(Anode* root){
    if(root == NULL) return;
    if(root -> parent == NULL){
        printf("Name:: %-16s Parent:: NULL, Token:: NULL\n", root -> name);
    }else{
        if (root -> token != NULL){
            printf("Name:: %-16s Parent:: %-16s Token:: %-16s TokenType: %-16s\n", root -> name, root -> parent -> name, root -> token -> value, id_to_token(root -> token -> id));
        }else{
            printf("Name:: %-16s Parent:: %-16s Token:: NULL\n", root -> name, root -> parent -> name);
        }
    }
    print_ast(root -> child);
    if(root -> child == NULL)
        return;
    Anode* pt = root -> child;
    pt = pt -> sibling;
    while(pt != NULL){
        print_ast(pt);
        pt = pt -> sibling;
    }
}