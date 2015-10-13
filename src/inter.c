#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "list.h"
#include "parse.h"
#include "char.h"
#include "inter.h"

#define INTER_STATEMENTS {T_IF,T_PRINT,T_EQUALS}
#define INTER_STATEMENTS_LENGTH 3
// BEGIN HELPER FUNCTIONS

// substitution will be implimented here.
int inter_operation(struct Token *ta, struct Token *op, struct Token *tb){
	int type = op->type;
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
int inter_find_next_type(struct List *list, int type, int start){
	int i;
	for(i=start;i<list->length;i++){
		if( list->tokens[i]->type == type )
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
		int type = t->type;
		if( type==T_IF )
			nested_ifs++;
		if( type==T_END){
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
		int type = t->type;
		if( type == T_OPEN_PAREN )
			nested_parens++;
		if( type == T_CLOSE_PAREN ){
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
		if( list->tokens[0]->type != T_NUMBER ){
			printf("(ERROR:inter_expression_check) Expression is non-numeric '%c'.\n", list->tokens[0]->type);
			return -1;
		}
	}

	int i;
	for(i=0; i<list->length; i++){
		struct Token *t = list->tokens[i];
		if( token_is_operator(t->type) ){
			if( i+1 < list->length ){
				struct Token *left=list->tokens[i-1],*right=list->tokens[i+1];
				if( left->type != T_NUMBER && left->type != T_CLOSE_PAREN ){
					printf("(ERROR:inter_expression_check) Operator with invalid operand '%d %d'.\n", right->type, t->type);
					return -1;
				}
				if( right->type != T_NUMBER && right->type != T_OPEN_PAREN ){
					printf("(ERROR:inter_expression_check) Operator with invalid operand '%d %d'.\n", t->type, right->type);
					return -1;
				}
			}else{
				printf("(ERROR:inter_expression_check) Operator at end of token stream");
				return -1;
			}
		}
	}
}

// Reduces parenthesis of given expression
// Mutation
int inter_expression_reduce(struct List *temp){
	int open_paren_index;
	while( (open_paren_index = inter_find_next_type(temp,T_OPEN_PAREN, 0)) != -1 ){
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
				struct Token *result_token = token_create(T_NUMBER,result); // replace close paren with result
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
		int type = temp->tokens[i]->type;
		if( token_is_operator(type) ){
			int result = inter_operation(temp->tokens[i-1],temp->tokens[i],temp->tokens[i+1]);
			struct Token *result_token = token_create(T_NUMBER,result);
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

struct Token *inter_word_if(struct List *list, int index){
	// if, expression, statement
	struct Token *word = token_create(T_LIST, LIST_IF);
	word->list = list_create();

	// Find matching keywords: if, then, end
	int if_index   = index;
	int then_index = inter_find_next_type(list,T_THEN,if_index);
	int end_index  = inter_find_end(list,then_index);
	word->end = end_index;

	// Error checking
	if(then_index==-1){
		printf("(ERROR::inter_word_if) 'if' missing 'then' at %d\n",if_index);
		return NULL;
	} if(end_index==-1){
		printf("(ERROR::inter_word_if) 'if' missing 'end' at %d\n",if_index);
		return NULL;
	} if(end_index<then_index){
		printf("(ERROR::inter_word_if) found 'end' expected 'then' at %d\n",end_index);
		return NULL;
	}


	// Extract expression
	struct Token *expression = token_create(T_LIST,LIST_EXPRESSION);
	expression->list = list_sub(list,if_index+1,then_index-1);

	// Extract statement
	struct Token *statement = token_create(T_LIST,LIST_STATEMENT);
	statement->list = inter_list_to_statement(list_sub(list,then_index+1,end_index-1));

	// Extract type
	//struct Token *type = list->tokens[index];
	
	// Add segments to our 'word'
	//list_add(word->list, type);
	list_add(word->list, expression);
	list_add(word->list, statement);

	return word;
}

struct Token *inter_word_print(struct List *list, int index){
	// print, expression
	struct Token *word = token_create(T_LIST, LIST_PRINT);
	word->list = list_create();

	struct Token *type, *expression;
	expression = token_create(T_LIST, LIST_EXPRESSION);

	int open_paren_index = index+1;
	int close_paren_index = inter_find_close_paren(list,open_paren_index+1);
	word->end = close_paren_index;

	if(list->tokens[open_paren_index]->type != T_OPEN_PAREN){
		printf("(ERROR::inter_word_print) 'print' missing '(' at %d\n",index);
		return NULL;
	} if(close_paren_index == -1){
		printf("(ERROR::inter_word_print) 'print' missing ')' at %d\n",index);
		return NULL;
	}

	expression->list = list_sub(list,open_paren_index+1,close_paren_index-1);

	//type = list->tokens[index];
	//list_add(word->list, type);
	list_add(word->list, expression);

	return word;
}

struct Token *inter_word_assignment(struct List *list, int index){
	// =, identifier, expression
	struct Token *word = token_create(T_LIST, LIST_ASSIGNMENT);
	word->list = list_create();

	struct Token *type, *identifier, *expression;
	type       = list->tokens[index];
	identifier = list->tokens[index-1];
	expression = list->tokens[index+1];
	word->end = index+1;

	list_add(word->list,type);
	list_add(word->list,identifier);
	list_add(word->list,expression);

	return word;
}

struct Token *inter_word(struct List *list, int index){
	int type = list->tokens[index]->type;

	if( type == T_IF )
		return inter_word_if(list,index);
	
	if( type == T_PRINT )
		return inter_word_print(list,index);

	if( type == T_EQUALS )
		return inter_word_assignment(list,index);
}

struct List *inter_list_to_statement(struct List *list){
	int STATEMENTS[INTER_STATEMENTS_LENGTH] = INTER_STATEMENTS;

	struct List *statement;
	statement = list_create();

	list_print(list);

	int i;
	for(i=0; i<list->length; i++){
		struct Token *t = list->tokens[i];
		if( token_is_match(t->type,STATEMENTS,INTER_STATEMENTS_LENGTH) ){
			struct Token *word = inter_word(list,i);
			i = word->end; // Jump forward to the end of the 'word'
			list_add(statement,word);
		}
	}

	return statement;
}

/*
// Search a list for a statement
int inter_list_statement(struct List *list){
	int i;
	for(i=0; i<list->length; i++){
		struct Token *t = list->tokens[i];
		if( char_is_match(t->type,INTER_STATEMENTS) ){
			int skip_to = inter_list_statement_token(list,i);
			if(skip_to != -1){
				i = skip_to - 1;
			}else{
				// Did not evaluate
			}
		}
	}
}*/


