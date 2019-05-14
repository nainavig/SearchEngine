####Naina Vig

##TSE Crawler Implementation

The purpose of the tiny search engine crawler is to take a seed webpage and crawl all valid webpages within a specified maximum depth, and then write the contents of each valid webpage to a directory. I implemented my tse crawler as a graph search, where each webpage is a node and the links contained in each webpage are the node's neighbors. 


###Data Structures

The cs50 library contains header files with implementation specifics for the following data structures. 

`webpage_t` is a struct to represent a webpage and its contents. It contains a pointer to a string for URL, a pointer to a string for HTML, an int for the depth, and the size of the webpage html represented by the size_t datatype. Each of these fields are accessed using the `webpage_getFIELD` method for each field. 

`bag` is a data structure which contains items of type `webpage_t`. The bag is implemented as a linked list. The caller can extract one item at a time, which removes it from the bag and returns its pointer. As each new valid link is discovered, a `webpage` is created for that link, and the pointer to the webpage item is added to the bag. 

`hashtable` is a data structure which stores (key, item) pairs. The hashtable is implemented as an array of linked lists. Each (key, item) pair can be accessed by key. Each key can only be inserted once. The hashtable data structure is used as a visited set in our graph search, where each key item pair contains a char * URL. If a URL is successfully added to the hashtable, it means that that URL has not yet been explored. 

###Pseudocode

The main function for the crawler takes a seedURL, a pageDirectory, and a maxDepth. If it receives the incorrect amount of arguments or arguments are invalid, it returns an error message. It then passes the arguments into the bag loop.

####Pseudocode for bag loop
~~~C
create webpage for seedurl
add webpage to bag and visited set
while bag nonempty:
	get webpage from bag
	get HTML and store in page
	save webpage to directory
	if depth(webpage) < maxdepth:
		for link in webpage:
			normalize url
			if url internal:
				if url not visited:
					create new webpage for url
					insert webpage into bag
~~~

####Pseudocode for page saver
~~~C
allocate memory for pathname+docID string
copy pathname and docID into allocated memory for filename
file pointer = open filename for writing
if file pointer is NULL:
	print error message
	return
else:
	write webpage info to file
	close file
	free filename 
~~~


###Error Handling

The main function verifies that the arguments are valid. This includes checking whether the pageDirectory is a valid path and checking whether the maxDepth is an integer and within the specified range. 

Within the body of the the functions, each time memory is allocated the pointer is verified to not be NULL before being used. 

If the URL is unable to be normalized, the function prints an error message and returns. 





