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
#define T_FUNCTION 14
#define T_COMMA 15
#define T_STRING 16
#define T_QUOTE 17
#define T_EXPRESSION 18
#define T_LOOKUP 19
#define T_VAR 20
#define T_NOT 21

#define TOKEN_MAP_SIZE 22
#define TOKEN_MAP_TYPES {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21}

static char * const TOKEN_MAP_WORDS[TOKEN_MAP_SIZE] = 
{
	"if",
	"then",
	"end",
	"number",
	"print",
	"(",
	")",
	"list",
	"pointer",
	"+",
	"-",
	"*",
	"/",
	"=",
	"function",
	",",
	"string",
	"\"",
	"expression",
	"lookup",
	"var",
	"not"
};

struct Token{
	int type;
	int value;
	int end;
	struct List *list;
	char *string;
};

void token_print(struct Token *t);
struct Token *token_create(int type, int value);
int token_is_numeric(int type);
int token_is_alphabetic(int type);
int token_is_symbol(int type);
int token_is_statement(int type);

#endif