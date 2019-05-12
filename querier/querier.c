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



/*************************** union and intersection utilities ************************/


// used to get intersection of counters
typedef struct two_cntrs{
	counters_t *c1;
	counters_t *empty;
} two_cntrs_t;

// used to get min count for intersection
void iter_func(void *arg, const int key, const int count){
	two_cntrs_t *t = arg;
	counters_t *c1 = t->c1;
	int count1;
	if ((count1 = counters_get(c1, key)) != 0){
		if (count < count1){
			counters_set(t->empty, key, count);
		} else {
			counters_set(t->empty, key, count1);
		}
	}
}

// computes counters intersection
counters_t * counters_intersect(counters_t *counters, counters_t *new){
	two_cntrs_t *t = malloc(sizeof(two_cntrs_t));
	t->c1 = new;
	t->empty = counters_new();
	counters_iterate(counters, t, iter_func);
	return t->empty;
}

// used for computing counters union
void union_func(void *arg, const int key, const int count){
	int i = counters_get(arg, key);
	counters_set(arg, key, i+count);
}

// used to take the union of two counters
void counters_union(counters_t *counters, counters_t *new){
	counters_iterate(new, counters, union_func);
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


void
display_results(counters_t *counters, char *pageDirectory){
	counters_print(counters, stdout);
}



/*************************** main algorithm ***************************************/

counters_t *
querier(hashtable_t *index, char *line){
	char *p1 = line;
	char *p2 = line;


	counters_t *c_union = counters_new();
	counters_t *c_intersect = NULL;


	// used to determine whether line starts, ends, or has duplicate conjunctions
	bool conjunction_last_seen = true;

	while (*p2 != '\n'){
			
		// if line contains invalid input character raise error
		if (!is_valid(p2)){
			printf("Error: only enter alphabetical characters and spaces\n");
			return NULL;
		}

		// reach end of word
		if ((*p2 == ' ')){
			*p2 = '\0';
			// ensure p1 not empty space (will happen when space in input)
			if (!is_empty(p1)){
				// check if p1 is conjunction
				if (is_conjunction(p1)) {
					if (conjunction_last_seen) {
						printf("Error: cannot have two adjacent conjunctions\n");
						return NULL;
					}
					conjunction_last_seen = true;
					if (strcmp(p1, "or") == 0){
						counters_union(c_union, c_intersect);
						c_intersect = NULL;
					}
				} else {
					// valid word retrived
					printf("%s\n", p1);
					counters_t *counters = hashtable_find(index, p1);
					if (counters != NULL){
						if (c_intersect == NULL){
							c_intersect = counters;
						} else {
							c_intersect = counters_intersect(c_intersect, counters);
						}
					}
						conjunction_last_seen = false;
				}
			}
			// initialize start to new word
			p1 = p2+1;
		}
		p2++;
	}
	// check ending word
	if (p1 != line){
		*p2 = '\0';
		if (!is_empty(p1)){
			conjunction_last_seen = is_conjunction(p1);
		}
		if (conjunction_last_seen){
			printf("Error: can't end with conjunction\n");
			return NULL;
		}
		if (!is_empty(p1)){
			printf("%s", p1);
			counters_t *counters = hashtable_find(index, p1);
			if (c_intersect == NULL){
				c_intersect = counters;
			} else {
				c_intersect = counters_intersect(c_intersect, counters);
			}
		}
	}
	counters_union(c_union, c_intersect);
	return c_union;
	
}


char **parse_input(char *line){
	char **arr = malloc(20*sizeof(char*));
	
	//normalized line
	//char *n_line = normalizeWord
	
	char *p1 = line;
	char *p2 = line;
	
	// maintains pos in array
	int i = 0;	

	while (*p2 != '\n'){

		// if line contains invalid input character raise error
		if (!is_valid(p2)){
			printf("Error: only enter alphabetical characters and spaces\n");
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
	if (p1 != line){
		*p2 = '\0';
		if (!is_empty(p1)){
			arr[i] = p1;	
		}
	}

	for (int j = 0; j <= i; j++){
		printf("%s\n", arr[j]);
	}

	return arr;
}

int main(int argc, char **argv){
	if (argc != 3){
		printf("usage: ./querier pageDirectory indexFilename\n"); 
	}
	FILE *fp = fopen(argv[2], "r");
	hashtable_t *index = indexLoad(fp);

//	hashtable_print(index, stdout, print_item);
	
	char *line = malloc(50);
	printf("Query? ");
	while (fgets(line, 50, stdin) != NULL){
		char **arr = parse_input(line);

	//	counters_t *result = querier(index, line);
	//	if (result != NULL){
	//		display_results(result, argv[1]);
	//	}
	//	printf("Query? ");
	}
}


