#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "_SET.h"
#include "_LEXER.h"
#include "_LEXERDEF.h"
#include "_PARSERDEF.h"
#include "_PARSER.h"

int main(){
    Set* keywords = get_keywords();
    FILE* fp = fopen("test", "r");
    int k = 4096;
    buffer b = (buffer) malloc(k * sizeof(char));
    eof = false;
    offset = 0;
    // while(! ( offset >= strlen(b) && eof )){
    //     print_token(getToken(fp, b, k, keywords));
    // }

    fp = fopen("grammer", "r");
    G_Ele G[MAX_RULE][MAX_RULE_LENGTH];
    read_grammer(G, fp);
    // print_grammer_table(G);
    bool first_set[NUM_NONTERMINAL][NUM_TERMINAL];
    compute_first(G, first_set);
    // print_first_set(first_set);
    bool follow_set[NUM_NONTERMINAL][NUM_TERMINAL];
    compute_follow(G, first_set, follow_set);
    print_follow_set(follow_set);
}
