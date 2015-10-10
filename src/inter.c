#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "list.h"
#include "parse.h"
#include "char.h"
#include "inter.h"

#define INTER_STATEMENTS "ip="

// BEGIN HELPER FUNCTIONS
int inter_operation(struct Token *ta, struct Token *op, struct Token *tb){
	char type = op->type;
	int result;
	
	int a = ta->value;
	int b = tb->value;
	
	if(type == '+')
		result = a + b;
	if(type == '*')
		result = a * b;
	if(type == '-')
		result = a - b;
	if(type == '/')
		result = a / b;
	
	return result;
}
// Inclusive of start! (checks the first index.)
int inter_find_next_type(struct List *list, char type, int start){
	int i;
	for(i=start;i<list->length;i++){
		if( list->tokens[i]->type == type )
			return i;
	}
	return -1;
}

int inter_find_next_match(struct List *list, char *types, int start){
	int i;
	for(i=start;i<list->length;i++){
		if( char_is_match(list->tokens[i]->type,types) )
			return i;
	}
	return -1;
}

// Finds the next MATCHING 'end', from start index. 
int inter_find_end(struct List *list, int start){
	int then_index = start;

	int nested_ifs = 0;

	int i;
	for(i=then_index; i<list->length; i++){
		struct Token *t = list->tokens[i];
		char type = t->type;
		if( type=='i' )
			nested_ifs++;
		if( type=='e'){
			if(nested_ifs<=0){
				return i;
			}else{
				nested_ifs--;
			}
		}
	}

	return -1;
}

// Finds the next MATCHING close parenthesis from start index.
int inter_find_close_paren(struct List *list, int start){
	int then_index = start;

	int nested_parens = 0;

	int i;
	for(i=then_index; i<list->length; i++){
		struct Token *t = list->tokens[i];
		char type = t->type;
		if( type == '(' )
			nested_parens++;
		if( type == ')'){
			if(nested_parens<=0){
				return i;
			}else{
				nested_parens--;
			}
		}
	}

	printf("(ERROR:inter_find_close_paren) Could not find matching ')' for '(' at %d.\n",start);
	return -1;	
}

// Checks if an expression is valid
// Returns boolean
int inter_expression_check(struct List *list){
	if(list->length == 0){
		printf("(ERROR:inter_expression_check) Expression is empty.\n");
	}

	if(list->length == 1){
		if( list->tokens[0]->type != '#' ){
			printf("(ERROR:inter_expression_check) Expression is non-numeric '%c'.\n", list->tokens[0]->type);
			return -1;
		}
	}

	int i;
	for(i=0; i<list->length; i++){
		struct Token *t = list->tokens[i];
		if( char_is_operator(t->type) ){
			if( i+1 < list->length ){
				struct Token *left=list->tokens[i-1],*right=list->tokens[i+1];
				if( left->type != '#' && left->type != ')' ){
					printf("(ERROR:inter_expression_check) Operator with invalid operand '%c%c'.\n", right->type, t->type);
					return -1;
				}
				if( right->type != '#' && right->type != '('){
					printf("(ERROR:inter_expression_check) Operator with invalid operand '%c%c'.\n", t->type, right->type);
					return -1;
				}
			}else{
				printf("(ERROR:inter_expression_check) Operator at end of token stream '%c'.\n", t->type);
				return -1;
			}
		}
	}
}

// Reduces parenthesis of given expression
// Mutation
int inter_expression_reduce(struct List *temp){
	int open_paren_index;
	while( (open_paren_index = inter_find_next_type(temp,'(', 0)) != -1 ){
		// Handle parenthesis recursively.
		if( open_paren_index != -1 ){
			int close_paren_index = inter_find_close_paren(temp,open_paren_index+1);
			if( close_paren_index != -1){
				int paren_size = (close_paren_index - open_paren_index);
				// Create a new token list 'sub_expression' for whats between the parenthesis
				struct List *sub_expression = list_sub(temp,open_paren_index+1,close_paren_index-1);
				// Calculate the result of that expression
				int result = inter_expression(sub_expression);
				// Remove that chunk of tokens from our list
				list_remove_sub(temp,open_paren_index+1,close_paren_index);
				// Add a 'result token' where the expression was.
				struct Token *result_token = token_create('#',result); // replace close paren with result
				temp->tokens[open_paren_index] = result_token;
			}else{
				printf("(ERROR:inter_expression) found '(' without matching ')'.\n");
				return 0;
			}
		}
	}
}

