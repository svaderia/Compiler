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
#include "_SEMANTIC.h"
#include "_SEMANTICDEF.h"

int main(int argc, char* argv[]){
    FILE* input, *output;
    input = fopen(argv[1], "r");
    output = fopen("parse.out", "w");
    printf("\n Shyamal Vaderia\t\t 2015A7PS0048P");
    printf("\n\n\tFirst and Follow set Automated.\n");
    printf("\tBoth lexical and syntax analysis modules implemented.\n");
    printf("\tAll testcase gives expected answer\n\n\n");


    int option;
    printf("Kindly choose an Option:  ");
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
    Anode* node;
    SymTree* snode;
    
    while(option != 0){
        input = fopen(argv[1], "r");
        switch(option){
            case 1:
                inp = getTokens(input, b, k, keywords);
                print_list(inp);
                break;
            case 2:
                fp = fopen("grammar", "r");
                read_grammar(G, fp);
                compute_first(G, first_set);
                compute_follow(G, first_set, follow_set);
                compute_parse_table(Table, first_set, follow_set, G);
                root = construct_parse_tree(input, Table, G);
                print_parse_tree(output, root);
                // printf("Parse Tree is dumped into the file < %s >.\n", argv[2]);
                break;
            case 3:
                fp = fopen("grammar", "r");
                read_grammar(G, fp);
                compute_first(G, first_set);
                compute_follow(G, first_set, follow_set);
                compute_parse_table(Table, first_set, follow_set, G);
                root = construct_parse_tree(input, Table, G);
                node = get_ast(root, NULL);
                printf("Traversal Order : Root -> left -> right\n");
                print_ast(node);
                break;
            case 4:
                printf("NOT IMPLEMENTED THIS FUNCTIONALITY\n");
                break;
            case 5:
                fp = fopen("grammar", "r");

                read_grammar(G, fp);
                compute_first(G, first_set);
                compute_follow(G, first_set, follow_set);
                compute_parse_table(Table, first_set, follow_set, G);
                root = construct_parse_tree(input, Table, G);
                node = get_ast(root, NULL);
                // print_ast(node);
                snode = make_tree_node();
                snode -> table = create_table("MAIN");
                printf("\n\n");
                snode = get_sym_tree(node, snode);
                print_sym_tree(snode);
                break;
            case 6:
                fp = fopen("grammar", "r");

                read_grammar(G, fp);
                compute_first(G, first_set);
                compute_follow(G, first_set, follow_set);
                compute_parse_table(Table, first_set, follow_set, G);
                root = construct_parse_tree(input, Table, G);
                node = get_ast(root, NULL);
                print_ast(node);
                snode = make_tree_node();
                snode -> table = create_table("MAIN");
                printf("\n\n");
                snode = get_sym_tree(node, snode);
                break;
            case 7: 
                printf("NOT IMPLEMENTED THIS FUNCTIONALITY\n");
                break;
        }
        printf("Kindly choose an Option:  ");
        scanf("%d", &option);
        printf("\n\n\n");
        
    }
}
