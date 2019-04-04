#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "_SEMANTICDEF.h"
#include "_LEXERDEF.h"
#include "_LEXER.h"

#define next_sib(nchild) while(nchild -> sibling != NULL){ nchild = nchild -> sibling; }

int semantic_error;

SymTree* get_sym_tree(Anode* root, SymTree* tree);

SymTable* create_table(char* name){
    SymTable* temp = (SymTable*) malloc(sizeof(SymTable));
    if(temp == NULL){
        return NULL;
    }else{
        temp -> head = NULL;
        temp -> tail = NULL;
        temp -> size = 0;
        temp -> name = (char*) malloc((strlen(name) + 1) * sizeof(char));
        strcpy(temp -> name, name);
        return temp;
    }
}

SymTableNode* create_table_node(SymTableEntry* data){
    SymTableNode* temp = (SymTableNode*) malloc( sizeof(SymTableNode));
    if (temp == NULL){
        return NULL;
    }else{
        temp -> data = data;
        temp -> next = NULL;
        return temp;
    }
}

int get_size(token_id type){
    if(type == INT) return 2;
    else if(type == REAL) return 4;
    else return 0;
}

SymTable* add_to_table(SymTable* table, token_id type, char* value, Anode* astnode){
    SymTableEntry* data = (SymTableEntry*) malloc(sizeof(SymTableEntry));
    if(data == NULL){
        return NULL;
    }else{
        data -> type = type;
        data -> value = value;
        data -> size = get_size(type);
        data -> astnode = astnode;
        data -> dim[0] = 0;
        data -> dim[1] = 0; 
    }
    SymTableNode* node = create_table_node(data);

    if(table -> size == 0){
        table -> head = node;
        table -> tail = node;
    }else{
        table -> tail -> next = node;
        table -> tail = node;
    }
    (table -> size)++;
    return table;
}

SymTableEntry* get_ele_table(SymTree* tree, char* value){
    SymTree* pt = tree;
    SymTableNode* temp;
    while (pt != NULL){
        temp = pt -> table -> head;
        while(temp != NULL){
            if(strcmp(temp -> data -> value, value) == 0) return temp -> data;
            temp = temp -> next;
        }
        pt = pt -> parent;
    }
    return NULL;
}

void print_sym_data(SymTableEntry* data, int offset, char* parent, char* scope, int depth){
    printf("%-16s %-16s %-16d %-16s %-16s %-16d %-16d \n", 
    data -> value, scope, depth, parent, id_to_token(data -> type), data -> size, offset);
}

void print_table(SymTable* table, char* parent, int depth){
    SymTableNode* temp = table -> head;
    int base = 0;
    while(temp != NULL){
        print_sym_data(temp -> data, base, parent, table -> name, depth);
        base += temp -> data -> size;
        temp = temp -> next;
    }
    printf("\n");
}

SymTree* make_tree_node(){
    SymTree* temp = (SymTree*) malloc(sizeof(SymTree));
    temp -> table = NULL;
    temp -> parent = NULL;
    temp -> child = NULL;
    temp -> sibling = NULL;
    return temp;
}

void handle_dec_stmt(Anode* root, SymTree* tree){
    Anode* type = root -> child;
    SymTable* table = tree -> table;
    Anode* id = type -> sibling;
    while(id != NULL){
        if( get_ele_table(tree, id -> token -> value) != NULL){
            semantic_error = 1;
            printf("Semantic Error : < %s > is already declared at Line < %d >\n", id -> token -> value, id -> token -> lineNo);
        }else{
            add_to_table(table, type -> token -> id , id -> token -> value, id);
        }
        id = id -> sibling;
    }
}

