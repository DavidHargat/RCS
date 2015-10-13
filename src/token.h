#ifndef TOKEN_H
#define TOKEN_H

#define T_IF 0
#define T_THEN 1
#define T_END 2
#define T_NUMBER 3
#define T_PRINT 4
#define T_OPEN_PAREN 5
#define T_CLOSE_PAREN 6
#define T_LIST 7
#define T_POINTER 8
#define T_ADD 9
#define T_SUBTRACT 10
#define T_MULTIPLY 11
#define T_DIVIDE 12
#define T_EQUALS 13


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
	int type;
	int value;
	int end;
	struct List *list;
};

void token_print(struct Token *t);
struct Token *token_create(int type, int value);
int token_is_numeric(int type);
int token_is_alphabetic(int type);
int token_is_symbol(int type);
int token_is_statement(int type);


#endif