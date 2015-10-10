#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "list.h"
#include "parse.h"
#include "char.h"
#include "inter.h"

// TO DO1
// * Build a tree
//glib
//string manipulation

void eval(char *str){
	struct List *list = parse(str); // Parse string to tokens array
	list_print(list); // Print token array
	inter_list_statement(list); // Interpret.
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