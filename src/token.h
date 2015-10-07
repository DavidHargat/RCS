#ifndef TOKEN_H
#define TOKEN_H

struct Token{
	char type;
	int value;
};

void token_print(struct Token *t);
struct Token *token_create(char type, int value);

#endif