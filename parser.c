#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "_LEXER.h"
#include "_LEXERDEF.h"
#include "_SET.h"
#include "_PARSERDEF.h"
#include "_PARSER.h"

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

void read_grammer(G_Ele G[][MAX_RULE_LENGTH], FILE* fp){
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

void print_grammer_table(G_Ele G[][MAX_RULE_LENGTH]){
    int i, j;
    for (i = 0; i < MAX_RULE; i++){
        printf("%d) %s --> ",i+1 ,  id_to_nt(G[i][0].value.nt));
        for(j = 1; j < G[i][0].length + 1; j++){
            if(G[i][j].tp != NT)
                printf("%s ", id_to_token(G[i][j].value.t));
            else
                printf("%s ", id_to_nt(G[i][j].value.nt));
        }
        printf("\n");
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
                }else if(G[i][j+1].tp == T){
                    C = G[i][j+1].value.t;
                    follow_set[B][C] = true;
                    // break;
                }else if (G[i][j+1].tp == NT){
                    l = j+1;
                    C = G[i][l].value.nt;
                    while(G[i][l].tp == NT && first_set[C][EPSILON] && l < G[i][0].length + 1){
                        C = G[i][l].value.nt;
                        for(k=0; k < NUM_TERMINAL ; k++){
                            if(first_set[C][k])
                                follow_set[B][k] = true;
                        }
                        follow_set[B][EPSILON] = false;
                        l++;
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