#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "_LEXER.h"
#include "_LINKEDLIST.h"

List* create_list(){
	List* temp = (List*) malloc(sizeof(List));
	if(temp == NULL){
		return NULL;
	}else{
		temp -> head = NULL;
		temp -> tail = NULL;
		temp -> size = 0;
		return temp;
	}
}

LNode* create_node(Token* t){
	LNode* temp = (LNode*) malloc(sizeof(LNode));
	if(temp == NULL){
		return NULL;
	}else{
		temp -> ele = t;
		temp -> next = NULL;
		return temp;
	}
}

List* add_to_list(List* lst, Token* t){
    LNode* new_node = create_node(t);
    if(lst -> size == 0){
        lst -> head = new_node;
        lst -> tail = new_node;
        (lst -> size) ++;
    }else{
        lst -> tail -> next = new_node;
        lst -> tail = new_node;
        (lst -> size) ++;
    }
    return lst;
}

void print_list(List* lst){
	LNode* temp = lst -> head;
	while(temp != NULL){
        print_token(temp -> ele);
		temp = temp -> next;
	}
	printf("\n");
}