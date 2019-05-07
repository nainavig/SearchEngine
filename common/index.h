// Naina Vig
// index.h - header file for index.c

#include <stdio.h>
#include <stdlib.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/set.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"


void writeCountersPair(void *arg, const int key, const int item);
void writeHashtablePair(void *arg, const char *key, void *item);

void indexSave(hashtable_t *index, FILE *fp);

hashtable_t *indexLoad(FILE *fp);
