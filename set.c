#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "_SET.h"

Set* get_set(){
    Set* temp = (Set*) malloc(sizeof(Set));
    if(temp){
        temp -> elements = (char**) malloc(SIZE * sizeof(char*));
        temp -> size = SIZE;
        temp -> empty = 0;
    }
    return temp;
}

Set* add_to_set(Set* set, char* key){
    set -> elements[set->empty] = (char*) malloc((strlen(key) + 1) * sizeof(char));
    memcpy(set -> elements[set->empty], key, (strlen(key) + 1));
    (set -> empty) ++;

    if(set -> empty >= set -> size){
        set -> size *= 2;
        set -> elements = (char**) realloc(set -> elements, set -> size);
    }
    return set;
}

bool serach_set(Set* set, char* key){
    int i;
    bool has = false;
    char** lst = set -> elements;
    for(i = 0; i < set->empty; i++){
        // if(lst[i] == NULL) break;
        if(strcmp(lst[i], key) == 0){
            has = true;
            break;
        }
    }
    return has;
}


/*
// TESTING FUNCTIONALITIES OF SET.
int main(){
    Set* set = get_set();
    add_to_set(set, "end");
    add_to_set(set, "int");
    add_to_set(set, "real");
    add_to_set(set, "string");
    add_to_set(set, "matrix");

    printf("%d\n", serach_set(set, "end"));
    printf("%d\n", serach_set(set, "int"));
    printf("%d\n", serach_set(set, "hola"));
    printf("%d\n", serach_set(set, "matrix"));
    printf("%d\n", serach_set(set, "bb"));
    printf("%d\n", serach_set(set, "mat"));
    printf("%d\n", set->size);
    

}
*/