// Reduces math operations of given expression
// Mutation
int inter_expression_resolve(struct List *temp){
	int i;
	for(i=0; i < temp->length; i++){
		char type = temp->tokens[i]->type;
		if( char_is_operator(type) ){
			int result = inter_operation(temp->tokens[i-1],temp->tokens[i],temp->tokens[i+1]);
			struct Token *result_token = token_create('#',result);
			list_remove(temp,i-1);
			list_remove(temp,i-1);
			temp->tokens[i-1] = result_token;
			i = -1;
		}
	}
}

// END HELPER FUNCTIONS

// Resolves an expression to a value.
// TODO: Should probably resolve to a token (to support multiple values.)
int inter_expression(struct List *list){
	struct List *temp = list_copy(list);

	if( list->length == 0){
		return 0;
	}

	if( list->length == 1 ){
		return list->tokens[0]->value;
	}

	if( inter_expression_check( temp ) == -1 ){
		printf("(ERROR:inter_expression) Invalid expression. \n");
		return 0;
	}

	inter_expression_reduce( temp );

	inter_expression_resolve( temp );

	if( temp->length != 1 ){
		printf("(ERROR:inter_expression) Expected expression length: 1, found: %d.\n",temp->length);
		list_print(temp);
	}
	
	return temp->tokens[0]->value;
}

int inter_if_statement(struct List *list, int start){
	int if_index = start;
	int then_index = inter_find_next_type(list,'t',if_index+1);
	int end_index = inter_find_end(list,if_index+1);

	if(then_index == -1){
		printf("(ERROR:inter_if_statement) 'if' at %d missing matching 'then'.\n",if_index);
		return 0;
	}

	if(end_index == -1){
		printf("(ERROR:inter_if_statement) 'if' at %d missing matching 'end'.\n",if_index);
		return 0;
	}

	if(end_index<then_index){
		printf("(ERROR:inter_if_statement) 'if' at %d expected 'then' found 'end'.\n",if_index);
		return 0;	
	}

	struct List *exp = list_sub(list,if_index+1,then_index-1);
	int condition_result = inter_expression(exp);

	if(condition_result>=1){
		struct List *statement = list_sub(list,then_index+1,end_index-1);
		return 1;
	}else{
		return 0;
	}
}

int inter_print_statement(struct List *list, int start){
	int open_paren_index = inter_find_next_type(list,'(',start);
	int close_paren_index = inter_find_close_paren(list,open_paren_index+1);

	if(open_paren_index == -1){
		printf("(ERROR:inter_print_statement) 'print' at '%d' missing '('.\n",start);
		return -1;
	}

	if(close_paren_index == -1){
		printf("(ERROR:inter_print_statement) 'print' at '%d' missing '0'.\n",start);
		return -1;
	}

	if(close_paren_index<open_paren_index){
		printf("(ERROR:inter_print_statement) 'print' at '%d' found ')' expected '('.\n",start);
		return -1;
	}

	struct List *exp = list_sub(list,open_paren_index+1,close_paren_index-1);
	
	int result = inter_expression(exp);

	printf("%d\n",result);
}

int inter_list_statement_token(struct List *list, int index){
	struct Token *t = list->tokens[index];
	char type = t->type;

	if( type == 'i' ){
		inter_if_statement(list, index);
	}

	if( type == 'p' ){
		inter_print_statement(list, index);
	}
}

// Search a list for a statement
int inter_list_statement(struct List *list){
	int i;
	for(i=0; i<list->length; i++){
		struct Token *t = list->tokens[i];
		if( char_is_match(t->type,INTER_STATEMENTS) ){
			inter_list_statement_token(list,i);
		}
	}
}


