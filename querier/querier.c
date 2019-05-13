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
	counters_t *empty = t->empty;
	int count1;
	if ((count1 = counters_get(c1, key)) != 0){
		if (count < count1){
			counters_set(empty, key, count);
		} else {
			counters_set(empty, key, count1);
		}
	}
}

// computes counters intersection
counters_t *counters_intersect(counters_t *counters, counters_t *new){
	counters_t *new_intersection = counters_new();
	two_cntrs_t *t = malloc(sizeof(two_cntrs_t));
	t->c1 = new;
	t->empty = new_intersection;
	counters_iterate(counters, t, iter_func);
	free(t);
	return new_intersection;
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

// used as node in sorted array
typedef struct sort_node {
	int key;
	int count;
} sort_node_t;


// used to store array and index info as arg to set iterate
typedef struct list {
	sort_node_t ** arr;
	int *i;
} list_t;

// called by counters_iterate, helps get size of counters
void get_size(void *arg, const int key, const int count){
	*(int*)arg = (*(int*)arg) + 1;
}

// called by counters_iterate, helps sort counters
void sort_helper(void *arg, const int key, const int count){

	// create new node to add to list
	sort_node_t *s = malloc(sizeof(sort_node_t));
	s->key = key;
	s->count = count;


	// get info from arg
	list_t *t = arg;
	sort_node_t ** arr = t->arr;
	int *i = t->i;

	// insert into sorted list
	int j = (*i)-1;
	while (j >= 0 && arr[j]->count > count){
		arr[j + 1] = arr[j];
		j = j - 1;
	}
	arr[j+1] = s;

	// increment index
	(*i) = (*i) + 1;
}


// sorts counters into array of (key, count) structs
list_t *
sort_counters(counters_t *counters){
	int c_size = 0;
	counters_iterate(counters, &c_size, get_size);

	sort_node_t ** arr = malloc(sizeof(sort_node_t)*c_size);

	list_t *l = malloc(sizeof(list_t));
	l->arr = arr;
	int j = 0;
	l->i = &j; 

	counters_iterate(counters, l, sort_helper);

	return l;
}

void 
display_results(list_t *l, char *pageDirectory){
	sort_node_t ** arr = l->arr;
	int *length = l->i;

	for (int i = 0; i < *length; i++){
		printf("%d: %d", arr[i]->key, arr[i]->count);
	}
}

void duplicate_helper(void *arg, const int key, const int count){
	counters_t *counters = arg;
	counters_set(counters, key, count);
}

counters_t *
duplicate_counters(counters_t *counters){
	counters_t *new_counters = counters_new();

	counters_iterate(counters, new_counters, duplicate_helper);
	return new_counters;
}


/*************************** main algorithm ***************************************/

typedef struct parsed_line {
	char ** arr;
	int num_words;
} parsed_line_t;


parsed_line_t *
parse_input(char *line){
	parsed_line_t *s = malloc(sizeof(parsed_line_t)); 
	char **arr = malloc(50*sizeof(char*));
	
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

	for (int j = 0; j < i; j++){
		printf("%s\n", arr[j]);
	}
		
	s->arr = arr;
	s->num_words = i;

	return s;
}

counters_t *
querier(hashtable_t *index, parsed_line_t *s){

	// maintains whether conjunction is last word seen
	bool conj_last_seen = false;

	counters_t *c_union = counters_new();
	counters_t *c_intersect = NULL;

	for (int i = 0; i < s->num_words; i++){
		// edge case: first word is conjunction
		if ((i == 0) && (is_conjunction(s->arr[i]))){
			printf("Error: '%s' cannot be first\n", s->arr[i]);
			return NULL;
		}

		// if conjunction, adjust counters sets
		if (is_conjunction(s->arr[i])){
			// bad case: two conjunctions in a row
			if (conj_last_seen) {
				printf("Error: cannot have two adjacent conjunctions\n");
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
					c_intersect = duplicate_counters(counters);
				// otherwise, take intersection of counters with current intersect counters set
				} else {
					counters_t *new_intersection = counters_intersect(c_intersect, counters);
					c_intersect = new_intersection;
				}
			}
			conj_last_seen = false;	
		}
	}
	if (c_intersect != NULL){	
		counters_union(c_union, c_intersect);
		counters_delete(c_intersect);
	}
	counters_print(c_union, stdout);
	return c_union;
}



int main(int argc, char **argv){
	if (argc != 3){
		printf("usage: ./querier pageDirectory indexFilename\n"); 
	}
	FILE *fp = fopen(argv[2], "r");
	hashtable_t *index = indexLoad(fp);

//	hashtable_print(index, stdout, print_item);
	

/*
	printf("Testing parse_input:\n");
	char line[50] = "alligator";
	printf("%s\n", line);
	parsed_line_t *s = parse_input(line);

	for (int i = 0; i < s->num_words; i++){
		printf("%s\n", s->arr[i]); 
	} 

*/
/*
	printf("Testing sort_counters:\n");
	counters_print(c, stdout);
	display_results(sort_counters(c), argv[1]);

*/

	
	char *line = malloc(50);
	printf("Query? ");
	while (fgets(line, 50, stdin) != NULL){
		parsed_line_t *s  = parse_input(line);


		counters_t *result = querier(index, s);
	//	sort_counters(result);

	//	if (result != NULL){
	//		display_results(result, argv[1]);
	//	}
		counters_delete(result);
		printf("Query? ");
	
		free(s->arr);
		free(s);
	}

	free(line);

	fclose(fp);
	hashtable_delete(index, counters_delete);
}


