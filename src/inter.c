#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "list.h"
#include "parse.h"
#include "char.h"
#include "inter.h"
#include "functions.h"

#define INTER_STATEMENTS {T_IF,T_PRINT,T_EQUALS,T_ADD,T_SUBTRACT,T_MULTIPLY,T_DIVIDE}
#define INTER_STATEMENTS_LENGTH 7

const int STATEMENTS[INTER_STATEMENTS_LENGTH] = INTER_STATEMENTS;

// BEGIN HELPER FUNCTIONS

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

struct Token *inter_statement_string(struct List *list, int index){
	struct Token *word = token_create(T_LIST,LIST_FUNCTION);
	list_add(word->list,token_create(T_STRING,0));
	list_add(word->list,list->tokens[index]);
	word->end = index;
	return word;
}

struct Token *inter_statement_expression(struct List *list, int index){
	struct Token *word = token_create(T_LIST, LIST_FUNCTION);

	int expression_end = -1;

	int i;
	for(i=index;i<list->length;i++){
		char t = list->tokens[i]->type;
		if( !(token_is_operator(t)||t==T_OPEN_PAREN||t==T_CLOSE_PAREN||t==T_NUMBER) ){
			expression_end = i;
			break;
		}
		if( i == list->length-1){
			expression_end = i+1;
		}
	}


	if(expression_end == -1)printf("(ERROR:inter_statement_expression) Could not find end of expression\n");

	word->end = expression_end-1;
	word->list = list_sub(list,index,expression_end-1);
	list_insert(word->list,token_create(T_EXPRESSION,LIST_FUNCTION),0);

	return word;
}

struct Token *inter_statement_if(struct List *list, int index){
	// if, expression, statement
	struct Token *word = token_create(T_LIST, LIST_FUNCTION);

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
	struct Token *expression = token_create(T_LIST,LIST_FUNCTION);
	expression = inter_statement(list,if_index+1);
	//list_insert(expression->list, token_create(T_EXPRESSION,0),0); // Add the header

	// Extract statement
	struct Token *statement = token_create(T_LIST,LIST_STATEMENT);
	statement->list = inter_list_to_statement(list_sub(list,then_index+1,end_index-1));

	// Extract type
	struct Token *header = list->tokens[index];
	
	// Add segments to our 'word'
	list_add(word->list, header);
	list_add(word->list, expression);
	list_add(word->list, statement);

	return word;
}

struct Token *inter_statement_operator(struct List *list, int index){
	struct Token *word = token_create(T_LIST,LIST_FUNCTION);

	word->end = index+1;

	struct Token *header,*left,*right;
	header = list->tokens[index];
	left   = list->tokens[index-1];
	right  = list->tokens[index+1];

	// If the token to the right isn't a number
	// we need to recursively parse it as a function
	if ( right->type != T_NUMBER ) {
		right = inter_statement(list,index+1);
		word->end = right->end;
	}

	//1+2+3	
	if( right->type == T_NUMBER && list->length > index+2 ){
		
		struct Token *right_right = list->tokens[index+2];
		
		if( token_is_operator(right_right->type) ){
			right = inter_statement_operator(list,index+2);
			word->end = right->end;
		}
		
		//right = inter_statement(list,index+1);
		//word->end = right->end;

	}

	// Operands
	list_add(word->list, header); // HEADER
	list_add(word->list, left); 
	list_add(word->list, right);


	return word;
}

struct Token *inter_statement_assignment(struct List *list, int index){
	struct Token *word = token_create(T_LIST,LIST_FUNCTION);

	struct Token *header, *name, *value;

	header = list->tokens[index];
	name   = list->tokens[index-1];
	value  = inter_statement(list,index+1);

	word->end = value->end;

	struct Token *nametoken = token_create(T_STRING,0);
	nametoken->string = name->string;

	list_add(word->list,header);
	list_add(word->list,nametoken);
	list_add(word->list,value);

	return word;
}

struct Token *inter_statement_var(struct List *list, int index){
	struct Token *word = token_create(T_LIST,LIST_FUNCTION);

	struct Token *str = token_create(T_STRING,0);
	str->string = list->tokens[index]->string;
	
