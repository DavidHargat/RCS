#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "token.h"
#include "list.h"

struct List *list_create(){
	struct List *list = malloc(sizeof(struct List));
	list->tokens = malloc(sizeof(struct Token *));
	list->tokens[0] = token_create('#',0);
	list->length = 1;
	list->type   = 0;
	return list;
}

struct List *list_copy(struct List *list){
	struct List *nlist = list_create();
	int size = sizeof(struct Token *)*list->length;
	
	nlist->length = list->length;
	nlist->tokens = malloc(size);
	memcpy(nlist->tokens,list->tokens,size);
	
	return nlist;
}

struct List *list_sub(struct List *list, int start, int end){
	int length = (end-start)+1,
		size = length * sizeof(struct Token *);
	struct List *nlist;
	nlist->tokens = malloc(size);
	memcpy(nlist->tokens,&list->tokens[start-1],size);
	nlist->length = length;

	return nlist;
}

void list_resize(struct List *list, int l){
	list->length = l;
	list->tokens = (struct Token **) realloc(list->tokens, sizeof(struct Token *)*l);
}

void list_add(struct List *list, struct Token *t){
	list_resize(list,list->length+1);
	list->tokens[list->length-1] = t;
}

// Inserts Token* BEFORE given index.
void list_insert(struct List *list, struct Token *t, int index){
	list_resize(list,list->length+1);
	int i;
	for( i=list->length-1; i>=index; i-- ){
		list->tokens[i] = list->tokens[i-1];
	}
	list->tokens[index] = t;
}

void list_replace(struct List *list, int index, struct Token *t){
	list->tokens[index] = t;
}

void list_remove(struct List *list, int index){
	int i;
	for( i=index; i<list->length-1; i++ ){
		list->tokens[i] = list->tokens[i+1];
	}
	list_resize(list,list->length-1);
}

void list_print(struct List *list){
	printf("list:\n");
	int i;
	for(i=0;i<list->length;i++){
		token_print(list->tokens[i]);
	}
	printf("\n");
}