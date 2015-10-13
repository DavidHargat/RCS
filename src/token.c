#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "parse.h"
#include "list.h"

void token_print(struct Token *t){
	if(t->type == '#'){
		printf(" %d",t->value);
	}else if (t->type == 'l'){
		//printf("\n");
		if(t->value == LIST_EXPRESSION)
			printf("exp");
		if(t->value == LIST_STATEMENT)
			printf("stmnt");
		if(t->value == LIST_IF)
			printf("if");
		if(t->value == LIST_PRINT)
			printf("print");
		if(t->value == LIST_ASSIGNMENT)
			printf("assignment");
		printf("->\n");
		list_print(t->list);
	}else{
		if(char_is_alphabetic(t->type)){
			char *s = parse_type_to_word(t->type);
			printf(" %s",s);
		}else{
			printf(" %c",t->type);
		}
	}
	//printf("\n");
}

struct Token *token_create(char type, int value){
	struct Token *t = malloc(sizeof(struct Token *));
	t->type  = type;
	t->value = value;
	t->list  = NULL;
	return t;
}