#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "list.h"
#include "parse.h"
#include "char.h"
#include "inter.h"
#include "functions.h"
#include "token.h"


// TO DO1
// * Build a tree
//glib
//string manipulation

void eval(char *str){
	struct List *list = parse(str); // Parse string to tokens array

	list_print(list);

	struct List *tree = inter_list_to_statement(list); // recursively parse tokens to a list of statement (tree)

	printf("Tree: %d\n",tree->length);
	list_print_tree(tree);

	printf("Running...\n");
	function_execute_list(tree,tree);
}

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