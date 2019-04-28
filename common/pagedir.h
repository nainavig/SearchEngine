// Naina Vig
// Header file for pagedir

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/************************ pagedir *************************/

/* Returns true if directory is valid
 * Takes pathname and functionality name
 * Creates a file of the form pathname/fun if path is valid and returns true
 * If path is not a valid directory, returns false
 */

bool pagedir(char *pageDirectory, char *Fun);
