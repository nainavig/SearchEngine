// Naina Vig
// index.c - index utility functions
//
#include <stdio.h>
#include <stdlib.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/set.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"
#include "index.h"



// used as itemfunc for counters_iterate
void 
writeCountersPair(void *arg, const int key, const int count){
        fprintf(arg, "%d %d ", key, count);
}


// used as itemfunc for hashtable_iterate
void 
writeHashtablePair(void *arg, const char *key, void *item){
        fprintf(arg, "%s ", key);
        counters_iterate(item, arg, writeCountersPair);
        fprintf(arg, "\n");
}

// writes index to file
void 
indexSave(hashtable_t *index, FILE *fp){
        hashtable_iterate(index, fp, writeHashtablePair);
}


// loads file contents into index
hashtable_t *
indexLoad(FILE *fp){

	if (fp == NULL){
		printf("invalid file pointer\n");
		return NULL;
	}

        hashtable_t *index = hashtable_new(100);

        int lines = lines_in_file(fp);
        int i = 0;

	// loop through each line in file
        while (i < lines){

		// read first word in line
                char *word = freadwordp(fp);

		// initialize counters and insert word into hashtable
		counters_t *counters = counters_new();
                hashtable_insert(index, word, counters);

                int docID;
                int count;
                int output;

		// add counts
                while ((output = fscanf(fp, "%d %d ", &docID, &count)) == 2){
                        counters_set(counters, docID, count);
                }
                i++;
		free(word);
        }

        return index;
}
