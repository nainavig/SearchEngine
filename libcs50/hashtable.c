// Naina Vig
// Hashtable.c

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "jhash.h"
#include "hashtable.h"
#include "set.h"
#include "memory.h"


/************************ local types *************************/

/************************ global types *************************/

typedef struct hashtable {
	int num_slots;
	set_t **array;
} hashtable_t;



/************************ hashtable new *************************/

hashtable_t *
hashtable_new(const int num_slots){
	hashtable_t *ht = malloc(sizeof(hashtable_t));
	
	// return NULL if out of memory
	if (ht == NULL){
		return NULL;
	}
	
	// set members, allocate memory for array of sets
	ht->num_slots = num_slots;
	ht->array = calloc(num_slots, sizeof(set_t*));

	// create new set for each index of array
	for (int i = 0; i < num_slots; i++){
		ht->array[i] = set_new();
	}

	return ht;
}


/************************ hashtable insert *************************/
bool 
hashtable_insert(hashtable_t *ht, const char *key, void *item){
	
	if ((ht == NULL) || (key == NULL) || (item == NULL)){
		return false;
	}

	// compute which slot to insert (key, item)
	// use num_slots for mod val
	unsigned long index = JenkinsHash(key, ht->num_slots);
	
	// if set_insert == false, key is already in set for that slot
	// if set_insert == true, key is successfully added to ht
	bool added = set_insert(ht->array[index], key, item);

	return added;

}


/************************ hashtable find *************************/
void *
hashtable_find(hashtable_t *ht, const char *key){
	if ((ht == NULL) || (key == NULL)){
		return NULL;
	}

	// compute which slot to look in
	unsigned long index = JenkinsHash(key, ht->num_slots);
	
	// call set_find on the set for the given slot
	void *item = set_find(ht->array[index], key);
	return item;

}


/************************ hashtable print *************************/
void 
hashtable_print(hashtable_t *ht, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)){
	
	if ((fp == NULL) || (ht == NULL)){
		return;
	}

	for (int i = 0; i < ht->num_slots; i++){
		fprintf(fp, "\n");
		fprintf(fp, "slot %d contents: ", i);
		set_print(ht->array[i], fp, (itemprint));
	
	}

}


/************************ hashtable iterate *************************/
void 
hashtable_iterate(hashtable_t *ht, void *arg, void (*itemfunc)(void *arg, const char *key, void *item)){

	if ((ht != NULL) && (itemfunc != NULL)){
		for (int i = 0; i < ht->num_slots; i++){
			set_iterate(ht->array[i], arg, itemfunc);		
		}
	}
}


/************************ hashtable delete *************************/
void 
hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item)){
	if (ht != NULL){
		for (int i = 0; i < ht->num_slots; i++){
			set_delete(ht->array[i], itemdelete);
		}
		free(ht->array);
		free(ht);
	}	

#ifdef MEMTEST
	count_report(stdout, "End of hashtable_delete");
#endif
}
