### Naina Vig

## Querier Design Spec

### User interface

Usage: `./querier pageDirectory indexFilename`

pageDirectory must be valid directory produced by crawler and indexFilename must be valid index file.

### Inputs and outputs

Inputs are queries in the Backus Naur form. Output is sorted list of documents and word counts produced by querier. 

### Functional decomposition into modules

`main`: validates inputs and passes index to querier

`parse_input`: given a user input line, `parse_input` breaks the input into an array of pointers to valid words

`querier`: given an array of words, the querier loops through the words and returns a counters set corresponding to the query. 

`counters_intersection`: takes the intersection of two counters sets

`counters_union`: takes the union of two counters sets

`sort_and_display`: given a counters set, sorts and prints document information to user

### Dataflow through modules

`main`: validates commandline args and loads index, contains loop which reads user input, passes user input to `parse_input`

`parse_input`: given an input line, returns an array of pointers to words, which is then passed to querier

`querier`: given array representing query from parse input, initializes counters set and loops through query, adding docs and counts to total counters set using `counters_union` and `counters_intersection`. it then returns the counters to, which is then passed to `sort_and_display`.

`sort_and_display`: given counters set returned by querier and pageDirectory, `sort_and_display` sorts the counters set and gets URL info from pageDirectory. 

### Pseudo code for logic/algorithmic flow


~~~C
index = loadIndex(indexFilename)
while input:
	if input == blank:
		pass
	normalize(input)
	line = parse_input(input)
	counters = querier(line)
	sort_and_display(counters)
~~~


### Major data structures

`counters` maintains the document word counts for each query

`hashtable` maintains a lookup table that maps a word to a counters set

### Testing plan

I plan to test that my union/intersection logic holds by including queries with null intersections, queries with words not in the index, and unions starting with word not in index. I'll demonstrate that my associativity holds by including the same query, but rephrased in multiple equivalent ways, and demonstrating that the result will always be the same documents. 

I plan to demonstrate that my parse argument function holds by including the following edge cases: 

* words with mixed cases (summer capital and lowercase letters)
* queries with multiple spaces between words
* a query beginning with spaces
* a query ending with spaces
* queries with bad characters

I plan to demonstrate that my logic for checking that the queries are in Backus Naur form holds by including the following edge cases:

* queries beginning with "and" and "or"
* a query with multiple adjacent conjunctions
* a query where the "and" is ommited
* queries ending with "and" and "or"
* a query with a single word

I plan to test that my sorting function works by including a query where the resulting document counts are varied, and demonstrating that they will be printed in order.  

I also plan on testing that my querier will work for longer queries (> five words) and short queries (no words or one word). 

