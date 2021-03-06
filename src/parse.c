#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "char.h"
#include "list.h"

// Conversion from 'string' to 'type' probably wanna refactor this.

int PARSE_POINTER = 0;

// Probably some memory leaks over here.
int parse_word_to_type(char *str){	
	
	int i;
	for(i=0; i<TOKEN_MAP_SIZE; i++){
		if(strcmp(str,TOKEN_MAP_WORDS[i]) == 0) return i;
	}
	
	return -1;
}

// Here too.
char *parse_type_to_word(char type){
	
	int i;
	for(i=0; i<TOKEN_MAP_SIZE; i++){
		if(type == i) return TOKEN_MAP_WORDS[i];
	}
	
	char *msg = malloc(16);
	sprintf(msg, "<%d>", type);
	return msg;	
}
// Move to parser
int parse_find_number_end(char *str, int start){
	int i,l=strlen(str);
	for(i=start; i<l; i++){
		char c = str[i];
		if( !char_is_numeric(c)){
			return i-1;	
		}
		if( i == l-1 ){
			return i;
		}
	}
	return -1;
}

int parse_find_word_end(char *str, int start){
	int i,l=strlen(str);
	for(i=start; i<l; i++){
		char c = str[i];
		if( !char_is_alphabetic(c) ){
			return i-1;
		}
		if( i == l-1 ){
			return i;
		}
	}
	return -1;
}

int parse_find_string_end(char *str, int start){
	int i=0, l=strlen(str);
	for(i=start; i<l; i++){
		char c = str[i];
		if( c == '"' ){
			return i;
		}
	}
	return -1;
}

int parse_number(char *str, int i){
	int start = i;
	int end = parse_find_number_end(str,start)+1;
	if( end >= start ){
		// converts to a string, then convert string to a number
		int num_of_digits = end-start;
		
		char sub[num_of_digits+1];
		
		memcpy( sub, &str[start], num_of_digits );
		sub[num_of_digits] = '\0';

		return strtol(sub, 0, 10);
	}else{
		printf("(parse.c::parse_number) failed to parse number at %d \n",i);
		return 0;
	}
}

char *parse_word(char *str, int i){
	int start = i;
	int end = parse_find_word_end(str,start)+1;
	if( end >= start ){
		// converts to a string, then convert string to a number
		int num_of_chars = end-start;
		
		char *sub = malloc(sizeof(char)*(num_of_chars+1));
		
		memcpy( sub, &str[start], num_of_chars );
		sub[num_of_chars] = '\0';

		return sub;
	}else{
		printf("(parse.c::parse_word) failed to parse word at %d \n",i);
		return "ERROR";
	}
}

char *parse_string(char *str, int i){
	int start = i+1;
	int end = parse_find_string_end(str,start+1);

	PARSE_POINTER = end;

	if( end >= start ){
		// converts to a string, then convert string to a number
		int num_of_chars = end-start;
		
		char *sub = malloc(sizeof(char)*(num_of_chars+1));
		
		memcpy( sub, &str[start], num_of_chars );
		sub[num_of_chars] = '\0';
		return sub;
	}else{
		printf("(parse.c::parse_word) failed to parse word at %d \n",i);
		return "ERROR";
	}
}

// parse_char returns: checks chars and returns a token if one is found, else return ?void?
struct Token *parse_char(char *str, int i){
	char current = str[i], left = str[i-1], right = str[i+1];
	
	/* 
	* If our scan is 'out of bounds' 
	* assume whitespace to avoid undefined behaviour.
	*/

	
	if( i == 0 )
		left = ' ';

	if( i == (strlen(str)-1) ) 
		right = ' ';

	struct Token *t;
	t = token_create(-1,0); // Blank token, gets thrown out by default. 

	if( char_is_numeric(current) && !char_is_numeric(left) ){
		int value = parse_number(str,i);
		
		t->type = T_NUMBER;
		t->value = value;
	}
	
	if( char_is_alphabetic(current) && !char_is_alphabetic(left) && !(left=='"') ){
		char *word = parse_word(str,i);
		
		char type = parse_word_to_type(word);

		if( type != -1 ){
			t->type = type;
		}else{
			t->type   = T_VAR;
			t->string = word;
		}
	}

	if( current == '"'){
		char *strx = parse_string(str,i);
		t->type   = T_STRING;
		t->string = strx;
	}
	
	if( char_is_operator(current) ){
		if( current == '+' ) t->type = T_ADD;
		if( current == '-' ) t->type = T_SUBTRACT;
		if( current == '*' ) t->type = T_MULTIPLY;
		if( current == '/' ) t->type = T_DIVIDE;
	}
	
	if( char_is_symbol(current) ){
		if( current == '(' ) t->type = T_OPEN_PAREN;
		if( current == ')' ) t->type = T_CLOSE_PAREN;
		if( current == '=' ) t->type = T_EQUALS;
		if( current == ',' ) t->type = T_COMMA;
		if( current == '!' ) t->type = T_NOT;
	}

	return t;
}

// Builds a token list from a string
struct List *parse(char *str){
	
	struct List *list;
	list = list_create();
	
	int length = strlen(str);
	//int i;
	for(PARSE_POINTER=0; PARSE_POINTER<length; PARSE_POINTER++){
		struct Token *t = parse_char(str,PARSE_POINTER);
		if( t->type == -1 ){
			//printf("Freeing token at %d\n",PARSE_POINTER);
			//token_print(t);
			free(t);
		}else{
			list_add(list,t);
		}
	}
	
	return list;
}