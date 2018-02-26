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

int main(){
    Set* keywords = get_keywords();
    FILE* fp = fopen("test", "r");
    int k = 4096;
    buffer b = (buffer) malloc(k * sizeof(char));
    eof = false;
    offset = 0;
    List* tokens = all_tokens(fp, b, k, keywords);
    // print_list(tokens);

    fp = fopen("grammar", "r");
    G_Ele G[MAX_RULE][MAX_RULE_LENGTH];
    read_grammar(G, fp);
    // print_grammar_table(G);
    bool first_set[NUM_NONTERMINAL][NUM_TERMINAL];
    compute_first(G, first_set);
    // print_first_set(first_set);
    bool follow_set[NUM_NONTERMINAL][NUM_TERMINAL];
    compute_follow(G, first_set, follow_set);
    // print_follow_set(follow_set);

    int Table[NUM_NONTERMINAL][NUM_TERMINAL];
    compute_parse_table(Table, first_set, follow_set, G);
    // print_parse_table(Table, G);
    Tnode* root;
    root = construct_parse_tree(Table, G, tokens);
    FILE* out = fopen("out", "w");

    print_parse_tree(out, root);
}
