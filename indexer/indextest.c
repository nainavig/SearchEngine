// Naina Vig
// test file for indexer
//
#include <stdlib.h>
#include <stdio.h>
#include "../libcs50/file.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/set.h"
#include "../libcs50/counters.h"
#include "../common/index.h"


// used in debugging for printing hashtable
void print_item(FILE *fp, const char *key, void *item){
        counters_print(item, fp);
}



/* index test
*  tests indexer by loading information from indexer output file into index,
*  and then moving the contents of that index into a new file
*/
int main(int argc, char **argv){
	// verify correct number of arguments
	if (argc != 3){
		printf("usage: ./indextest oldIndexFilename newIndexFilename\n");
		return 1;
	}

	// verify files can be opened
	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL){
		printf("oldIndexFilename invalid\n");
		return 1;
	}

	FILE *fout = fopen(argv[2], "w");
	if (fout == NULL){
		printf("unable to open new index filename\n");
		return 1;
	}
	
	// load file contents into index
	hashtable_t *index = indexLoad(fp);

	// save index contents to output file
	indexSave(index, fout);
	
	// clean up
	hashtable_delete(index, counters_delete);
	fclose(fp);
	fclose(fout);

}