	list_add(word->list, token_create(T_LOOKUP, 0)); // Header
	list_add(word->list, str);

	word->end = index;

	return word;
}

// construct function syntax
struct Token *inter_statement_function(struct List *list, int index){
	// Create a new 'statement' token
	struct Token *statement = token_create(T_LIST,LIST_FUNCTION);
	// Grab the header token so we can append it to the statement
	struct Token *header = list->tokens[index];
	// Find/check the parenthesis
	int open_paren_index,close_paren_index;
	open_paren_index  = index+1;
	close_paren_index = inter_find_close_paren(list,open_paren_index+1);
	// ERROR CHECKING
	if ( list->tokens[open_paren_index]->type != T_OPEN_PAREN ){
		printf("(ERROR:inter_statement_function) Expected '(' after function found '%s' %d!=%d \n",
		parse_type_to_word(list->tokens[open_paren_index]->type),list->tokens[open_paren_index]->type,T_OPEN_PAREN);
	}else if(close_paren_index == -1){
		printf("(ERROR:inter_statement_function) Expected ')' after function.\n");
	}
	// Add the header+arguments to the 'statement'
	list_add(statement->list, header);

	int parenthesis_skips = 0;
	// Extract the 'arguments' from between the parenthesis
	int i, start = open_paren_index+1, end = close_paren_index-1, last = start;
	for(i=start; i<=end; i++){
		int t = list->tokens[i]->type;
		
		if( t == T_OPEN_PAREN )
			parenthesis_skips++;
		if( t == T_CLOSE_PAREN )
			parenthesis_skips--;
		if( parenthesis_skips>0 )
			continue;

		if( i == end || t == T_COMMA){
			struct Token *arg = token_create(T_LIST,LIST_FUNCTION);
			
			struct List *arg_segment;
			if( i == end ){
				arg_segment = list_sub(list,last,i);
			}else{
				arg_segment = list_sub(list,last,i-1);
			}

			arg = inter_statement(arg_segment,0);
			
			// Ugly ass op handler code
			list_print(arg_segment);
			int op_index = -1;
			int j;
			for(j=0; j<arg_segment->length; j++){
				struct Token *arg_t = arg_segment->tokens[j];
				if(token_is_operator(arg_t->type)){
					op_index = j;
					break;
				}
			}
			if( op_index != -1 ){
				arg_segment->tokens[op_index-1] = arg;
				arg = inter_statement(arg_segment,op_index);
			}
			// Glad thats over.

			list_add(statement->list,arg);

			last = i+1;	
		}
	}
	statement->end = close_paren_index;

	// Return our new statement function
	return statement;
}

struct Token *inter_statement(struct List *list, int index){
	int type = list->tokens[index]->type;

	if( token_is_operator(list->tokens[index]->type) )
		return inter_statement_operator(list,index);

	if( type == T_NUMBER ){
		/* If it's a number
		* check if it's attached
		* to an operator, else
		* return it as an expression */
		if( list->length > index+1 ){
			if( token_is_operator(list->tokens[index+1]->type) ){
				return inter_statement_operator(list,index+1);
			}
		}
		return inter_statement_expression(list,index);
	}

	if( type == T_VAR )
		return inter_statement_var(list,index);

	if( type == T_EQUALS )
		return inter_statement_assignment(list,index);

	if( type == T_IF )
		return inter_statement_if(list,index);
	
	if( type == T_PRINT )
		return inter_statement_function(list,index);

	if( type == T_LOOKUP )
		return inter_statement_function(list,index);

	if( type == T_NOT )
		return inter_statement_function(list,index);

	if( type == T_STRING)
		return inter_statement_string(list,index);


	printf("(ERROR:inter_statement) Invalid statement header '%s' \n",parse_type_to_word(type));

}

struct List *inter_list_to_statement(struct List *list){

	struct List *statements;
	statements = list_create();

	//list_print(list);

	int i;
	for(i=0; i<list->length; i++){
		struct Token *t = list->tokens[i];
		if( token_is_match(t->type,STATEMENTS,INTER_STATEMENTS_LENGTH) ){
			struct Token *statement = inter_statement(list,i);
			i = statement->end; // Jump forward to the end of the 'word'
			list_add(statements,statement);
		}
	}

	return statements;
}