#ifndef LINEAR_INTERPRETER_H
#define LINEAR_INTERPRETER_H

#include "list.h"
#include "token.h"

int li_resolve_tokens(struct Token *ta,struct Token *op,struct Token *tb);
int li_resolve_list(struct List *list);
int li_find_type(struct List *list, char type, int start);
int li_find_operator(struct List *list, int i);
int li_check_expression(struct List *list, int start, int end);
int li_print(struct List *list, int index);
int li_if(struct List *list, int i);
int li_token(struct List *list, int i);
void li_statement(struct List *list);

#endif