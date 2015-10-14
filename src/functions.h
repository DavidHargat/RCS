#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "token.h"
#include "list.h"

// takes n arguments
int function_print(struct Token *statement);
int function_if(struct Token *statement);
int function_execute(struct Token *statement);
void function_execute_list(struct List *statements);

#endif