#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "list.h"
#include "parse.h"
#include "char.h"

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

int inter_statement(struct List *list){

}