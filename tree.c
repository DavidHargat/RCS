#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tree.h"
#include "list.h"


void node_set_token(struct Node *n, struct Token *t){
	n->token = t;
}

struct Node *node_create(struct Token *t){
	struct Node *n = malloc(sizeof(struct Node));
	n->token  = t;
	n->parent = NULL;
	n->left   = NULL;
	n->right  = NULL;
	return n;
}

int tree_check_expression(struct List *list){
	
	if( list->length % 2 == 0 ){
		printf("(ERROR:tree_check_expression) Expression has even number of tokens.\n");
		return 0;
	}
	
	int i;
	for(i=0;i<list->length;i++){
		// Evens
		if( (i%2)==0 ){
			if( list->tokens[i]->type == '#' ){
				// Correct
			}else{
				printf("(ERROR::tree_check_expression) Found %c expected number.\n",list->tokens[i]->type);
				return 0;
			}
		}else{
			if( char_is_operator(list->tokens[i]->type) ){
				// Correct
			}else{
				printf("(ERROR::tree_check_expression) Found %c expected operator.\n",list->tokens[i]->type);
				return 0;
			}			
		}
	}
	
	return 1;
}

struct Node *tree_exp_node(struct List *list, int i){
	
	char this_type = list->tokens[i]->type;
	
	struct Node  *n = node_create(t);
	
	if( !char_is_operator(this_type) && !this_type == '#'){
		printf("(ERROR:tree_exp_node) non op character '%c'. \n",t->type);
	}
	
	if( i < list->length-1 ){
		char next_type = list->tokens[i+1];
		
		if( t->type == '#' ){
			if( char_is_operator(next_type) ){
				
			}else{
				printf("Found\n");
			}
		}
		
		if( char_is_operator(t->type) ){
			
		}
		
		
	}else{
		// End of stream
	}
	
}

struct Node *tree_exp(struct List *list){
	
	return tree_exp_node(list,0);
	
}

struct Node *tree_create_expression(struct List *list, int i){
	int l = list->length;
	
	if(l<=0){
		printf("(ERROR::tree_create_expression) Empty Expression. \n");
		return NULL;
	}
	
	if(l==1){
		return node_create(list->tokens[0]);
	}
	
	struct Node *base;
	
	if( tree_check_expression(list) ){
		if( i%2 != 0 ){
			struct Node *op = node_create(list->tokens[i]);
			node_set_left(op,node_create(list->tokens[i-1]));
			if( i+2 == (l) ){
				node_set_right(op,node_create(list->tokens[i+1]));
			}else{
				node_set_right(op,tree_create_expression(list,i+2));
			}
			base = op;
		}
	}else{
		printf("(ERROR::tree_create_expression) Invalid Expression. \n");
		return NULL;
	}
	
	return base;
}

void tree_print(struct Node *base, int indent){
	
	token_print(base->token);
	
	int i;
	
	if(base->has_left){
		printf("\n");
		for(i=0;i<indent;i++){
			printf(":::");
		}
		printf("L ");
		tree_print(base->left,i+1);
	}
	
	if(base->has_right){
		printf("\n");
		for(i=0;i<indent;i++){
			printf(":::");
		}
		printf("R ");
		tree_print(base->right,i+1);		
	}
	
}