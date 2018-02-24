#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "_LEXER.h"
#include "_LEXERDEF.h"
#include "_SET.h"

#define TOKEN_RETURN(token, i, lexeme) token -> value = (char*) malloc((strlen(lexeme) + 1) * sizeof(char));\
strcpy(token->value, lexeme); \
token -> id = i;\
token -> lineNo = lineNo;\
return token;\

#define REPORT_ERROR(token, lexeme)  printf("Error:: Unknown pattern <%s> at Line <%d>\n", lexeme, lineNo);\
token -> id = ERROR;\
token -> lineNo = lineNo;\
return token;\

int offset = 0;
bool eof = false;
int lineNo = 1;
bool lexical_error = 0;
char* token_class[41] = {"ASSIGNOP","COMMENT","FUNID","ID","NUM","RNUM","STR","END","INT","REAL","STRING","MATRIX","MAIN","SQO","SQC","OP","CL","SEMICOLON","COMMA","IF","ELSE","ENDIF","READ","PRINT","FUNCTION","PLUS","MINUS","MUL","DIV","SIZE","AND","OR","NOT","LT","LE","EQ","GT","GE","NE","ENDFILE","ERROR"};

FILE *getStream(FILE *fp, buffer b, int k)
{	int count = -1;
    if (!eof)
        count = fread(b, 1, k, fp);

    if(count < k) eof = true ;
    b[count] = '\0';
    return fp;
}

char* id_to_token(token_id id){
    return token_class[id];
}

token_id token_to_id(char* token){
    int i;
    for(i=0; i < 41; i++){
        if(! strcmp(token_class[i], token))
            return i;
    }
}

char* convert_to_upper(char* str){
    int i;
    char* temp = malloc(strlen(str)+1);
    for(i = 0; str[i] != '\0'; i++){
        temp[i] = str[i];
        if(str[i] >= 'a' && str[i] <= 'z')
            temp[i] = str[i] + 'A' - 'a';
    }
    return temp;
}

Set* get_keywords(){
    Set* key = get_set();
    key = add_to_set(key, "end");
    key = add_to_set(key, "int");
    key = add_to_set(key, "real");
    key = add_to_set(key, "string");
    key = add_to_set(key, "matrix");
    key = add_to_set(key, "if");
    key = add_to_set(key, "else");
    key = add_to_set(key, "endif");
    key = add_to_set(key, "read");
    key = add_to_set(key, "print");
    key = add_to_set(key, "function");
    
    return key;
}

