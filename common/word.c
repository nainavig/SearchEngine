// Naina Vig
// word.c 

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "word.h"

char *normalizeWord(char *word){
	int len = strlen(word);
	char *wordLower = malloc(len*sizeof(char));

	for (int i = 0; i < len; i++){
		wordLower[i] = tolower(word[i]);
	}
	return wordLower;
}


