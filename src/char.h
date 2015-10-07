#ifndef CHARLIB_H
#define CHARLIB_H

// Move to char.c
// *Replace with lookup table.
int char_is_match(int c, char *str);
int char_is_operator(int c);
int char_is_numeric(int c);
int char_is_alphabetic(int c);
int char_is_parenthesis(int c);

#endif