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


void eval(char *str){
	struct List *list = parse("1+2+3+4+5+6+7+8");

	
	list_print(list_sub(list,1,3));
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