token_id handle_ae(Anode* root, SymTree* tree){
    SymTable* table = tree -> table;
    SymTableEntry* data;

    if(strcmp(root -> name, "VAR") == 0){
        if(root -> token -> id == NUM) return INT;
        else if(root -> token -> id == RNUM) return REAL;
        else if(root -> token -> id == STR) return STRING;
    }else if( strcmp(root -> name, "MATRIX") == 0){
        return MATRIX;
    }else if( strcmp(root -> name, "ID") == 0){
        if(root -> child == NULL){
            if((data = get_ele_table(tree, root -> token -> value)) == NULL){
                semantic_error = 1;
                printf("Semantic Error : < %s > is not declared at Line < %d >\n", root -> token -> value, root -> token -> lineNo);
            }else{
                return data -> type;
            }
        }else{
            return INT;
        }
    }else if( strcmp(root -> name, "OP_LOW") == 0 || strcmp(root -> name, "OP_HIGH") == 0){
        token_id lhs = handle_ae(root -> child, tree), rhs = handle_ae(root -> child -> sibling, tree);
        switch(root -> token -> id){
            case PLUS:
                if( lhs == rhs){
                    if(lhs == MATRIX ){
                        SymTableEntry* ldata = get_ele_table(tree, root -> child -> token -> value);
                        SymTableEntry* rdata = get_ele_table(tree, root -> child -> sibling -> token -> value);
                        if(rdata -> size != ldata -> size){
                            semantic_error = 1;
                            printf("Semantic Error : < %s > and < %s > does not have compatable SIZE at Line < %d >\n", root -> child -> token -> value, root -> child -> sibling -> token -> value, root -> token -> lineNo);
                        }
                    }
                    return lhs;
                }else{
                    semantic_error = 1;
                    printf("Semantic Error : < %s > and < %s > are not compatible with < %s > operation at Line < %d >\n", root -> child -> token -> value, root -> child -> sibling -> token -> value, id_to_token(root -> token -> id) ,root -> child -> token -> lineNo);
                }
                break;
            case MINUS:
                if(lhs == rhs){
                    if(lhs == STRING){
                        semantic_error = 1;
                        printf("Semantic Error : < %s > is STRING type and is not compatible with < %s > operation at Line < %d >\n", root -> child -> token -> value, id_to_token(root -> token -> id) ,root -> child -> token -> lineNo);
                    }
                    if(lhs == MATRIX ){
                        SymTableEntry* ldata = get_ele_table(tree, root -> child -> token -> value);
                        SymTableEntry* rdata = get_ele_table(tree, root -> child -> sibling -> token -> value);
                        if(rdata -> size != ldata -> size){
                            semantic_error = 1;
                            printf("Semantic Error : < %s > and < %s > does not have compatable size at Line < %d >\n", root -> child -> token -> value, root -> child -> sibling -> token -> value, root -> token -> lineNo);
                        }
                    }
                    return lhs;
                }else{
                    semantic_error = 1;
                    printf("Semantic Error : < %s > and < %s > are not compatible with < %s > operation at Line < %d >\n", root -> child -> token -> value, root -> child -> sibling -> token -> value, id_to_token(root -> token -> id) ,root -> child -> token -> lineNo);
                }
                break;
            
            case DIV:
            case MUL:
                if(lhs == rhs){
                    if(lhs == STRING || lhs == MATRIX){
                        semantic_error = 1;
                        printf("Semantic Error : < %s > is < %s > type and is not compatible with < %s > operation at Line < %d >\n", root -> child -> token -> value, id_to_token(lhs), id_to_token(root -> token -> id) ,root -> child -> token -> lineNo);
                    }
                    return lhs;
                }else{
                    semantic_error = 1;
                    printf("Semantic Error : < %s > and < %s > are not compatible with < %s > operation at Line < %d >\n", root -> child -> token -> value, root -> child -> sibling -> token -> value, id_to_token(root -> token -> id) ,root -> child -> token -> lineNo);
                }
                break;
        }
    }
}

