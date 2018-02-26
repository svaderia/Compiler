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

void compute_parse_table(int Table[][NUM_TERMINAL], bool first_set[][NUM_TERMINAL], bool follow_set[][NUM_TERMINAL], G_Ele G[][MAX_RULE_LENGTH]);

void print_parse_table(int Table[][NUM_TERMINAL], G_Ele G[][MAX_RULE_LENGTH]);

char* id_to_nt(non_terminal id);

non_terminal nt_to_id(char* token);

Tnode* construct_parse_tree(int parseTable[][NUM_TERMINAL], G_Ele G[][MAX_RULE_LENGTH], List* input);

void print_parse_tree(FILE* fp, Tnode* root);

#endif