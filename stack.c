#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "_PARSERDEF.h"
#include "_LEXERDEF.h"
#include "_LEXER.h"
#include "_PARSER.h"
#include "_STACK.h"


Stack* initialize_stack(){
    Stack* s = (Stack*) malloc(sizeof(Stack));
    if(s){
        s -> head = NULL;
        s -> size = 0;
    }
    return s;
}

Stack* push_stack(Stack* s, G_Ele r ){
    Node* temp = (Node*) malloc(sizeof(Node));
    if(r.tp == T){
        temp -> tp = T;
        temp -> ele.t = r.value.t;
    }else{
        temp -> tp = NT;
        temp -> ele.nt = r.value.nt;
    }
    (s -> size) ++;
    temp -> next = s -> head;
    s -> head = temp;
    return s;
}

Stack* pop_stack(Stack* s){
    Node* temp = NULL;
    if(s -> size > 0){
        temp = s -> head;
        s -> head = s -> head -> next;
        (s -> size) --;
    }
    return s;
}

Node* top_stack(Stack* s){
    return s -> head;
}

Stack* push_rule(Stack* s, G_Ele G[][MAX_RULE_LENGTH], int i){
    int j;
    for(j = G[i][0].length; j > 0; j--){
        s = push_stack(s, G[i][j]);
    }
    return s;
}

/*
int main(){
    Stack* s = initialize_stack();
    FILE* fp = fopen("grammar", "r");
    G_Ele G[MAX_RULE][MAX_RULE_LENGTH];
    read_grammar(G, fp);

    s = push_rule(s, G, 12);
    s = push_stack(s, G[0][3]);
    
    Node* t;
    while(s->size > 0){
        t = pop_stack(s);
        if(t -> tp == NT)
            printf("%s\n", id_to_nt(t -> ele.nt));
        else
            printf("%s\n", id_to_token(t -> ele.t));
    }
}
*/