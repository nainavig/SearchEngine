/*
*  Naina Vig
*  Counters
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "counters.h"


/***************** Local Types *****************/
typedef struct cnode{
	int key; 
	int key_count; 
	struct cnode *next;  //pointer to next node
} cnode_t;


/**************** Global Types ***********************/
typedef struct counters{
	struct cnode *head;  // head of list of items in bag
} counters_t;


/**************** counters_new() ***********************/
counters_t *
counters_new(void){
	counters_t *counters = malloc(sizeof(counters_t));

	// if out of memory, return NULL
	if (counters == NULL){
		return NULL;
	} else {
		counters->head = NULL;
		return counters;
	}

}


/**************** counters_add ***********************/
int
counters_add(counters_t *ctrs, const int key){

	// check for errors before looking for key
	if ((ctrs == NULL) || (key < 0)){
		return 0;
	}

	cnode_t *temp = ctrs->head;

	while (temp != NULL){
		// check whether temp->key will be int or pointer
		if (temp->key == key){
			temp->key_count++;
			return temp->key_count;
		}
		temp = temp->next;
	}
	
        // if not returned yet, insert new key in front of list and initialize to 0
	cnode_t *new = malloc(sizeof(cnode_t));
	new->key = key;        
	new->key_count = 1;
        new->next = ctrs->head;  // new next now points to current head
	ctrs->head = new;         // current head is set to new

	return new->key_count;
}


/**************** counters_get ***********************/
int counters_get(counters_t *ctrs, const int key){
	if (ctrs == NULL){
		// error, return 0
		return 0;
	}

	cnode_t *temp = ctrs->head;

	while (temp != NULL){
		// key found, return key count
		if (temp->key == key){
			return temp->key_count;
		}

		temp = temp->next;
	}

	// key never found, return 0
	return 0;

}




/**************** counters_set ***********************/
bool counters_set(counters_t *ctrs, const int key, const int count){
	if ((ctrs == NULL) || (key < 0) || (count < 0)){
		// error, return 0
		return false;
	}

	cnode_t *temp = ctrs->head;

	while (temp != NULL){
		// key found, set key_count to count from argument
		if (temp->key == key){
			temp->key_count = count;
			return true;
		}

		temp = temp->next;
	}

	// key never found, create new key and set to count
	cnode_t *new = malloc(sizeof(cnode_t));


	if (new == NULL){
		return false;
	}

	new->key = key;
	new->key_count = count;

	new->next = ctrs->head;
	ctrs->head = new;
	return true;

}

/**************** counters_print ***********************/

void counters_print(counters_t *ctrs, FILE *fp){
	if (fp == NULL){
		return;
	}
	if (ctrs == NULL){
		fprintf(fp, "(null)");
		return;
	}
	
	cnode_t *temp = ctrs->head;
	fprintf(fp, "{");
	// loop through counters
	while (temp != NULL){
		// print key and counts  to file
		fprintf(fp, "(%d, %d)", temp->key, temp->key_count);
		temp = temp->next;
		if (temp != NULL){
			fprintf(fp, ", ");
		}
	}
	fprintf(fp, "}");

}


/****************** counters_iterate *******************/

void
counters_iterate(counters_t *ctrs, void *arg, void (*itemfunc)(void *arg, const int key, const int count)){
	if ((ctrs != NULL) && (itemfunc != NULL)){
		cnode_t *temp = ctrs->head;
		while (temp != NULL){
			(*itemfunc)(arg, temp->key, temp->key_count);
			temp = temp->next;
		}
	}

}


/****************** counters_delete *******************/

void counters_delete(counters_t *ctrs){
	if (ctrs != NULL){
		cnode_t *node = ctrs->head;
		while (node != NULL){
			cnode_t *next = node->next;
			free(node);
			node = next;
		}
		free(ctrs);
	}
}
