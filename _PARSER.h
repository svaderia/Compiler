#include <stdio.h>
#include "_PARSERDEF.h"

#ifndef _PARSER
#define _PARSER

void read_grammar(G_Ele G[][MAX_RULE_LENGTH], FILE* fp);

void print_grammar_table(G_Ele G[][MAX_RULE_LENGTH]);

void compute_first(G_Ele G[][MAX_RULE_LENGTH], bool first_set[][NUM_TERMINAL]);

void print_first_set(bool first_set[][NUM_TERMINAL]);

void compute_follow(G_Ele G[][MAX_RULE_LENGTH], bool first_set[][NUM_TERMINAL], bool follow_set[][NUM_TERMINAL]);

void print_follow_set(bool follow_set[][NUM_TERMINAL]);

#endif