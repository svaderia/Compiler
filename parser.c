/*
    Name: Shyamal Vaderia
    ID: 2015A7PS0048P
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "_LEXER.h"
#include "_LEXERDEF.h"
#include "_SET.h"
#include "_PARSERDEF.h"
#include "_STACK.h"
#include "_LINKEDLIST.h"

char* nt[NUM_NONTERMINAL] = {"mainFunction","stmtsAndFunctionDefs","recStmtsAndFunctionDefs","stmtOrFunctionDef","stmt","functionDef","parameter_list","type","remainingList","declarationStmt","var_list","more_ids","assignmentStmt_type1","assignmentStmt_type2","leftHandSide_singleVar","leftHandSide_listVar","rightHandSide_type1","rightHandSide_type2","sizeExpression","ifStmt","elseStmt","otherStmts","ioStmt","funCallStmt" ,"inputParameterList","listVar","arithmeticExpression", "recArithmeticExpression", "arithmeticTerm", "recArithmeticTerm", "factor","operator_lowPrecedence","operator_highPrecedence","booleanExpression","constrainedVars","var","matrix","rows","recRows","row","recRow","remainingColElements","matrixElement","logicalOp","relationalOp"};

char* id_to_nt(non_terminal id){
    return nt[id];
}

non_terminal nt_to_id(char* token){
    int i;
    for(i=0; i < NUM_NONTERMINAL; i++){
        if(! strcmp(nt[i], token))
            return i;
    }
    return -1;
}

bool is_nt(char* nt){
    int i = nt_to_id(nt);
    return i != -1;
}

void read_grammar(G_Ele G[][MAX_RULE_LENGTH], FILE* fp){
    int i = 0;
    int pos = 0;
    char LHS[100];
    char RHS[100];
    while(!feof(fp)){
        pos = 0;
        fscanf(fp, "%s", LHS);
        // printf("%s -->  ", LHS);      
        G[i][pos].value.nt = nt_to_id(LHS);
        G[i][pos].tp = NT;
        G[i][pos].length = 0;
        fscanf(fp, "%s", RHS);
        while(strcmp(RHS, ".")){
            pos++;
            // printf("%s  ", RHS);
            if(is_nt(RHS)){
                G[i][pos].value.nt = nt_to_id(RHS);
                G[i][pos].tp = NT;
            }else{
                G[i][pos].value.t = token_to_id(RHS);
                G[i][pos].tp = T;
            }
            G[i][0].length += 1;
            fscanf(fp, "%s", RHS);
        }
        // printf("\n");
        i++;
    }
}

void print_rule(G_Ele G[][MAX_RULE_LENGTH], int i){
    int j;
    printf("%d) %s --> ",i+1 ,  id_to_nt(G[i][0].value.nt));
    for(j = 1; j < G[i][0].length + 1; j++){
        if(G[i][j].tp != NT)
            printf("%s ", id_to_token(G[i][j].value.t));
        else
            printf("%s ", id_to_nt(G[i][j].value.nt));
    }
    printf("\n");
}

void print_grammar_table(G_Ele G[][MAX_RULE_LENGTH]){
    int i;
    for (i = 0; i < MAX_RULE; i++){
        print_rule(G, i);   
    }
}

void first(G_Ele G[][MAX_RULE_LENGTH], bool first_set[][NUM_TERMINAL], int alpha, bool* isdone){
    int i, j, temp, k;
    if(! isdone[alpha]){
        for(i = 0; i < MAX_RULE; i++){
            if(G[i][0].value.nt == alpha){
                for(j = 1; j < G[i][0].length + 1; j++){
                    if(G[i][j].tp != NT){
                        temp = G[i][j].value.t;
                        first_set[alpha][temp] = true;
                        break;
                    }else{
                        temp = G[i][j].value.nt;
                        if(! isdone[temp]){
                            first(G, first_set, temp, isdone);
                        }
                        for(k=0; k < NUM_TERMINAL - 1; k++){
                            if(first_set[temp][k])
                                first_set[alpha][k] = true;
                        }
                        isdone[alpha] = true;
                        if(first_set[temp][EPSILON] == true){
                            if(j == G[i][0].length){
                                first_set[alpha][EPSILON] = true;
                            }
                        }else{
                            break;
                        }
                    }
                }
            }
        }
    }
}

void initialize_first(bool first_set[][NUM_TERMINAL]){
    int i,j;
    for(i=0; i < NUM_NONTERMINAL; i++){
        for(j=0; j<NUM_TERMINAL; j++){
            first_set[i][j] = false;
        }
    }
}

void compute_first(G_Ele G[][MAX_RULE_LENGTH], bool first_set[][NUM_TERMINAL]){
    bool isdone[NUM_NONTERMINAL];
    int i;
    initialize_first(first_set);
    for(i = 0; i < NUM_NONTERMINAL; i++){
        isdone[i] = false;
    }
    for(i = 0; i < NUM_NONTERMINAL; i++){
        first(G, first_set, i, isdone);
    }
}

void print_first_set(bool first_set[][NUM_TERMINAL]){
    int i,j;
    for(i=0; i < NUM_NONTERMINAL; i++){
        printf("First set of :: %s\n\t", id_to_nt(i));
        for(j=0; j < NUM_TERMINAL; j++){
            if(first_set[i][j])
                printf("%s  ", id_to_token(j));
        }
        printf("\n");
    }
}

void follow(G_Ele G[][MAX_RULE_LENGTH], bool follow_set[][NUM_TERMINAL], bool first_set[][NUM_TERMINAL]){
    int i, j, k, l, B, A, C, val;
    for(i = 0; i < MAX_RULE; i++){
        A = G[i][0].value.nt;
        for(j = 1; j < G[i][0].length + 1; j++ ){
            if(G[i][j].tp == NT){
                B = G[i][j].value.nt;
                if(j == G[i][0].length){
                    for(k=0; k < NUM_TERMINAL - 1; k++){
                            if(follow_set[A][k])
                                follow_set[B][k] = true;
                    }
                }else if(G[i][j+1].tp == T && G[i][j+1].value.t != EPSILON){
                    C = G[i][j+1].value.t;
                    follow_set[B][C] = true;
                    // break;
                }else if (G[i][j+1].tp == NT){
                    l = j+1;
                    C = G[i][l].value.nt;
                    while(G[i][l].tp == NT && first_set[C][EPSILON] && l < G[i][0].length + 1){
                        for(k=0; k < NUM_TERMINAL ; k++){
                            if(first_set[C][k])
                                follow_set[B][k] = true;
                        }
                        follow_set[B][EPSILON] = false;
                        l++;
                        C = G[i][l].value.nt;                        
                    }
                    C = G[i][l].value.nt;
                    if(l == G[i][0].length + 1){
                        for(k = 0; k < NUM_TERMINAL ; k++){
                                if(follow_set[A][k])
                                    follow_set[B][k] = true;
                        }
                        // break;
                    }else if(G[i][l].tp == T){
                        C = G[i][l].value.t;
                        follow_set[B][C] = true;
                        // break;
                    }else if(! first_set[C][EPSILON]){
                        C = G[i][l].value.nt;
                        for(k=0; k < NUM_TERMINAL ; k++){
                            if(first_set[C][k])
                                follow_set[B][k] = true;
                        }
                        // break;
                        
                    }
                }
            }
        }
    }
}

bool diff(bool temp_follow[][NUM_TERMINAL], bool follow_set[][NUM_TERMINAL]){
    int i, j;
    for(i = 0;i < NUM_NONTERMINAL; i++){
        for(j = 0; j < NUM_TERMINAL; j++){
            if(temp_follow[i][j] != follow_set[i][j]){
                return true;
            }
        }
    }
    return false;
}

bool copy_follow(bool temp_follow[][NUM_TERMINAL], bool follow_set[][NUM_TERMINAL]){
    int i, j;
    for(i = 0;i < NUM_NONTERMINAL; i++){
        for(j = 0; j < NUM_TERMINAL; j++){
            if(follow_set[i][j])
                temp_follow[i][j] = true;
        }
    }
}

void initialize_follow(bool follow_set[][NUM_TERMINAL]){
    int i,j;
    for(i=0; i < NUM_NONTERMINAL; i++){
        for(j=0; j<NUM_TERMINAL; j++){
            follow_set[i][j] = false;
        }
    }
    follow_set[mainFunction][DOLLAR] = 1;
}

void compute_follow(G_Ele G[][MAX_RULE_LENGTH], bool first_set[][NUM_TERMINAL], bool follow_set[][NUM_TERMINAL]){
    bool temp_follow[NUM_NONTERMINAL][NUM_TERMINAL];
    initialize_follow(temp_follow);
    initialize_follow(follow_set);
    while(1){
        follow(G, follow_set, first_set);
        if(!diff(temp_follow, follow_set)) break;
        copy_follow(temp_follow, follow_set);
    }
}

void print_follow_set(bool follow_set[][NUM_TERMINAL]){
    int i, j;
    for(i=0; i < NUM_NONTERMINAL; i++){
        printf("Follow set of :: %s\n\t", id_to_nt(i));
        for(j = 0; j < NUM_TERMINAL; j++){
            if(follow_set[i][j])
                printf("%s  ", id_to_token(j));
        }
        printf("\n");
    }
}

void initialize_Table(int Table[][NUM_TERMINAL]){
    int i,j;
    for(i = 0; i < NUM_NONTERMINAL; i++){
        for(j = 0; j < NUM_TERMINAL; j++){
            Table[i][j] = -1;
        }
    }
}

void compute_parse_table(int Table[][NUM_TERMINAL], bool first_set[][NUM_TERMINAL], bool follow_set[][NUM_TERMINAL], G_Ele G[][MAX_RULE_LENGTH]){
    int i, j, k , A, B, C;
    initialize_Table(Table);
    for(i = 0; i < MAX_RULE; i++){
        A = G[i][0].value.nt;
        for(j = 1; j < G[i][0].length + 1; j++ ){
            if(G[i][j].tp == T && G[i][j].value.t != EPSILON){
                C = G[i][j].value.t;
                Table[A][C] = i;
                break;
            }else if(G[i][j].tp == NT && ! first_set[G[i][j].value.nt][EPSILON]){
                C = G[i][j].value.nt;
                for(k = 0; k < NUM_TERMINAL; k++){
                    if(first_set[C][k])
                        Table[A][k] = i;
                }
                break;
            }else{
                if(G[i][j].tp == NT){
                    C = G[i][j].value.nt;
                    for(k = 0; k < NUM_TERMINAL; k++){
                        if(first_set[C][k] && k != EPSILON)
                            Table[A][k] = i;
                    }
                }
                if( j == G[i][0].length){
                    for(k = 0; k < NUM_TERMINAL; k++){
                        if(follow_set[A][k] && k != EPSILON){
                            Table[A][k] = i;
                        }
                    }
                }
            }
        }
    }
}

void print_parse_table(int Table[][NUM_TERMINAL], G_Ele G[][MAX_RULE_LENGTH]){
    int i, j, k;
    for(i = 0; i < NUM_NONTERMINAL; i++){
        printf("Non Terminal %s\n", id_to_nt(i));
        for(j = 0; j < NUM_TERMINAL; j++){
            if(Table[i][j] != -1){
                printf("\tTerminal %s \t ", id_to_token(j));            
                print_rule(G, Table[i][j]);
            }
        }
    }
}

Tnode* get_tree_node(G_Ele g){
    Tnode* t = (Tnode*) malloc(sizeof(Tnode));
    t -> parent = NULL;
    t -> sibling = NULL;
    t -> child = NULL;
    t -> token = NULL;
    t -> isleaf = false;
    if(g.tp == T){
        t -> ele.t = g.value.t;
        t -> tp = g.tp;
    }else{
        t -> ele.nt = g.value.nt;
        t -> tp = g.tp;
    }
    return t;
}

Tnode* construct_parse_tree(FILE* test, int parseTable[][NUM_TERMINAL], G_Ele G[][MAX_RULE_LENGTH]){
    Set* keywords = get_keywords();
    int k = 4096;
    buffer b = (buffer) malloc(k * sizeof(char));
    eof = false;
    offset = 0;
    List* input = getTokens(test, b, k, keywords);
    LNode* inp = input -> head;
    int rule_num, j;
    Tnode* temp;
    G_Ele dol, mainf, rule;
    dol.value.t = DOLLAR;
    dol.tp = T;
    dol.length = 0;
    mainf.value.t = mainFunction;
    mainf.tp = NT;
    mainf.length = 0;
    Tnode* pt = get_tree_node(mainf);
    pt -> sibling = pt;
    Stack* st = initialize_stack();
    st = push_stack(st, dol);
    st = push_stack(st, mainf);
    Node* top;
    int synt_error;

    while(1){
        top = top_stack(st);
        if(inp ->ele -> id == COMMENT){
            inp = inp -> next;
            continue;
        }
        if(top == NULL || inp == NULL)
                break;
            
        if( top -> tp == T && top -> ele.t == DOLLAR && inp->ele->id == DOLLAR && synt_error != 1){
            printf("Input source code is syntactically correct.\n");
            break;
        }else if( top -> tp == T && top -> ele.t == inp->ele->id){
            st = pop_stack(st); 
            pt -> token = inp -> ele;
            pt -> isleaf = true;
            inp = inp -> next;            
            while(pt -> sibling == NULL)
                pt = pt -> parent;
            pt = pt -> sibling;
        }else if (top -> tp == NT && parseTable[top->ele.nt][inp->ele->id] != -1){
            st = pop_stack(st);
            st = push_rule(st, G, parseTable[top->ele.nt][inp->ele->id]);
            bool flag = 1;
            rule_num = parseTable[top->ele.nt][inp->ele->id];
            for(j = 1; j < G[rule_num][0].length + 1; j++){
                if(flag == 1){
                    temp = get_tree_node(G[rule_num][j]);
                    temp -> parent = pt;
                    pt -> child = temp;
                    flag = 0;
                    pt = temp;
                }else{
                    temp = get_tree_node(G[rule_num][j]);
                    temp -> parent = pt -> parent;
                    pt -> sibling = temp;
                    pt = temp;
                }
            }
            pt = pt -> parent -> child;
        }else if( top -> tp == T && top -> ele.t == EPSILON){
            st = pop_stack(st);
            pt -> token = (Token*) malloc(sizeof(Token));
            pt -> token -> id = EPSILON;
            pt -> token -> value = NULL;
            pt -> token -> lineNo = 0;
            pt -> ele.t = EPSILON;
            pt -> tp = T;
            pt -> isleaf = true;    
            while(pt -> sibling == NULL)
                pt = pt -> parent;
            pt = pt -> sibling;
        }else{
            
            if(top -> tp == T && top -> ele.t != inp->ele->id && top->ele.t != DOLLAR){
                synt_error = 1;
                printf("Expected token < %s > instead of < %s > at Line < %d > \n", id_to_token(top -> ele.t), id_to_token(inp->ele->id), inp->ele->lineNo );
            }else if (top->ele.t != DOLLAR){
                synt_error = 1;
                printf("Syntactic error at Line < %d > \n", inp -> ele -> lineNo);
            }
            while(inp->ele->id != SEMICOLON){
                inp = inp -> next;
            }
            inp = inp->next;
            while( st->size > 0 && top_stack(st) -> ele.t != SEMICOLON) {
                // printf("Popping: ")
                st = pop_stack(st);
            }
            st = pop_stack(st);
            if(pt -> parent != NULL)
                pt = pt -> parent;
            while(pt -> sibling == NULL){
                pt = pt -> parent;
            }
            pt = pt -> sibling;
            // printf("Error in Syntax analyser\n");
            // break;
        }
    }
    return pt;
}

void print_parse_tree(FILE* writter_file, Tnode* root){
    if (root == NULL)
        return;
    print_parse_tree(writter_file, root->child);
    char not_available[] = "-----";
    if(root->isleaf){
        char leaf[] = "yes";
        if (root->ele.t == EPSILON){
            printf("%-16s %-16s %-16s %-16s %-26s %-16s %-16s\n", 
                    not_available, not_available, id_to_token(root->ele.t), not_available, 
                    id_to_nt(root->parent->ele.nt), leaf,
                    not_available);
        }
        else {
            printf("%-16s %-16d %-16s %-16s %-26s %-16s %-16s\n",
                    root->token->value,
                    root->token->lineNo, id_to_token(root->token->id),
                    not_available, id_to_nt(root->parent->ele.nt), leaf,
                    not_available);
        }
    }
    else{
        char nonleaf[] = "no";
        char at_root[10];
        if (root->parent == NULL){
            strcpy(at_root, "ROOT");
        }
        else {
            strcpy(at_root, id_to_nt(root->parent->ele.nt));
        }
        printf("%-16s %-16s %-16s %-16s %-26s %-16s %-16s\n",
                not_available, not_available, not_available, not_available, 
                at_root, nonleaf,
                id_to_nt(root->ele.nt));
 
    }
    Tnode *pt = root->child;
    if(root->child == NULL)
        return;
    pt = pt->sibling;
    while(pt != NULL){
        print_parse_tree(writter_file, pt);
        pt = pt->sibling;
    }
}