token_id get_type(Anode* root){
    if(strcmp(root -> name, "VAR") == 0){
    if(root -> token -> id == NUM) return INT;
    else if(root -> token -> id == RNUM) return REAL;
    else if(root -> token -> id == STR) return STRING;
    }else if( strcmp(root -> name, "MATRIX") == 0){
        return MATRIX;
    }else if( strcmp(root -> name, "ID") == 0){
        if(root -> child == NULL){
            return root -> token  -> id;
        }else{
            return INT;
        }
    }
}
void handle_ass1_stmt(Anode* root, SymTree* tree){
    Anode* lhs = root -> child;
    SymTable* table = tree -> table;
    SymTableEntry* data;
    Anode* rhs = lhs -> sibling;
    if(strcmp(rhs -> name, "SIZE") == 0){
        if( (data = get_ele_table(tree, lhs -> token -> value)) == NULL){
            semantic_error = 1;
            printf("Semantic Error : < %s > is not declared at Line < %d >\n", lhs -> token -> value, lhs -> token -> lineNo);
        }else{
            token_id type = data -> type;
            if(type != INT){
                semantic_error = 1;
                printf("Semantic Error : < %s > is not of type INT at Line < %d >\n", lhs -> token -> value, lhs -> token -> lineNo);
            }
        }
        if((data = get_ele_table(tree, rhs -> token -> value)) == NULL){
            semantic_error = 1;
            printf("Semantic Error : < %s > is not declared at Line < %d >\n", rhs -> token -> value, rhs -> token -> lineNo);
        }else{
            if(data -> type != STRING){
                semantic_error = 1;
                printf("Semantic Error : < %s > is not of type STRING at Line < %d >\n", rhs -> token -> value, rhs -> token -> lineNo);
            }
        }
    }else if(strcmp(rhs -> name, "FUN_CALL") == 0){
        if( (data = get_ele_table(tree, lhs -> token -> value)) == NULL){
            semantic_error = 1;
            printf("Semantic Error : < %s > is not declared at Line < %d >\n", lhs -> token -> value, lhs -> token -> lineNo);
            handle_ae(rhs, tree);
        }else{
            SymTableEntry* fun;
            if((fun = get_ele_table(tree, rhs -> token -> value)) == NULL){
                semantic_error = 1;
                printf("Semantic Error : Function < %s > is not declared at Line < %d >\n", rhs -> token -> value, rhs -> token -> lineNo);
            }else{
                if( strcmp(fun -> value, table -> name) == 0){
                    semantic_error = 1;
                    printf("Semantic Error : Function < %s > has recursive declaration at Line < %d >\n", fun -> value, rhs -> token -> lineNo);
                }
                Anode* pt = fun -> astnode;
                pt = pt -> child;
                if(pt -> child -> token -> id != data -> type ){
                    semantic_error = 1;
                    printf("Semantic Error : < %s > is not compatible with output of function < %s > at Line < %d >\n", lhs -> token -> value, fun -> value ,lhs -> token -> lineNo);                    
                }
                pt = pt -> sibling -> sibling;
                pt = pt -> child;
                rhs = rhs -> child;
                int i = 0 ;
                while( pt != NULL && rhs != NULL){
                    Anode* type = pt;
                    pt = pt -> sibling; 
                    SymTableEntry* temp;
                    if( strcmp(rhs -> name, "ID") ==0 ){
                        if( (temp = get_ele_table(tree, rhs -> token -> value)) == NULL){
                            semantic_error = 1;
                            printf("Semantic Error : < %s > is not declared at Line < %d >\n", rhs -> token -> value, rhs -> token -> lineNo);
                        }else if( type -> token -> id != temp -> type){
                            semantic_error = 1;
                            printf("Semantic Error : < %s > does not have same type as < %s > at Line < %d >\n", pt -> token -> value , rhs -> token -> value, rhs -> token -> lineNo);                        
                        }
                    }else{
                        if( type -> token -> id != get_type(rhs)){
                            semantic_error = 1;
                            printf("Semantic Error : < %s > does not have same type as < %s > at Line < %d >\n", pt -> token -> value , rhs -> token -> value, rhs -> token -> lineNo);
                        }
                    }
                    pt = pt -> sibling;
                    rhs = rhs -> sibling;
                }
                if(pt != NULL || rhs != NULL){
                    semantic_error = 1;
                    printf("Semantic Error : Function call < %s > does not have correct input parameter length at Line < %d >\n", fun -> value, lhs -> token -> lineNo);                    
                }
            }
        }

    }else{
        if( (data = get_ele_table(tree, lhs -> token -> value)) == NULL){
            semantic_error = 1;
            printf("Semantic Error : < %s > is not declared at Line < %d >\n", lhs -> token -> value, lhs -> token -> lineNo);
            handle_ae(rhs, tree);
        }else{
            if(strcmp(rhs -> name, "VAR") == 0 && rhs -> token -> id == STR){
                data -> size = strlen(rhs -> token -> value) - 2;
            }else if(strcmp(rhs -> name, "MATRIX") == 0 ){
                Anode* child = rhs -> child;
                int row = 0;
                int column;
                while(child != NULL ) {
                    Anode* nchild = child -> child;
                    column = 0;
                    while(nchild != NULL){
                        nchild = nchild -> sibling;
                        column++;
                    }
                    if( column != data -> dim[1] && data -> dim[1] != 0){
                        semantic_error = 1;
                        printf("Semantic Error : MATRIX < %s > has imbalanced rows < %d > and < %d > at Line < %d >\n", lhs -> token -> value, data -> dim[1],  column, lhs -> token -> lineNo);
                    }
                    data -> dim[1] = column;
                    row++;
                    child = child -> sibling;
                }
                data -> dim[0] = row;
                data -> size = (data -> dim[0]) * (data -> dim[1]) * 2;
            }else if (data -> type != handle_ae(rhs, tree)){
                semantic_error = 1;
                printf("Semantic Error : < %s > is not compatible with RHS expression at Line < %d >\n", lhs -> token -> value, lhs -> token -> lineNo);
            }
        }
    }
}

