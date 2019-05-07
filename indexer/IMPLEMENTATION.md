###Naina Vig

##Indexer Implementation Specifications

###Detailed Pseudocode for each of the componenets

main

~~~C
if not two input parameters:
	return error
if pageDirectory is not a directory:
	return error
if indexFilename cannot be opened:
	return error

index = indexBuilder(pageDirectory)
indexSave(index, indexFilename)
~~~

indexBuilder

~~~C
index = hashtable_new()
docID = 1
docFilename = pageDir/docID
while ((file = docFilename) can be opened):
	webpage = doctoWebpage(file)
	for word in webpage:
		if len(word) > 2:
			normalize word
			counters = hashtable_find(index, word)
			if counters == NULL:
				hashtable_insert(index, word, counters_new())
				counters = hashtable_find(index, word)
			counters_add(counters, docID)
	docID++
	docFilename = pageDir/docID
return index
~~~

indexSave

~~~C
hashtable_iterate(index, file, writeHashtablePair)
~~~

writeHashtablePair(arg, key, item)

~~~C
write key to file
counters_iterate(item, file, writeCountersPair)
~~~

writeCountersPair(arg, key, count)

~~~C
write key count to file
~~~

indexLoad

~~~C
initialize index data structure
lines = lines in file
for line in lines:
	word = first word
	initialize counters
	insert (word, counters) into index
	scan (%d, %d) until invalid, add to counters
return index
~~~


###Definition of detailed function prototypes/parameters

~~~C
int main(int argc, char **argv);
webpage_t *doctoWebpage(FILE *fp);
hashtable_t *indexBuilder(char *pageDirectory);
hashtable_t *indexLoad(FILE *fp);
void indexSave(hashtable_t *index, FILE *fp);
hashtable_t *indexLoad(FILE *fp);
~~~

###Data Structures

The data structures used in the indexer module are hashtable, set (used by hashtable), and counters. The specifications for these are included within their individual .h files. 


###Error Handling

Errors are handled by checking if each pointer is NULL before using, and verifying parameters before using. 

###Resource Management

After indexes are loaded into files, they are freed using hashtable\_delete, calling counters\_delete on each hashtable item. Malloc'd strings are freed after use. 