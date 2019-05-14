// Naina Vig
// querier.c


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "file.h"
#include "hashtable.h"
#include "set.h"
#include "counters.h"
#include "index.h"
#include "word.h"


/*************************** union and intersection utilities ************************/


// wraps two counters in struct to pass to iterator helper function
typedef struct two_cntrs{
	counters_t *c1;
	counters_t *empty;
} two_cntrs_t;

// helper function for counters_intersect
// for each (key, count) in counters, search c1 for key
// if key in c1, add the key and the minimum of the two counters to counters
void inter_helper(void *arg, const int key, const int count){

	//initialize variables
	two_cntrs_t *t = arg;
	counters_t *c1 = t->c1;
	counters_t *empty = t->empty;
	int count1;

	// check if key is in c1
	if ((count1 = counters_get(c1, key)) != 0){
		// set key to the minimum of two counts in new counters set
		if (count < count1){
			counters_set(empty, key, count);
		} else {
			counters_set(empty, key, count1);
		}
	}
}

// computes counters intersection
counters_t *counters_intersect(counters_t *counters, counters_t *c_word){
	// create new counters set for intersection
	counters_t *new_intersection = counters_new();

	// create new two_cntrs struct to hold new intersection and word counters
	two_cntrs_t *t = malloc(sizeof(two_cntrs_t));
	t->c1 = c_word;
	t->empty = new_intersection;
	counters_iterate(counters, t, inter_helper);
	free(t);
	return new_intersection;
}

// used for computing counters union
void union_helper(void *arg, const int key, const int count){
	int i = counters_get(arg, key);
	counters_set(arg, key, i+count);
}

// used to take the union of two counters
void counters_union(counters_t *counters, counters_t *new){
	counters_iterate(new, counters, union_helper);
}

// used in debugging for printing hashtable
void print_item(FILE *fp, const char *key, void *item){
        counters_print(item, fp);
}



/************************* string utilities ******************************/

// returns true if pointer points to conjunction defined by backus nor
bool is_conjunction(char *p){
	return ((strcmp(p, "or") == 0) || (strcmp(p, "and") == 0));
}

// returns true if pointer points to empty string or string of all spaces
bool is_empty(char *p){
	return ((*p == '\0') || (*p == ' '));
}

// returns true if pointer points to valid letter or space or end delimiter
bool is_valid(char *p){
	return ((isalpha(*p)) || (*p == ' ') || (*p == '\n') || (*p == '	') || (*p == '\0'));
}



/**************************** display results **********************************/

// used as node in sorted array
typedef struct sort_node {
	int key;
	int count;
} sort_node_t;


// used to store array and index info as arg to set iterate
typedef struct sortedlist {
	sort_node_t ** arr;
	int *i;
} sortedlist_t;

// called by counters_iterate, helps get size of counters
void get_size(void *arg, const int key, const int count){
	*(int*)arg = (*(int*)arg) + 1;
}

// called by counters_iterate, helps sort counters
void sort_helper(void *arg, const int key, const int count){

	// create new node to add to list
	sort_node_t *node = malloc(sizeof(sort_node_t));
	node->key = key;
	node->count = count;

	// get info from arg
	sortedlist_t *list = arg;
	sort_node_t ** arr = list->arr; // list
	int *i = list->i;	// index for list 

	// find index for node
	int j = (*i)-1;
	while (j >= 0 && arr[j]->count > count){
		arr[j + 1] = arr[j];
		j = j - 1;
	}

	// place node in list
	arr[j+1] = node;

	// increment index
	(*i) = (*i) + 1;
}


// sorts counters into array of (key, count) structs
void
sort_and_display(counters_t *counters, char *pageDirectory){

	// get length of counters list
	int c_size = 0;
	counters_iterate(counters, &c_size, get_size);

	// if counters list is empty, don't print anything
	if (c_size == 0){
		printf("No documents match\n");
		return;
	}

	// initialize array
	sort_node_t ** arr = calloc(c_size, sizeof(sort_node_t));

	// store array in list struct and set list struct index to 0
	sortedlist_t *list = malloc(sizeof(sortedlist_t));
	list->arr = arr;
	int j = 0;
	list->i = &j; 

	// fill sorted array
	counters_iterate(counters, list, sort_helper);


	char *docFilename = malloc((strlen(pageDirectory)+1)*sizeof(char) + sizeof(int));

	// loop through array and print elements, clean up data structures
	int k;
	printf("Matches %d documents (ranked):\n", *(list->i));
	for (k = 0; k < *(list->i); k++){

		// get filename for doc 		
        	sprintf(docFilename, "%s/%d", pageDirectory, arr[k]->key);

		// open file and get first line
		FILE *fp;
		if ((fp = fopen(docFilename, "r")) == NULL){
			printf("Error opening file %s\n", docFilename);
			return;
		}
		char *URL = freadlinep(fp);
		// close file
		fclose(fp);

		//print results
		printf("score %5d document %3d: %s\n", arr[k]->count, arr[k]->key, URL);

		// clean up array while iterating
		free(arr[k]);
		free(URL);
	}

	free(docFilename);
	free(arr);
	free(list);
	
	return;
}



