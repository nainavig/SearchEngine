// Naina Vig
// indexer.c - c file for indexer module


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../common/index.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/set.h"
#include "../libcs50/counters.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"


// used in debugging for printing hashtable
void print_item(FILE *fp, const char *key, void *item){
	counters_print(item, fp);
}


// moves info from docs in directory into webpage structs
webpage_t *
doctoWebpage(FILE *fp){
	char *url;
	char *depth;
	char *html;

	// read first two lines
	url = freadlinep(fp);
	depth = freadlinep(fp);

	// read rest of file
	html = freaduntil(fp, NULL);


	if ((url == NULL) || (depth == NULL) || (html == NULL)){
		return NULL;
	}

	int depth_int = atoi(depth);

	// create webpage
	webpage_t *wp = webpage_new(url, depth_int, html);

	free(depth);

	if (wp == NULL){
		return NULL;
	}
	return wp;

}


hashtable_t *
indexBuilder(char *pageDirectory){

	hashtable_t *index = hashtable_new(800);

	int docID = 1;

	char *docFilename = malloc((strlen(pageDirectory)+1)*sizeof(char) + sizeof(int));

	sprintf(docFilename, "%s/%d", pageDirectory, docID); 


	// loop through files based on docID
	FILE *fp;
	while ((fp = fopen(docFilename, "r")) != NULL){
		
		// move info into webpage
		webpage_t *wp = doctoWebpage(fp);


		// loop through words in webpage, add doc to word counts 
		int pos = 0;
		char *word;
		while ((word = webpage_getNextWord(wp, &pos)) != NULL){

			// only consider words of len > 2
			if (strlen(word) > 2){

				char *normalizedWord = normalizeWord(word);	
				counters_t *counters = hashtable_find(index, normalizedWord);
				if (counters == NULL){
					counters = counters_new();
					hashtable_insert(index, normalizedWord, counters);
				}
				counters_add(counters, docID);
				//free(normalizedWord);
			}
			free(word);
		}

		// close file/free file pointer
		fclose(fp);		

		// set docFilename to next file
		docID++;
		sprintf(docFilename, "%s/%d", pageDirectory, docID);
	
		// free memory	
		webpage_delete(wp);
	}

	free(docFilename);
	return index;
}

int main(int argv, char **argc){

	if (argv != 3){
		printf("usage: ./indexer pageDirectory indexFilename");
		return 1;
	}

	char *pageDirectory = argc[1];
	char *indexFilename = argc[2];


	// if pageDirectory is not a directory
	if (pagedir(pageDirectory, ".indexer") == false){
		printf("pageDirectory invalid");
		return 1;
	}
	// if file can't be opened
	FILE *fp = fopen(indexFilename, "w");
	if (fp == NULL){
		printf("indexFilename invalid\n");
		return 1;
	}

	hashtable_t *index = indexBuilder(pageDirectory);

	indexSave(index, fp);

	hashtable_delete(index, counters_delete);
	fclose(fp); 
	return 0;
}

