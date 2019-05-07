// Naina Vig
// word.c 
//
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "word.h"

char *normalizeWord(char *word){
	char *wordLower = malloc(sizeof(word)+1);

	int i;
	for (i = 0; i < strlen(word); i++){
		wordLower[i] = tolower(word[i]);
	}
	wordLower[i] = '\0';
	return wordLower;
}

