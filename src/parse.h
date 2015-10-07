#ifndef PARSE_H
#define PARSE_H

int parse_find_number_end(char *str, int start);
int parse_find_word_end(char *str, int start);
int parse_number(char *str, int i);
void parse_char(char *str, int i);
struct List *parse(char *str);

#endif