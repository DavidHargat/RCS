#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "token.h"
#include "list.h"

// takes n arguments
struct Token *function_print(struct Token *statement);
struct Token *function_if(struct Token *statement);
struct Token *function_execute(struct Token *statement);
struct Token *function_expression(struct Token *statement);
void function_execute_list(struct List *statements);

#endif