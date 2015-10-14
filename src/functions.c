#include <stdio.h>

#include "inter.h"
#include "list.h"
#include "token.h"
#include "functions.h"

// takes n arguments
struct Token *function_print(struct Token *statement){
	// For each argument, evaluate and print
	int i;
	for(i=1; i<statement->list->length; i++){
		struct Token *exp = statement->list->tokens[i];
		if(exp->value == LIST_FUNCTION){
			//list_print(exp->list);
			int result = function_execute(exp)->value;
			printf("->%d\n",result);
		}else{
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

	if(header == T_PRINT)
		return function_print(statement);

	if(header == T_IF)
		return function_if(statement);

	if(header == T_EXPRESSION)
		return function_expression(statement);

	printf("(ERROR:function_execute) Statement was not a valid function.\n");
	return token_create(T_NUMBER,0);
}

void function_execute_list(struct List *statements){
	int i;
	for(i=0; i<statements->length; i++){
		function_execute(statements->tokens[i]);
	}
}