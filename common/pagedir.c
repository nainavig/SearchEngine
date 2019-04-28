// Naina Vig
// pagedir.c - functionality for pageDirectory verification

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pagedir.h"


/*********************** page dir *****************************/


bool pagedir(char *pageDirectory, char *Fun){

	// allocates memory for size of pagedir string and fun string to be concatenated
	char *filename = malloc(((strlen(pageDirectory)+1)*sizeof(char)) + ((strlen(Fun)+1)*sizeof(char)));

	// copies pagedir string and fun string into memory allocated for filename
        sprintf(filename, "%s/%s", pageDirectory, Fun);

	// attempts to create file at filename
	FILE *fp = fopen(filename, "w");
	
	// frees memory allocated for filename
	free(filename);

	// if file creation failed, return false
	if (fp == NULL){
		return false;
	} else {
	// if file creation succeeded, write to file, close file, and return true
		fprintf(fp, "%s", Fun);
		fclose(fp);
		return true;
	}
}

