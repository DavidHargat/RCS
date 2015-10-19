#include <string.h>

// Move to char.c
// *Replace with lookup table.
int char_is_match(int c, char *str){
	int i;
	for(i=0; i<strlen(str); i++){
		if( c == str[i] ) return 1;
	}
	return 0;
}
int char_is_operator(int c){ return char_is_match(c, "*/-+"); }
int char_is_numeric(int c){ return char_is_match(c, "0123456789"); }
int char_is_alphabetic(int c){ return char_is_match(c, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"); }
int char_is_symbol(int c){ return char_is_match(c, "()=,!"); }
int char_is_statement(int c){ return char_is_match(c,"ip="); }

