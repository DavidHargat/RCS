#include <stdio.h>
#include <string.h>

#include "inter.h"
#include "list.h"
#include "token.h"
#include "functions.h"
#include "parse.h"

struct Token *function_assignment(struct Token *statement){

	struct Token *name = statement->list->tokens[1];
	struct Token *exp  = statement->list->tokens[2];

	statement
	
}

struct Token *function_lookup(struct Token *statement){
	struct Token *word;

	word = statement->scope->tokens

	return word;
}

struct Token *function_operator(struct Token *statement){
	struct Token *left, *right, *op;

	op    = statement->list->tokens[0];
	left  = statement->list->tokens[1];
	right = statement->list->tokens[2];

	if( left->type  != T_NUMBER ) left  = function_execute(left);
	if( right->type != T_NUMBER ) right = function_execute(right);

	int result = 0;

	if( op->type == T_ADD)      result = left->value + right->value;
	if( op->type == T_SUBTRACT) result = left->value - right->value;
	if( op->type == T_MULTIPLY) result = left->value * right->value;
	if( op->type == T_DIVIDE)   result = left->value / right->value;

	return token_create(T_NUMBER,result);	
}


// takes n arguments
struct Token *function_print(struct Token *statement){
	// For each argument, evaluate and print
	int i;
	for(i=1; i<statement->list->length; i++){
		struct Token *arg = statement->list->tokens[i];
		if(arg->type == T_LIST && arg->value == LIST_FUNCTION){
			//list_print(exp->list);
			struct Token *result = function_execute(arg);

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

struct Token *function_if(struct Token *statement){
	struct Token *exp, *stm;

	exp = statement->list->tokens[1];
	stm = statement->list->tokens[2];

	struct Token *result = function_execute(exp);

	if(result->value > 0){
		function_execute_list(stm->list);
	}

	return token_create(T_NUMBER,0);
}

struct Token *function_expression(struct Token *statement){
	return token_create(T_NUMBER,
			   inter_expression(
			    	list_sub(statement->list,1,statement->list->length-1)
			   )
		   );
}

struct Token *function_execute(struct Token *statement){

	int header = statement->list->tokens[0]->type;

	if( token_is_operator(header) )
		return function_operator(statement);

	if(header == T_PRINT)
		return function_print(statement);

	if(header == T_IF)
		return function_if(statement);

	if(header == T_EXPRESSION)
		return function_expression(statement);

	if(header == T_STRING)
		return statement->list->tokens[1];

	printf("(ERROR:function_execute) Statement was not a valid function.\n");
	return token_create(T_NUMBER,0);
}

void function_execute_list(struct List *statements){
	int i;
	for(i=0; i<statements->length; i++){
		function_execute(statements->tokens[i]);
	}
}