void handle_ass2_stmt(Anode* root, SymTree* tree){
    Anode* lhs = root -> child;
    SymTable* table = tree -> table;
    SymTableEntry* data;
    Anode* rhs = lhs -> sibling;
    while( strcmp(rhs -> name, "SIZE") != 0 && strcmp(rhs -> name, "FUN_CALL") != 0){
        rhs = rhs -> sibling;
    }
    if(strcmp(rhs -> name, "SIZE") == 0){
        int i = 0;
        while (strcmp(rhs -> name, "SIZE") != 0){
            i++;
            if(i > 2){
                semantic_error = 1;
                printf("Semantic Error : More than 2 variables in LSH at Line < %d >\n", lhs -> token -> lineNo);
                break;
            }
            if( (data = get_ele_table(tree, lhs -> token -> value)) == NULL){
                semantic_error = 1;
                printf("Semantic Error : < %s > is not declared at Line < %d >\n", lhs -> token -> value, lhs -> token -> lineNo);
            }else{
                token_id type = data -> type;
                if(type != INT){
                    semantic_error = 1;
                    printf("Semantic Error : < %s > is not of type INT at Line < %d >\n", lhs -> token -> value, lhs -> token -> lineNo);
                }
            }
            lhs = lhs -> sibling;
        }
        if( i == 1){
            semantic_error = 1;
            printf("Semantic Error : Needed 2 variables in LSH at Line < %d >\n", lhs -> token -> lineNo);
        }
        if((data = get_ele_table(tree, rhs -> token -> value)) == NULL){
            semantic_error = 1;
            printf("Semantic Error : < %s > is not declared at Line < %d >\n", rhs -> token -> value, rhs -> token -> lineNo);
        }else{
            if(data -> type != MATRIX){
                semantic_error = 1;
                printf("Semantic Error : < %s > is not of type STRING at Line < %d >\n", rhs -> token -> value, rhs -> token -> lineNo);
            }
        }
    }else if(strcmp(rhs -> name, "FUN_CALL") == 0){
        SymTableEntry* fun;
        if((fun = get_ele_table(tree, rhs -> token -> value)) == NULL){
            semantic_error = 1;
            printf("Semantic Error : Function < %s > is not declared at Line < %d >\n", rhs -> token -> value, rhs -> token -> lineNo);
        }
        if( strcmp(fun -> value, table -> name) == 0){
            semantic_error = 1;
            printf("Semantic Error : Function < %s > has recursive declaration at Line < %d >\n", fun -> value, rhs -> token -> lineNo);
        }

        Anode* pt = fun -> astnode;
        pt = pt -> child -> child;
        while (strcmp(lhs -> name, "FUN_CALL") != 0 && (pt != NULL)){
            Anode* type = pt;
            pt = pt -> sibling; 
            if( (data = get_ele_table(tree, lhs -> token -> value)) == NULL){
                semantic_error = 1;
                printf("Semantic Error : < %s > is not declared at Line < %d >\n", lhs -> token -> value, lhs -> token -> lineNo);
            }else{
                if(type -> token -> id != data -> type ){
                    semantic_error = 1;
                    printf("Semantic Error : < %s > is not compatible with output of function < %s > at Line < %d >\n", lhs -> token -> value, fun -> value ,lhs -> token -> lineNo);                    
                }
            }
            pt = pt -> sibling;
            lhs = lhs -> sibling;
        }
        if(pt != NULL || strcmp(lhs -> name, "FUN_CALL") != 0){
            semantic_error = 1;
            printf("Semantic Error : Output parameter length conflict at Line < %d >\n", lhs -> token -> lineNo);                    
        }
        
        pt = fun -> astnode;
        pt = pt -> child -> sibling -> sibling;

        pt = pt -> child;
        rhs = rhs -> child;
        while( pt != NULL && rhs != NULL){
            Anode* type = pt;
            pt = pt -> sibling; 
            SymTableEntry* temp;
            if( strcmp(rhs -> name, "ID") == 0 ){
                if( (temp = get_ele_table(tree, rhs -> token -> value)) == NULL){
                    semantic_error = 1;
                    printf("Semantic Error : < %s > is not declared at Line < %d >\n", rhs -> token -> value, rhs -> token -> lineNo);
                }else if( type -> token -> id != temp -> type){
                    semantic_error = 1;
                    printf("Semantic Error : < %s > does not have same type as < %s > at Line < %d >\n", pt -> token -> value , rhs -> token -> value, rhs -> token -> lineNo);                        
                }
            }else{
                if( type -> token -> id != get_type(rhs)){
                    semantic_error = 1;
                    printf("Semantic Error : < %s > does not have same type as < %s > at Line < %d >\n", pt -> token -> value , rhs -> token -> value, rhs -> token -> lineNo);
                }
            }
            pt = pt -> sibling;
            rhs = rhs -> sibling;
        }
        if(pt != NULL || rhs != NULL){
            semantic_error = 1;
            printf("Semantic Error : Function call < %s > does not have correct input parameter length at Line < %d >\n", fun -> value, lhs -> token -> lineNo);                    
        }
    }
}

