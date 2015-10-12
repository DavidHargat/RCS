#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "parse.h"

void token_print(struct Token *t){
	if(t->type=='#'){
		printf(" %d",t->value);
	}else{
		if(char_is_alphabetic(t->type)){
			char *s = parse_type_to_word(t->type);
			printf(" %s",s);
		}else{
			printf(" %c",t->type);
		}
	}
}

struct Token *token_create(char type, int value){
	struct Token *t = malloc(sizeof(struct Token *));
	t->type  = type;
	t->value = value;
	t->list  = NULL;
	return t;
}