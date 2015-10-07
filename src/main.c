#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "list.h"
#include "parse.h"
#include "tree.h"
#include "char.h"
#include "linear_interpreter.h"
// TO DO1
// * Build a tree


void eval(char *str){
	struct List *list = parse("1+2+3+4+5+6+7+8");
	
	printf("\n\n");
	
	list_print(list);
	
	li_statement(list);
	//tree_check_expression(list);
	//struct Node *tree = tree_create_expression(list,1);
	//tree_print(tree,1);
	//printf("\n");
}

//glib
//string manipulation

int main(int argc, char *argv[]){
	
	char *filename = argv[1];

	if(filename){
		int l = strlen(filename);
		char *input = file_read_string(filename);
		if(input){
			eval(input);	
		}else{
			printf("Could not read file '%s'. \n",filename);
		}
	}else{
		printf("No filename given, try 'rcs <filename>'. \n");
	}
	
    return 0;
}