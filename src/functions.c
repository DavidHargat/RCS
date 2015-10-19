#include <stdio.h>
#include <string.h>

#include "inter.h"
#include "list.h"
#include "token.h"
#include "functions.h"
#include "parse.h"

struct Token *function_assignment(struct Token *statement, struct List *scope){
	return token_create(T_NUMBER,0);
}

struct Token *function_lookup(struct Token *statement, struct List *scope){
	struct Token *word = token_create(T_LIST,LIST_FUNCTION);

	struct Token *arg = statement->list->tokens[1];

	char *name;

	if( arg->value == T_LIST ){
		name = function_execute(arg, scope)->string;
	}else if (arg->type == T_STRING){
		name = arg->string;
	}else{
		printf("(ERROR:function_lookup) Invalid lookup type. ");
	}


	// For each statement
	int i;
	for(i=0; i<scope->length; i++){
		struct Token *func   = scope->tokens[i];
		struct Token *header = func->list->tokens[0]; // Grab the header of said function
		// If statement is an assignment
		if( header->type == T_EQUALS ){
			struct Token *fname    = func->list->tokens[1];
			struct Token *fvalue   = func->list->tokens[2];
	
			// If the function name matches the lookup name
			if( strcmp(fname->string,name) == 0 ){
				return function_execute(fvalue,scope);
			}
			
		}
	}

	printf("(ERROR:function_lookup) '%s' not found in scope.\n", name);

	return word;
}

struct Token *function_operator(struct Token *statement, struct List *scope){
	struct Token *left, *right, *op;

	op    = statement->list->tokens[0];
	left  = statement->list->tokens[1];
	right = statement->list->tokens[2];

	if( left->type  != T_NUMBER ) left  = function_execute(left,scope);
	if( right->type != T_NUMBER ) right = function_execute(right,scope);

	int result = 0;

	if( op->type == T_ADD)      result = left->value + right->value;
	if( op->type == T_SUBTRACT) result = left->value - right->value;
	if( op->type == T_MULTIPLY) result = left->value * right->value;
	if( op->type == T_DIVIDE)   result = left->value / right->value;

	return token_create(T_NUMBER,result);	
}

// takes n arguments
struct Token *function_print(struct Token *statement, struct List *scope){
	// For each argument, evaluate and print
	int i;
	for(i=1; i<statement->list->length; i++){
		struct Token *arg = statement->list->tokens[i];
		if(arg->type == T_LIST && arg->value == LIST_FUNCTION){
			//list_print(exp->list);
			struct Token *result = function_execute(arg,scope);

			if( result->type == T_STRING){
				printf("->'%s'\n",result->string);
			}else if ( result->type == T_NUMBER ){
				printf("->%d\n",result->value);
			}else{
				printf("(ERROR:function_print) Could not print type '%s'.\n",parse_type_to_word(result->type));
			}

		}else if (arg->type == T_STRING){
			printf("(ERROR:function_print) Invalid argument at '%d'.\n",i-1);
		}
	}

	return token_create(T_NUMBER,0);
}

struct Token *function_if(struct Token *statement, struct List *scope){
	struct Token *exp, *stm;

	exp = statement->list->tokens[1];
	stm = statement->list->tokens[2];

	struct Token *result = function_execute(exp,scope);

	if(result->value > 0){
		function_execute_list(stm->list,stm->list);
	}

	return token_create(T_NUMBER,0);
}

struct Token *function_expression(struct Token *statement, struct List *scope){
	return statement->list->tokens[1];
}

struct Token *function_not(struct Token *statement, struct List *scope){
	struct Token *exp;

	exp = statement->list->tokens[1];

	struct Token *result = function_execute(exp,scope);

	if(result->value > 0){
		return token_create(T_NUMBER,0);
	}else{
		return token_create(T_NUMBER,1);;
	}

}

struct Token *function_execute(struct Token *statement, struct List *scope){

	int header = statement->list->tokens[0]->type;

	if( token_is_operator(header) ) return function_operator(statement,scope);
	if(header == T_PRINT)           return function_print(statement,scope);
	if(header == T_IF)              return function_if(statement,scope);
	if(header == T_EXPRESSION)      return function_expression(statement,scope);
	if(header == T_STRING)          return statement->list->tokens[1];
	if(header == T_EQUALS)          return function_assignment(statement,scope);
	if(header == T_LOOKUP)          return function_lookup(statement,scope);
	if(header == T_NOT)             return function_not(statement,scope);

	printf("(ERROR:function_execute) Statement was not a valid function.\n");
	return token_create(T_NUMBER,0);
}

void function_execute_list(struct List *statements, struct List *scope){
	int i;
	for(i=0; i<statements->length; i++){
		function_execute(statements->tokens[i], scope);
	}
}