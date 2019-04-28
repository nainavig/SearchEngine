// Naina Vig
// Crawler.c
// Implementation of crawler

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/set.h"
#include "../libcs50/jhash.h"
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"



/* crawler.c takes a pageDirectory, seedURL, and maxDepth, and crawls 
 * through linked webpages and stores them in directory until reaching maxDepth
 * usage: ./crawler seedURL pageDirectory maxDepth
 */


/******************** logr ***********************/

/* used to display crawler progress
 */

inline static void logr(const char *word, const int depth, const char *url)
{
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
}


/******************** pageSaver ***********************/

/* pageSaver takes a webpage, docID, and directory
*  opens a file, and writes the webpage url, depth, and html to file
*/

void pageSaver(webpage_t *page, int docID, char *pageDirectory){
	// allocate memory for filename and copy pageDir and docID into filename
	char *filename = malloc(((strlen(pageDirectory)+1)*sizeof(char)) + sizeof(int));
	if (filename == NULL){
		printf("Memory error");
		return;
	}

	sprintf(filename, "%s/%d", pageDirectory, docID);

	// attempt to open filename
	FILE *fp = fopen(filename, "w");
	if (fp == NULL){
		printf("Error: couldn't open file\n");
		return;
	}	

	// write webpage data into file
	fprintf(fp, "%s\n%d\n%s\n", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page)); 

	// close file and free filename
	fclose(fp);
	free(filename);
}


/******************** bagLoop ***********************/

/* bagLoop crawls the webpages and adds those that haven't been seen 
*  and are below maxDepth to the bag. While the bag is nonempty, each 
*  URL in bag is explored. 
*/
void bagLoop(char *seedURL, int maxDepth, char *pageDirectory){

	// create hashtable of visited set
	hashtable_t *visited = hashtable_new(13);

	// create bag to be explored
	bag_t *bag = bag_new();
	if ((bag == NULL) || (visited == NULL)){
		printf("memory error, null pointer to bag\n");
		return;
	}

	// stored as item in the hashtable, doesn't signify anything
	int item = 0;

	// create webpage item for seed url and insert to bag, hashtable
	webpage_t *seeditem = webpage_new(seedURL, 0, NULL);
	bag_insert(bag, seeditem);
	hashtable_insert(visited, seedURL, &item);

	// explore bag while nonempty
	int docID = 1;
	webpage_t *page;
	while  ((page = bag_extract(bag))!= NULL){

		// fetch HTML for webpage
		if (webpage_fetch(page)){
			logr("Fetched", webpage_getDepth(page), webpage_getURL(page));
		}


		logr("Saved", webpage_getDepth(page), webpage_getURL(page));
		// save webpage info to directory
		pageSaver(page, docID, pageDirectory);
		docID++;
		if (webpage_getDepth(page) < maxDepth){

			// loop through links in HTML text
			logr("Scanning", webpage_getDepth(page), webpage_getURL(page));
			int pos = 0;
			char *link;
			while ((link = webpage_getNextURL(page, &pos)) != NULL){

				logr("Found", webpage_getDepth(page)+1, link);

				if (!NormalizeURL(link)){
					printf("error URL cannot be normalized\n");
					free(link);
					return;
				}
				if (IsInternalURL(link)){
					// If link is internal, check if its been seen by trying to add to hashtable
					if (hashtable_insert(visited, link, &item)){

						// if successfully added to hashtable, create new webpage and add to bag
						webpage_t *wp_new = webpage_new(link, webpage_getDepth(page)+1, NULL);
						bag_insert(bag, wp_new);
						logr("Added", webpage_getDepth(wp_new), webpage_getURL(wp_new));

					} else {
						logr("IgnDupl", webpage_getDepth(page)+1, link);
						free(link);
					}
				} else {
					logr("IgnExtrn", webpage_getDepth(page)+1, link);
					free(link);
				}
			
			}
		}
		// free mem allocated for webpage
		//webpage_delete(page);
	}

	// delete bag and webpages stored in bag (should be empty)
	bag_delete(bag, webpage_delete);

	// delete hashtable, items are ints so don't need to be freed
	hashtable_delete(visited, NULL);
}


/* Main verifies arguments are valid and then calls bagloop on arguments
 *
 */

int main(int argc, char *argv[]){
	char *seedURL;
	char *pageDirectory;
	int maxDepth;

	// verify correct amount of arguments
	if (argc != 4){
		printf("\nUsage: ./crawler seedURL pageDirectory maxDepth\n");
		return 1;
	}

	seedURL = argv[1];
	pageDirectory = argv[2];

	// convert depth from str to int 
	maxDepth = atoi(argv[3]);

	// verify if directory is a valid path
	if (pagedir(pageDirectory, ".crawler") == false){
		printf("\nError: Invalid pageDirectory\n");
		return 2;
	}

	// verify that maxDepth is valid
	if ((maxDepth <= 0) || (maxDepth > 10)){
		printf("\nMaxDepth must be integer in range [1...10]\n");
		return 3;
	}

	bagLoop(seedURL, maxDepth, pageDirectory);

}
