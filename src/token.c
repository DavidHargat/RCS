#include <stdio.h>
#include <stdlib.h>

#include "token.h"

void token_print(struct Token *t){
	if(t->type=='#'){
		printf(" %d",t->value);
	}else{
		if(char_is_alphabetic(t->type)){
			printf(" %s",parse_type_to_word(t->type));
		}else{
			printf(" %c",t->type);
		}
	}
}

struct Token *token_create(char type, int value){
	struct Token *t = malloc(sizeof(struct Token *));
	t->type  = type;
	t->value = value;
	return t;
}