void print_token(Token* t){
    printf("value :: %10s\tType :: %10s\tLineNo :: %d\n", t->value, id_to_token(t->id), t->lineNo);
}
Token* getToken(FILE* fp, buffer b, int k, Set* keywords){
    buffer lexeme = (buffer) malloc(k * sizeof(char));
    int i = 0;
    memset(lexeme, 0, k);
    Token* token = (Token*) malloc(sizeof(Token));
    int state = 0;
    while(1){
        if (offset == k || strlen(b) == 0 || b[offset] == '\0'){
			if( eof ){
                token -> id = ENDFILE;
                token -> lineNo = lineNo;
                token -> value = "$";
				return token;
			}
			memset(b, 0, k*sizeof(char));
            fp = getStream(fp, b, k);
            offset = 0;
		}

        switch(state){
            // START STATE  
            case 0:
                lexeme[i++] = b[offset];
                switch(b[offset])
                {
                    case '\n':
                        lineNo++;
                    case '\t':
                    case ' ':
                        lexeme[--i] = '\0';
                        state = 0;
                        break;
                    case '.':
                        state = 1;
                        break;
                    case '<':
                        state = 13;
                        break;
                    case '>':
                        state = 16;
                        break;
                    case '=':
                        state = 19;
                        break;
                    case '/':
                        offset++;
                        TOKEN_RETURN(token, DIV, lexeme);
                    case '+':
                        offset++;
                        TOKEN_RETURN(token, PLUS, lexeme);
                    case '-':
                        offset++;
                        TOKEN_RETURN(token, MINUS, lexeme);
                    case '@':
                        offset++;
                        TOKEN_RETURN(token, SIZE, lexeme);
                    case '*':
                        offset++;
                        TOKEN_RETURN(token, MUL, lexeme);
                    case '_':
                        state = 37;
                        break;
                    case '\"':
                        state = 41;
                        break;
                    case ';':
                        offset++;
                        TOKEN_RETURN(token, SEMICOLON, lexeme);
                    case '[':
                        offset++;
                        TOKEN_RETURN(token, SQO, lexeme);
                    case ']':
                        offset++;
                        TOKEN_RETURN(token, SQC, lexeme);
                    case '(':
                        offset++;
                        TOKEN_RETURN(token, OP, lexeme);
                    case ')':
                        offset++;
                        TOKEN_RETURN(token, CL, lexeme);
                    case ',':
                        offset++;
                        TOKEN_RETURN(token, COMMA, lexeme);
                    case 'a':
                    case 'b':
					case 'c':
					case 'd':
					case 'e':
					case 'f':
					case 'g':
					case 'h':
					case 'i':
					case 'j':
					case 'k':
					case 'l':
					case 'm':
					case 'n':
					case 'o':
					case 'p':
					case 'q':
					case 'r':
					case 's':
					case 't':
					case 'u':
					case 'v':
					case 'w':
					case 'x':
					case 'y':
					case 'z':
                        state = 34;
                        break;
                    case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
                        state = 29;
                        break;
                    default:
                        lexical_error = 1;
                        offset++;
                        REPORT_ERROR(token, lexeme);
                }
                offset++;
                break;

            case 1:
                lexeme[i++] = b[offset];
                switch(b[offset]){
                    case 'a':
                        state = 2;
                        break;
                    case 'o':
                        state = 6;
                        break;
                    case 'n':
                        state = 9;
                        break;
                    default:
                        lexical_error = 1;
                        lexeme[--i] = '\0';
                        REPORT_ERROR(token, lexeme);
                }
                offset++;
                break;
            
            case 2:
                lexeme[i++] = b[offset];
                switch(b[offset]){
                    case 'n':
                        state = 3;
                        break;
                    default:
                        lexical_error = 1;
                        REPORT_ERROR(token, lexeme);
                }
                offset++;
                break;
            
            case 3:
                lexeme[i++] = b[offset];
                switch(b[offset]){
                    case 'd':
                        state = 4;
                        break;
                    default:
                        lexical_error = 1;
                        REPORT_ERROR(token, lexeme);
                }
                offset++;
                break;
            
            case 4:
                lexeme[i++] = b[offset];
                switch(b[offset]){
                    case '.':
                        offset++;
                        TOKEN_RETURN(token, AND, lexeme);
                    default:
                        lexical_error = 1;
                        REPORT_ERROR(token, lexeme);
                }
                offset++;
                break;
            
            case 6:
                lexeme[i++] = b[offset];
                switch(b[offset]){
                    case 'r':
                        state = 7;
                        break;
                    default:
                        lexical_error = 1;
                        REPORT_ERROR(token, lexeme);
                }
                offset++;
                break;
            
            case 7:
                lexeme[i++] = b[offset];
                switch(b[offset]){
                    case '.':
                        offset++;
                        TOKEN_RETURN(token, OR, lexeme);
                    default:
                        lexical_error = 1;
                        REPORT_ERROR(token, lexeme);
                }
                offset++;
                break;
            
            case 9:
                lexeme[i++] = b[offset];
                switch(b[offset]){
                    case 'o':
                        state = 10;
                        break;
                    default:
                        lexical_error = 1;
                        REPORT_ERROR(token, lexeme);
                }
                offset++;
                break;
            
            case 10:
                lexeme[i++] = b[offset];
                switch(b[offset]){
                    case 't':
                        state = 11;
                        break;
                    default:
                        lexical_error = 1;
                        REPORT_ERROR(token, lexeme);
                }
                offset++;
                break;
            
            case 11:
                lexeme[i++] = b[offset];
                switch(b[offset]){
                    case '.':
                        offset++;
                        TOKEN_RETURN(token, NOT, lexeme);
                    default:
                        lexical_error = 1;
                        REPORT_ERROR(token, lexeme);
                }
                offset++;
                break;

            case 13:
                lexeme[i++] = b[offset];
                switch(b[offset]){
                    case '=':
                        offset++;
                        TOKEN_RETURN(token, LE, lexeme);
                    default:
                        TOKEN_RETURN(token, LT, lexeme);
                }
                offset++;
                break;

            case 16:
                lexeme[i++] = b[offset];
                switch(b[offset]){
                    case '=':
                        offset++;
                        TOKEN_RETURN(token, GE, lexeme);
                    default:
                        lexeme[--i] = '\0';
                        TOKEN_RETURN(token, GT, lexeme);
                }
                offset++;
                break;

            case 19:
                lexeme[i++] = b[offset];
                switch(b[offset]){
                    case '=':
                        offset++;
                        TOKEN_RETURN(token, EQ, lexeme);
                    case '/':
                        state = 21;
                        break;
                    default:
                        lexeme[--i] = '\0';
                        TOKEN_RETURN(token, ASSIGNOP, lexeme);
                }
                offset++;
                break;

            case 21:
                lexeme[i++] = b[offset];
                switch(b[offset]){
                    case '=':
                        offset++;
                        TOKEN_RETURN(token, NE, lexeme);
                    default:
                        lexical_error = 1;
                        REPORT_ERROR(token, lexeme);
                }
                offset++;
                break;

            case 29:
                while('0' <= b[offset] && b[offset] <= '9'){
                    lexeme[i++] = b[offset++];
                }
                switch(b[offset]){
                    case '.':
                        lexeme[i++] = b[offset];
                        state = 30;
                        break;
                    default:
                        // lexeme[--i] = '\0';
                        TOKEN_RETURN(token, NUM, lexeme);
                }
                offset++;
                break;

            case 30:
                lexeme[i++] = b[offset];
                switch(b[offset]){
                    case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
                        switch(b[offset+1]){
                            case '0':
                            case '1':
                            case '2':
                            case '3':
                            case '4':
                            case '5':
                            case '6':
                            case '7':
                            case '8':
                            case '9':
                                lexeme[i++] = b[offset+1];
                                offset += 2;
                                TOKEN_RETURN(token, REAL, lexeme);
                            default:
                                lexical_error = 1;
                                REPORT_ERROR(token, lexeme);
                        }
                    case 'a':
                    case 'o':
                    case 'n':
                        state = 1;
                        offset--;
                        break;
                    default:
                        lexical_error = 1;
                        REPORT_ERROR(token, lexeme);
                }
                offset++;
                break;
            
            case 34:
                while('a' <= b[offset] && b[offset] <= 'z' || 'A' <= b[offset] && b[offset] <= 'Z'){
                    lexeme[i++] = b[offset++];
                }
                
                switch(b[offset]){
                    case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
                        offset ++;
                        TOKEN_RETURN(token, ID, lexeme);
                    default:
                        if(serach_set(keywords, lexeme)){
                            token_id ik = token_to_id(convert_to_upper(lexeme));
                            offset++;
                            TOKEN_RETURN(token, ik, lexeme);
                        }
                        TOKEN_RETURN(token, ID, lexeme);
                }
                offset++;
                break;

            case 37:
                lexeme[i++] = b[offset];
                if('a' <= b[offset] && b[offset] <= 'z' || 'A' <= b[offset] && b[offset] <= 'Z'){
                    state = 38;
                    offset++;
                }else{
                    lexical_error = 1;
                    REPORT_ERROR(token, lexeme);
                }
                break;

            case 38:
                lexeme[i++] = b[offset];
                if('a' <= b[offset] && b[offset] <= 'z' || 'A' <= b[offset] && b[offset] <= 'Z' || '0' <= b[offset] && b[offset] <= '9'){
                    state = 39;
                    offset++;
                }else{
                    lexical_error = 1;
                    REPORT_ERROR(token, lexeme);
                }
                break;

            case 39:
                while('a' <= b[offset] && b[offset] <= 'z' || 'A' <= b[offset] && b[offset] <= 'Z' || '0' <= b[offset] && b[offset] <= '9'){
                    lexeme[i++] = b[offset++];
                }
                if(strcmp(lexeme, "_main")){
                    TOKEN_RETURN(token, MAIN, lexeme);
                }else
                    TOKEN_RETURN(token, FUNID, lexeme);
            
            case 41:
                lexeme[i++] = b[offset];
                if('a' <= b[offset] && b[offset] <= 'z'){
                    state = 42;
                    offset++;
                }else{
                    lexical_error = 1;
                    REPORT_ERROR(token, lexeme);
                }
                break;

            case 42:
                while('a' <= b[offset] && b[offset] <= 'z'){
                    lexeme[i++] = b[offset++];
                }
                if(b[offset] == '\"'){
                    lexeme[i++] = b[offset++];
                    TOKEN_RETURN(token, STR, lexeme);}
                else{
                    lexical_error = 1;
                    REPORT_ERROR(token, lexeme);
                }
                break;
       }
    }
    free(lexeme);
}