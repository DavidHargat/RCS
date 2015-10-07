#ifndef TREE_H
#define TREE_H

#include "token.h"
#include "list.h"

struct Node{
	struct Token *token;
	struct Node *left;
	struct Node *right;
	struct Node *parent;
};

void node_set_left(struct Node *n, struct Node *left);
void node_set_right(struct Node *n, struct Node *right);
void node_set_token(struct Node *n, struct Token *t);
struct Node *node_create(struct Token *t);
struct Node *tree_create_expression(struct List *list, int i);
void tree_print(struct Node *base, int indent);

#endif