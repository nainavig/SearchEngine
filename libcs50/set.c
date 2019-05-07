/*  Naina Vig
*   Set
*/


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "set.h"


/******************** local types *****************/
typedef struct snode {
	char *key;
	void *item;
	struct snode *next;
} snode_t;


/******************** global types *****************/
typedef struct set {
	snode_t *head;
} set_t;


/******************** set new  *****************/
set_t *
set_new(void){
	set_t *s = malloc(sizeof(set_t));

	// if run out of memory
	if (s == NULL){
		return NULL;
	}

	s->head = NULL;
	
	return s;
}

/******************** set insert *****************/
bool 
set_insert(set_t *set, const char *key, void *item){

	// return false for any null input
	if ((set == NULL) || (key == NULL) || (item == NULL)){
		return false;
	}

	// create temp node to loop through set
	snode_t *temp = set->head;

	while (temp != NULL){

		// if the key is the same as an existing key, return false
		if (strcmp((temp->key), key) == 0){
			return false;
		} 
		temp = temp->next;
	}


	snode_t *new = malloc(sizeof(snode_t));
	
	// if out of memory
	if (new == NULL){
		return false;
	}
	
	// allocating memory for str key
	new->key = malloc(((strlen(key)+1)*sizeof(char)));
	if (new->key == NULL){
		return false;
	}

	new->item = item;

	// copying key from function argument into key member of new node
	memcpy(new->key, key, strlen(key));

	// inserting new node at head of list
	new->next = set->head;
	set->head = new;

	return true;
}


/******************** set find *****************/
void *
set_find(set_t *set, const char *key){

	// return NULL on NULL inputs
	if ((set == NULL) || (key == NULL)){
		return NULL;
	}

	snode_t *temp = set->head;

	while (temp != NULL){
		// if there is a key in the set matching key argument, return corresponding item
		if (strcmp((temp->key), key) == 0){
			return temp->item;
		}
		temp = temp->next;
	}

	// if key is never found, return NULL
	return NULL;


}

/******************** set print *****************/
void
set_print(set_t *set, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)){

	
	if (fp != NULL){
		if (set != NULL){
			fputc('{', fp);
			snode_t *temp = set->head;

			// loop thorugh nodes in set
			while (temp != NULL){
				fprintf(fp, "(%s", temp->key); 
				// if there is an item print function, print the item
				if (itemprint != NULL){
					fputc(',', fp);
					(*itemprint)(fp, temp->key, temp->item);
				}
				fputc(')', fp);
				temp = temp->next;

				// if there are more nodes in the set, print a comma
				if (temp != NULL){
					fputc(',', fp);
				}
			}
			fputc('}', fp);
		} else {
			fputs("(null)", fp);
		}

	}
	

}


/******************** set iterate *****************/
void 
set_iterate(set_t *set, void *arg, void (*itemfunc)(void *arg, const char *key, void *tem)){
	if ((set != NULL) && (itemfunc != NULL)){
		snode_t *temp = set->head;
		while (temp != NULL){
			(*itemfunc)(arg, temp->key, temp->item);
			temp = temp->next;
		}
	}
}



/******************** set delete  *****************/
void 
set_delete(set_t *set, void (*itemdelete)(void *item)){
	if (set != NULL){
		for (snode_t *temp = set->head; temp != NULL; ){
			if (itemdelete != NULL){
				(*itemdelete)(temp->item);
			}

			// store address to next node
			snode_t *next = temp->next;

			// free memory allocated for key
			free(temp->key);

			// free memory allocated for temp
			free(temp);

			// set temp to the next node
			temp = next;
		}

		// free memory allocated for set_t
		free(set);
	}
}
