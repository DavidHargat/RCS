#ifndef LIST_H
#define LIST_H

#define LIST_EXPRESSION 0
#define LIST_STATEMENT 1
#define LIST_IF 2
#define LIST_PRINT 3
#define LIST_ASSIGNMENT 4

struct List{
	struct Token **tokens;
	char type;
	int length;
};

struct List *list_create();
void list_print(struct List *list);
void list_resize(struct List *list, int l);
void list_add(struct List *list, struct Token *t);
void list_remove(struct List *list, int index);
void list_replace(struct List *list, int index, struct Token *t);
void list_insert(struct List *list, struct Token *t, int index);
struct List *list_copy(struct List *list);
struct List *list_sub(struct List *list, int start, int end);

#endif