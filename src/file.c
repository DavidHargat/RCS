#include <stdlib.h>
#include <stdio.h>

/*
* Reads an entire file to a string.
* Returns a pointer to said string. 
*/

char *file_read_string(char *filename){
	FILE *f;
	
	// Open file
	f = fopen(filename,"r");
	
	// Check file exists.
	if ( !f ){
		printf("file::file_read_string() File Not Found.\n");
		return 0;
	}
	
	// Get the size of the file (number of characters)
	int file_size = 0;
	fseek( f, 0L, SEEK_END ); // Set position in stream to the end.
	file_size = ftell(f)+1;// -4 for some reason (I'm so sorry)
	
	// Reset our position in stream.
	fseek(f, 0L, SEEK_SET);
	
	// Initialize a buffer to read the file into.
	char *buf = malloc((file_size)*sizeof(char));
	
	int i;
	for( i=0; i<file_size; i++){
		buf[i] = fgetc( f );
	}
	buf[file_size-1] = '\0';
		
	fclose( f );
	
	return buf;
}