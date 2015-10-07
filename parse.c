#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "char.h"
#include "list.h"

// Conversion from 'string' to 'type' probably wanna refactor this.
#define PARSE_MAP_SIZE 5
#define PARSE_MAP_WORDS {"print","if","then","end","is"}
#define PARSE_MAP_TYPES {'p','i','t','e','z'}

// Probably some memory leaks over here.
char parse_word_to_type(char *str){	
	char *words[PARSE_MAP_SIZE] = PARSE_MAP_WORDS;
	char types[PARSE_MAP_SIZE]  = PARSE_MAP_TYPES;
	
	int i;
	for(i=0; i<PARSE_MAP_SIZE; i++){
		if(strcmp(str,words[i]) == 0) return types[i];
	}
	
	return 0;
}

// Here too.
char *parse_type_to_word(char type){
	char *words[PARSE_MAP_SIZE] = PARSE_MAP_WORDS;
	char types[PARSE_MAP_SIZE]  = PARSE_MAP_TYPES;
	
	int i;
	for(i=0; i<PARSE_MAP_SIZE; i++){
		if(type == types[i]) return words[i];
	}
	
	return "no word for type "+type;	
}
// Move to parser
int parse_find_number_end(char *str, int start){
	int i,l=strlen(str);
	for(i=start; i<l; i++){
		char c = str[i];
		if( !char_is_numeric(c) || i == l-1 ){
			if( i == l-1 ){
				return i+1;
			}else{
				return i;
			}
		}
	}
	return -1;
}

int parse_find_word_end(char *str, int start){
	int i,l=strlen(str);
	for(i=start; i<l; i++){
		char c = str[i];
		if( !char_is_alphabetic(c) ){
			return i;
		}
	}
	printf("Ended on '%c'\n", str[i]);
	return -1;
}

int parse_number(char *str, int i){
	int start = i;
	int end = parse_find_number_end(str,start);
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
	int end = parse_find_word_end(str,start);
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
	
	struct Token *t;
	
	t = token_create(' ',0); // Blank token, gets thrown out by default. 
	
	if( i == 0 )
		left = ' ';
	if( i == strlen(str)-1 )
		right = ' ';
	
	if( char_is_numeric(current) && !char_is_numeric(left) ){
		int value = parse_number(str,i);
		
		t->type  = '#';
		t->value = value;
	}
	
	if( char_is_alphabetic(current) && !char_is_alphabetic(left) ){
		char *word = parse_word(str,i);
		
		char type = parse_word_to_type(word);
		
		if( type != 0 ){
			t->type = type;
		}
	}
	
	if( char_is_operator(current) ){
		t->type = current;
	}
	
	if( char_is_parenthesis(current) ){
		t->type = current;
	}
	
	//printf("%c->%c\n",current,t->type);
	
	return t;
}

// Builds a token list from a string
struct List *parse(char *str){
	
	struct List *list;
	list = list_create();
	
	int length = strlen(str);
	int i;
	for(i=0; i<length; i++){
		struct Token *t = parse_char(str,i);
		// If the token is 'blank' we can forget it.
		if( t->type == ' ' ){
			free(t);
		}else{
			list_add(list,t);
		}
	}
	
	list_remove(list,0);
	
	return list;
}