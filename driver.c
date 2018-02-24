#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "_SET.h"
#include "_LEXER.h"
#include "_LEXERDEF.h"

int main(){
    Set* keywords = get_keywords();
    FILE* fp = fopen("test", "r");
    int k = 4096;
    buffer b = (buffer) malloc(k * sizeof(char));
    eof = false;
    offset = 0;
    while(! ( offset >= strlen(b) && eof )){
        print_token(getToken(fp, b, k, keywords));
    }
}
