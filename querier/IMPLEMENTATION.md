### Naina Vig

# Implementation Spec for Querier

The querier relies heavily on already provided data structures, so the main use for the creation of new data structures was for the purpose of returning multiple values and passing multiple values to a function. 

## Detailed Pseudocode

Including detailed pseudocode only for the functions which drive the logic of the querier.


`parse_input`

~~~
parsed_line *s
char ** arr

char *p1 = input
char *p2 = input

i = 0
while p2 != newline:
	if (invalid_char(p2)):
		return NULL
	if (is_space(p2)):
		*p2 = '\0'
		if not is_space(p1):
			arr[i] = p1
			i++
		p1 = p2+1
	p2++

*p2 = '\0'
if not is_space(p1):
	arr[i] = p1
	i++
	
s->arr = arr
s->num_words = i
return s
~~~

`querier`

~~~C
conjunction_last_seen = TRUE
union = counters_new()
intersection = NULL
	
for word in line: 
	if word == "or":
		if conjunction_last_seen = TRUE:
			return NULL
		else:
			union = union U intersection
			intersection = NULL
			conjunction_last_seen = TRUE
	if word == "and": 
		if conjunction_last_seen == TRUE:
			return NULL
		else:
			conjunction_last_seen = TRUE
	else:
		conjunction_last_seen = FALSE
		counters = hashtable_find(index, word)
		if counters != NULL:
			if intersection == NULL:
				intersection = counters
			else:
				intersection = intersection ^ counters
		else:
			counters = counters_new();
if conjunction_last_seen:
	return NULL
if intersection != NULL:
	union = union U intersection
	
return union
~~~

`sort_and_display`

~~~
size = get_size(counters)

if size == 0:
	print("no documents match")
	return
	
list = []
i = 0
for (key, count) in counters:
	j = i-1
	while (j >= 0 and list[j]->count > count):
		list[j+1] = list[j]
		j = j-1
	list[j+1] = (key, count)
	i++

k = 0
for (key, count) in list:
	file = open(pageDirectory/k)
	URL = readline(file)
	print(key, count, URL)
	k++
~~~

## Function Prototypes

### Main logic

~~~
counters_t *querier(hashtable_t *index, parsed_line_t *s);
parsed_line_t *parse_input(char *line);
void sort_and_display(counters_t *counters, char *pageDirectory);
~~~

### Sort helpers
~~~
void sort_helper(void *arg, const int key, const int count);
void get_size(void *arg, const int key, const int count);
~~~

### String utilities
~~~
bool is_conjunction(char *p);
bool is_empty(char *p);
bool is_valid(char *p);
~~~

### Union/intersection functions
~~~
void counters_union(counters_t *counters, counters_t *new);
void union_helper(void *arg, const int key, const int count);
counters_t *counters_intersect(counters_t *counters, counters_t *c_word);
void inter_helper(void *arg, const int key, const int count);
~~~

## Data Structures

### Structs:

`typedef struct two_cntrs`: holds two counters sets, used by `counters-iterate` in `counters_intersection`.

`typedef struct sort_node`: holds integers for key and count, represent (doc, count) pairs in sorted array.

`typedef struct sortedlist`: holds an array of pointers to sort nodes and an integer i representing the current index. Used by `counters_iterate` to help sort the counters list.

`typedef struct parsed_line`: holds array of pointers to strings and an integer representing the number of words in a query. Created and returned by `parse_input`.

### Modules:

`hashtable`: implementation and use specs in `hashtable.h`

`counters`: implementation and use specs in `counters.h`


## Error Handling

The querier avoids memory errors by validating each pointer before attempting to access memory. Upon a bad commandline argument, the querier cleans up the data structures and exits with non-zero status. Upon an invalid input string, the querier ignores the input string and prompts the user for another input. 