void handle_function_def(Anode* root, SymTree* tree){
    SymTree* snode = make_tree_node();
    SymTable* table = tree -> table;
    Anode* child = root -> child;
    child = child -> sibling;
    snode -> table = create_table(child -> token -> value);
    if( get_ele_table(tree, child -> token -> value) != NULL){
        semantic_error = 1;
        printf("Semantic Error : < %s > is already declared at Line < %d >\n", child -> token -> value, child -> token -> lineNo);
        return;        
    }
    add_to_table(table, FUNCTION, child -> token -> value, root);
    child = root -> child;
    Anode* id = child -> child;
    if(tree -> child == NULL){
        tree -> child = snode;
        snode -> parent = tree;
    }else{
        SymTree* tp = tree -> child;
        while( tp -> sibling != NULL){
            tp = tp -> sibling;
        }
        tp -> sibling = snode;
        snode -> parent = tree;
    }
    while(id != NULL){
        Anode* type = id;
        id = id -> sibling;
        if( get_ele_table(tree, id -> token -> value) != NULL){
            semantic_error = 1;
            printf("Semantic Error : < %s > is already declared at Line < %d >\n", id -> token -> value, id -> token -> lineNo);
        }else{
            add_to_table(snode -> table, type -> token -> id , id -> token -> value, id);
        }
        id = id -> sibling;
    }
    child = child -> sibling -> sibling;
    id = child -> child;
    while(id != NULL){
        Anode* type = id;
        id = id -> sibling;
        if( get_ele_table(tree, id -> token -> value) != NULL){
            semantic_error = 1;
            printf("Semantic Error : < %s > is already declared at Line < %d >\n", id -> token -> value, id -> token -> lineNo);
        }else{
            add_to_table(snode -> table, type -> token -> id , id -> token -> value, id);
        }
        id = id -> sibling;
    }
    child = child -> sibling;
    snode = get_sym_tree(child, snode);
}

