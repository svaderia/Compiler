/*
    Name: Shyamal Vaderia
    ID: 2015A7PS0048P
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "_SET.h"
#include "_LEXER.h"
#include "_LEXERDEF.h"
#include "_PARSERDEF.h"
#include "_PARSER.h"
#include "_STACK.h"
#include "_LINKEDLIST.h"

int main(int argc, char* argv[]){
    FILE* input, *output;
    if(argc > 1){
        input = fopen(argv[1], "r");
        output = fopen(argv[2], "w");
        if(input == NULL){
            printf("%s file does not exists", argv[1]);
            return -1;
        }
    }else{
        printf("Kindly input two files");
        return -1;
    }
    printf("\n Shyamal Vaderia\t\t 2015A7PS0048P");
    printf("\n\n\tFirst and Follow set Automated.\n");
    printf("\tBoth lexical and syntax analysis modules implemented.\n");
    printf("\tAll testcase gives expected answer\n\n\n");


    int option;
    printf("\n1. For removal of comments. \n");
    printf("2. For printing the token list\n");
    printf("3. For parsing to verify the syntactic correctness of the input source code. \n");
    printf("4. For printing the parse tree appropriately.\n");
    printf("Choose from above options:  ");
    scanf("%d", &option);
    printf("\n\n\n");

    Set* keywords = get_keywords();
    int k = 4096;
    buffer b = (buffer) malloc(k * sizeof(char));
    eof = false;
    offset = 0;
    List* inp;
    FILE* fp;
    G_Ele G[MAX_RULE][MAX_RULE_LENGTH];
    bool first_set[NUM_NONTERMINAL][NUM_TERMINAL];
    bool follow_set[NUM_NONTERMINAL][NUM_TERMINAL];
    int Table[NUM_NONTERMINAL][NUM_TERMINAL];
    Tnode* root;
    

    switch(option){
        case 1:
            remove_comments(input, b, k);
            break;
        case 2:
            inp = getTokens(input, b, k, keywords);
            print_list(inp);
            break;
        case 3:
            fp = fopen("grammar", "r");
            read_grammar(G, fp);
            compute_first(G, first_set);
            compute_follow(G, first_set, follow_set);
            compute_parse_table(Table, first_set, follow_set, G);
            root = construct_parse_tree(input, Table, G);
            break;
        case 4:
            fp = fopen("grammar", "r");
            read_grammar(G, fp);
            compute_first(G, first_set);
            compute_follow(G, first_set, follow_set);
            compute_parse_table(Table, first_set, follow_set, G);
            root = construct_parse_tree(input, Table, G);
            print_parse_tree(output, root);
            printf("Parse Tree is dumped into the file < %s >.\n", argv[2]);
            break;
    }
}
