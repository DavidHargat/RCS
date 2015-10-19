#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "list.h"
#include "parse.h"
#include "list.h"

void token_print(struct Token *t){
	if(t->type == T_NUMBER){
		printf(" %d",t->value);
	}else if (t->type == T_LIST){
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
		if(t->value == LIST_FUNCTION)
			printf("func");
		printf("[]->\n");
		list_print(t->list);
	}else if (t->type == T_STRING){
		printf(" s'%s' ",t->string);
	}else{
		char *s = parse_type_to_word(t->type);
		printf(" %s ",s);
	}
	//printf("\n");
}

void token_print_tree(struct Token *t, int indent){
	int i,j;

	printf("| ");

	for(j=0; j<indent; j++){printf("...");}

	if(t->type == T_NUMBER){
		printf(" %d",t->value);
	}else if (t->type == T_STRING) {
		if(t->string == NULL){
			printf(" str");
		}else{
			printf(" s'%s'",t->string);
		}
	}else if (t->type == T_LIST){
		printf(" ");

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
		if(t->value == LIST_FUNCTION)
			printf("func");

		printf("[%d]->\n",t->list->length);
		for(i=0;i<t->list->length;i++){
			token_print_tree(t->list->tokens[i],indent+1);
		}
	}else{
		char *s = parse_type_to_word(t->type);
		printf(" %s",s);
	}

	if (t->type != T_LIST)
		printf("\n");
	
}

struct Token *token_create(int type, int value){
	struct Token *t = malloc(sizeof(struct Token));
	t->type  = type;
	t->value = value;
	if( type == T_LIST ){
		t->list = list_create();
	}else{
		t->list  = NULL;
	}
	return t;
}


int token_is_match(int type, int types[], int len){
	int i;
	for(i=0; i<len; i++){
		if( type == types[i] ) return 1;
	}
	return 0;
}

int token_is_operator(int type){ 
	return 
		type == T_ADD      ||
		type == T_SUBTRACT ||
		type == T_MULTIPLY ||
		type == T_DIVIDE
	;
}

int token_is_numberic(int type){
	return type == T_NUMBER;
}

int token_is_symbol(int type){
	return type == T_OPEN_PAREN  ||
		   type == T_CLOSE_PAREN ||
		   type == T_EQUALS
	;
}

int token_is_statement(int type){
	return type == T_IF     ||
		   type == T_PRINT  ||
		   type == T_EQUALS 
	;
}