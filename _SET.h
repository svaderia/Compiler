#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef _SET
#define _SET

#define SIZE 4
struct set{
    char** elements;
    int size;
    int empty;
};

typedef struct set Set;

Set* get_set();

Set* add_to_set(Set* set, char* key);

bool serach_set(Set* set, char* key);

#endif