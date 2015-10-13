#ifndef TOKEN_H
#define TOKEN_H

/*
* TYPES
* Standard
* 	'i' - if
* 	't' - then
* 	'e' - end
* 	'#' - number
* 	'p' - print
* 	'(' - open parenthesis
* 	')' - close parenthesis
* Grammar
* 	'l' - list
* 	values
* 		0 - expression
* 		1 - function statement
* 		2 - if statement
*		3 - 
* Pointer
*   '.' - pointer
*/

struct Token{
	char type;
	int value;
	int end;
	struct List *list;
};

void token_print(struct Token *t);
struct Token *token_create(char type, int value);

#endif