token_id handle_be(Anode* root, SymTree* tree){
    SymTable* table = tree -> table;
    SymTableEntry* data;

    if(strcmp(root -> name, "NOT") == 0){
        if(handle_be(root -> child, tree) == DOLLAR){
            return DOLLAR;
        }else{
            return END;
        }
    }else if(strcmp(root -> name, "CONST_VAR") == 0){
        if(root -> token -> id == NUM) return INT;
        else if(root -> token -> id == RNUM) return REAL;
        else{
            if(root -> child == NULL){
                if((data = get_ele_table(tree, root -> token -> value)) == NULL){
                    semantic_error = 1;
                    printf("Semantic Error : < %s > is not declared at Line < %d >\n", root -> token -> value, root -> token -> lineNo);
                }else{
                    return data -> type;
                }
            }else{
                return INT;
            }
        }

    }else if ( strcmp(root -> name, "RELATIONAL_OP") == 0){
        token_id lhs = handle_be(root -> child, tree), rhs = handle_be(root -> child -> sibling, tree);
        if( lhs == rhs){
            return DOLLAR;
        }else{
            semantic_error = 1;
            printf("Semantic Error : < %s > and < %s > are not compatible with < %s > operation at Line < %d >\n", root -> child -> token -> value, root -> child -> sibling -> token -> value, id_to_token(root -> token -> id) ,root -> child -> token -> lineNo);
            return END;
        }
    }else if ( strcmp(root -> name, "LOG_OP") == 0){
        token_id lhs = handle_be(root -> child, tree), rhs = handle_be(root -> child -> sibling, tree);
        if(lhs == rhs){
            if(lhs != DOLLAR){
                semantic_error = 1;
                printf("Semantic Error : < %s > and < %s > are not compatible with < %s > operation at Line < %d >\n", root -> child -> token -> value, root -> child -> sibling -> token -> value, id_to_token(root -> token -> id) ,root -> child -> token -> lineNo);
                return END;
            }
            return DOLLAR;

        }else{
            semantic_error = 1;
            printf("Semantic Error : < %s > and < %s > are not compatible with < %s > operation at Line < %d >\n", root -> child -> token -> value, root -> child -> sibling -> token -> value, id_to_token(root -> token -> id) ,root -> child -> token -> lineNo);
            return END;
        }
    }
}
void handle_if_stmt(Anode* root, SymTree* tree){
    Anode* lhs = root -> child;
    SymTable* table = tree -> table;
    SymTableEntry* data;
    Anode* rhs = lhs -> sibling;

    if( handle_be(lhs, tree) != DOLLAR){
        semantic_error = 1;
        printf("Semantic Error : BooleanExpression is not valid in IF condition at Line < %d >\n", lhs -> token -> lineNo);
    }
    lhs = lhs -> sibling;
    while( lhs != NULL && strcmp(lhs -> name, "ELSE") != 0 ){
        get_sym_tree(lhs, tree);
        lhs = lhs -> sibling;
    }
    if( lhs != NULL){
        rhs = lhs -> child;
        get_sym_tree(rhs, tree);
    }   
}

void handle_io_stmt(Anode* root, SymTree* tree){
    Anode* lhs = root;
    SymTable* table = tree -> table;
    SymTableEntry* data;
    Anode* rhs = lhs -> child;

    if(lhs -> token -> id == PRINT){
        if( get_ele_table(tree, rhs -> token -> value) == NULL){
            semantic_error = 1;
            printf("Semantic Error : < %s > is not declared at Line < %d >\n", rhs -> token -> value, rhs -> token -> lineNo);
        }
    }else{
        if( (data = get_ele_table(tree, rhs -> token -> value) )== NULL){
            semantic_error = 1;
            printf("Semantic Error : < %s > is not declared at Line < %d >\n", rhs -> token -> value, rhs -> token -> lineNo);
        }else{
            if( data -> type == MATRIX || data -> type == STRING){
                semantic_error = 1;
                printf("Semantic Error : < %s > is not allowed to be used in READ statement at Line < %d >\n", rhs -> token -> value, rhs -> token -> lineNo);
            }
        }
    }
}
SymTree* get_sym_tree(Anode* root, SymTree* tree){
    if(root == NULL) return tree;
    if(root -> parent == NULL) root = root -> child;
    if(strcmp(root -> name , "DEC_STMT") == 0){
        handle_dec_stmt(root, tree);
    }else if(strcmp(root -> name , "ID_ASSIGN") == 0){
        handle_ass1_stmt(root, tree);
    }else if(strcmp(root -> name, "FUNCTION_DEF") == 0){
        handle_function_def(root, tree);
    }else if(strcmp(root -> name, "LIST_ASSIGN") == 0){
        handle_ass2_stmt(root, tree);
    }else if( strcmp(root -> name, "IF") == 0){
        handle_if_stmt(root , tree);
    }else if( strcmp(root -> name , "IO") == 0){
        handle_io_stmt(root, tree);
    }
    root = root -> sibling;
    tree = get_sym_tree(root, tree);
    return tree;
}

void print_sym_tree(SymTree* root){
    if( root == NULL) return;
    // printf("\n\n\t\t\t\t\t\tPrinting SYMBOL TABLE\n");

    if ( root -> parent == NULL)
        print_table(root->table, "NULL", 0);
    else
        print_table(root->table, root -> parent -> table -> name, 0);
    print_sym_tree(root -> child);
    if(root -> child == NULL)
        return;
    SymTree* pt = root -> child;
    pt = pt -> sibling;
    while(pt != NULL){
        print_sym_tree(pt);
        pt = pt -> sibling;
    }
}