#ifndef LINEAR_INTERPRETER_H
#define LINEAR_INTERPRETER_H

#include "list.h"
#include "token.h"

int inter_operation(struct Token *ta, struct Token *op, struct Token *tb);
int inter_expression(struct List *list);
int inter_find_end(struct List *list, int start);
int inter_find_next_type(struct List *list, int type, int start);
int inter_find_close_paren(struct List *list, int start);
int inter_expression_check(struct List *list);
int inter_expression(struct List *list);
int inter_if_statement(struct List *list, int start);
int inter_list_statement_token(struct List *list, int index);
int inter_list_statement(struct List *list);
struct List *inter_list_to_statement(struct List *list);

struct Token *inter_statement_if(struct List *list, int index);
struct Token *inter_statement_function(struct List *list, int index);
struct Token *inter_statement_assignment(struct List *list, int index);

#endif