/*************************** main algorithm ***************************************/


// struct to hold array of pointers to words and total number of words in query
typedef struct parsed_line {
	char ** arr;
	int num_words;
} parsed_line_t;


// given a pointer to a string, parse_input returns parsed_line struct containing array of pointers and num_words
parsed_line_t *
parse_input(char *line){
	parsed_line_t *s = malloc(sizeof(parsed_line_t)); 
	char **arr = calloc(50, sizeof(char*));
	
	//normalized line
	//char *n_line = normalizeWord
	
	char *p1 = line;
	char *p2 = line;
	
	// maintains pos in array
	int i = 0;	

	while (*p2 != '\n'){

		// if line contains invalid input character raise error
		if (!is_valid(p2)){
			printf("Error: %c is not a valid character for input\n", *p2);
			free(arr);
			free(s);
			return NULL;
		}

		// reach end of word
		if ((*p2 == ' ')){
			*p2 = '\0';
			// ensure p1 not empty space (will happen when space in input)
			if (!is_empty(p1)){
				arr[i] = p1;
				i++;
			}
			p1 = p2+1;
		}
		p2++;
	}

	
	// check ending word
	*p2 = '\0';
	if (!is_empty(p1)){
		arr[i] = p1;
		i++;	
	}


	int j;
	printf("Query: ");
	for (j = 0; j < i; j++){
		printf("%s ", arr[j]);
	}
	printf("\n");
		
	s->arr = arr;
	s->num_words = i;

	return s;
}


// querier contains the main logic for taking the unions and intersections of queries
counters_t *
querier(hashtable_t *index, parsed_line_t *s){

	// maintains whether conjunction is last word seen
	bool conj_last_seen = false;

	// intialize union and intersect counters sets
	counters_t *c_union = counters_new();
	counters_t *c_intersect = NULL;

	for (int i = 0; i < s->num_words; i++){
		// edge case: first word is conjunction
		if ((i == 0) && (is_conjunction(s->arr[i]))){
			printf("Error: '%s' cannot be first\n", s->arr[i]);
			// free data structures before returning
			counters_delete(c_union);
			counters_delete(c_intersect);
			return NULL;
		}

		// if conjunction, adjust counters sets
		if (is_conjunction(s->arr[i])){
			// bad case: two conjunctions in a row
			if (conj_last_seen) {
				printf("Error: cannot have two adjacent conjunctions\n");
				// free data structures before returning
				counters_delete(c_union);
				counters_delete(c_intersect);
				return NULL;
			}

			conj_last_seen = true;

			// if "or" seen, add c_intersect to union set and reset to NULL
			if (strcmp(s->arr[i], "or") == 0){
				counters_union(c_union, c_intersect);
				counters_delete(c_intersect);
				c_intersect = NULL;
			}

		// valid word seen, look up in index
		} else {
			counters_t *counters = hashtable_find(index, s->arr[i]);
			if (counters != NULL){
				// if intersection uninitialized, set to counters
				if (c_intersect == NULL){
					//c_intersect = duplicate_counters(counters);
					c_intersect = counters_new();
					counters_union(c_intersect, counters);
				// otherwise, take intersection of counters with current intersect counters set
				} else {
					counters_t *new_intersection = counters_intersect(c_intersect, counters);
					counters_delete(c_intersect);
					c_intersect = new_intersection;
				}
			} else {
				counters_delete(c_intersect);
				c_intersect = counters_new();
			}
			conj_last_seen = false;	
		}
	}

	// check if query ends with conjunction	
	if (conj_last_seen){
		printf("Error: cannot end query with conjunction\n");
		// free data structures before returning
		counters_delete(c_union);
		counters_delete(c_intersect);
		return NULL;	
	}

	// take final union of intersection and union sets
	if (c_intersect != NULL){	
		counters_union(c_union, c_intersect);
		counters_delete(c_intersect);
	}
	return c_union;
}



int main(int argc, char **argv){

	// verify number of arguments
	if (argc != 3){
		printf("usage: ./querier pageDirectory indexFilename\n"); 
		return 1;
	}

	// make sure index file can be opened
	FILE *fp = fopen(argv[2], "r");
	if (fp == NULL){
		printf("Error: indexFilename cannot be opened\n");
		return 1;
	}

	// load file into index
	hashtable_t *index = indexLoad(fp);

	
	char *line = malloc(50);
	printf("Query? ");
	while (fgets(line, 50, stdin) != NULL){
		char *normalized = normalizeWord(line);
		parsed_line_t *s  = parse_input(normalized);

		if (s != NULL){

			counters_t *result = querier(index, s);
		
			if (result != NULL){
				sort_and_display(result, argv[1]);
				counters_delete(result);
			}

			printf("Query? ");
	
			free(s->arr);
			free(s);
		}

		free(normalized);
	}

	// clean up data structures
	free(line);
	fclose(fp);
	hashtable_delete(index, counters_delete);
}


