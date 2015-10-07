#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "parse.h"
#include "tree.h"
#include "char.h"

int li_resolve_tokens(struct Token *ta, struct Token *op, struct Token *tb){
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

int li_resolve_list(struct List *list){
	
	int i;
	for( i=0; i<list->length; i++ ){
		struct Token *t = list->tokens[i];
		if( char_is_operator(t->type) ){
			struct Token *a = list->tokens[i-1];
			struct Token *b = list->tokens[i+1];
			
			int result = li_resolve_tokens(a,t,b);
			
			struct Token *r = token_create('#',result);
			list_remove(list,i-1);
			list_remove(list,i-1);
			list->tokens[i-1] = r;
			
			i = 0;
		}
	}
	
	if(list->length > 1){
		printf("(ERROR::li_resolve_list) could not fully li_resolve list.\n");
		list_print(list);
		return 0;
	}else{
		return list->tokens[0]->value;
	}
}

// Li header
int li_find_type(struct List *list, char type, int start){
	int found = -1;
	
	int i = start-1;
	while( i < list->length && found == -1 ){
		i++;
		if( list->tokens[i]->type == type ){
			return i;
		}
	}
	printf("Could not find %c within %d\n",type,list->length);
	
	return -1;
}

int li_find_operator(struct List *list, int i){
	int found = -1;
	
	int index = i;
	while( index < list->length && found == -1 ){
		if( char_is_operator(list->tokens[i]->type) ){
			return index;
		}
		index++;
	}
	
	return -1;
}

int li_check_expression(struct List *list, int start, int end){
	int size = end-start;
	
	int i;
	for(i=start; i<end; i++){
		char type = list->tokens[i]->type;
		
		if( type!='#' && !char_is_operator(type)){
			printf("(ERROR::li_check_expression) %d-%d. (non-operator/number found): %c \n",start,end,type);
			return -1;
		}
		
		if( type == '#' && i<end ){
			char right = list->tokens[i+1]->type;
			if( !char_is_operator(right) ){
				printf("(ERROR::li_check_expression) %d-%d. (number is not paired operator): %c \n",start,end,type);
				return -1;
			}
		}
		
		if( char_is_operator(type) ){
			char left  = list->tokens[i-1]->type;
			char right = list->tokens[i+1]->type;
			if( left == '#' && right == '#'){
				// All good
			}else{
				printf("(ERROR::li_check_expression) %d-%d. (operator is not paired with number): %c \n",start,end,type);
				return -1;
			}
		}
		
	}
	
	return 1;	
}

int li_print(struct List *list, int index){
	struct Token *next = list->tokens[index+1];
	int end = li_find_type(list,')',index+1);
	if( next->type == '(' ){
		if( end != -1 ){
			struct List *expression = list_sub(list,index+1,end-1);
			int result = li_resolve_list(expression);
			printf("->%d\n",result);
		}else{
			printf("(ERROR::li_print) Could not find ')' to match: %d\n",index);
			return -1;
		}
	}else{
		printf("(ERROR::li_print) Expected '(' found: %c\n",next->type);
		return -1;
	}
	return end;
}

void li_statement(struct List *list){
	printf("Statement\n");
	list_print(list);
	int i;
	for(i=0; i<list->length; i++){
		i = li_token(list,i);
	}
}

int li_find_matching_end(struct List *list, int start){
	int skips = 0;
	
	int i = start-1;
	while( i <= list->length ){
		if( list->tokens[i]->type == 'i' ){
			skips++;
		}
		if( list->tokens[i]->type == 'e' ){
			if(skips<=0){
				return i;
			}else{
				skips--;
			}
		}
		i++;
	}
	printf("Could not find matching 'end' for %d\n",i);
	
	return -1;
}

int li_if(struct List *list, int i){
	struct Token *t = list->tokens[i];
	
	int if_index   = i;
	int then_index = li_find_type(list,'t',i);
	int end_index  = li_find_matching_end(list,i+1);
	
	printf("if %d then %d end %d \n",if_index,then_index,end_index);
	
	if( then_index == -1 ){
		printf("(ERROR::li_if) %d 'then' not found. \n",i);
		return;
	}
	
	if( end_index == -1 ){
		printf("(ERROR::li_if) %d 'end' not found. \n",i);
		return;
	}
	
	if( end_index < then_index ){
		printf("(ERROR::li_if) %d found 'end' expected 'then'. \n",i);
		return;
	}
	
	int valid_expression = li_check_expression(list,if_index+1,then_index-1);
	
	if( valid_expression == 1 ){
		
		struct List *exp = list_sub(list,if_index+1,then_index-1);
		int result = li_resolve_list(exp);
		if( result ){
			struct List *statement = list_sub(list,then_index+1,end_index-1);
			li_statement(statement);
		}
		
	}else{
		printf("(ERROR::li_if) %d Invalid Expression.\n",i);
		return;
	}
	
	return end_index;
}

int li_token(struct List *list, int i){
	struct Token *t = list->tokens[i];
	char type = t->type;
	
	if( type == 'i' ){
		return li_if(list,i);
	}
	
	if( type == 'p' ){
		return li_print(list,i);
	}
}
