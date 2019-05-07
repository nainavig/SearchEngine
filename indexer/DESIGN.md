#### Naina Vig

## Indexer Design Specifications

###User Interface
The indexer's interface is the commandline. The indexer must take exactly two arguments. 

Usage:

~~~~C
./indexer pageDirectory filename
~~~~

Example:

~~~~C
./indexer ../crawler/data testingout
~~~~

###Inputs and Outputs

The only inputs are the commandline parameters, which specify the directory to be indexed and the file to be written to. The output is a file containing a unique word on each line, and for each word, a list of docIDs and corresponding counts of occurrences of the word in that document. 

###Functional Decomposition into Modules

The following functions are used for the indexer:

1. `main`, parses the arguments and passes them to other modules. 
2. `indexBuilder`, initializes index structure and loops through files in directory, storing their word counts in the index.
3. `doctoWebpage`, given a document in the pageDirectory, doctoWebpage stores the information from that document into a webpage structure. 
4. `indexSave`, writes the contents from the index data structure into the output file. 

We use helper modules to provide data structures:

1. `hashtable` provides data structure to maintain index
2. `counters` acts as items in hashtable, maintaining docID and count pairs for each word. 
3. `webpage` maintains specialized functions for accessing and parsing HTML data.  

###Pseudocode

The indexer runs as follows:

1. Execute from command line 
2. Parse command line arguments, validating that the directory exists. 
3. Call indexBuilder on pageDirectory
4. Initialize new hashtable for index and file pointer to file in directory
5. While there are still files able to be opened:
	1. Move file into webpage struct
	2. For each word in webpage
		1. Check if word is of at least length 3
		2. Normalize word
		3. If word is in index, retrieve counters item and increment count for current webpage's docID
		4. If word is not in index, insert word into hashtable with empty counters set, and increment the counter for the docID of the current webpage. 
	3. Set the document name equal to the name of the next file, and attempt to open that file for reading. 
6. Save the index to output file 

###Dataflow through modules

1. `main` parses arguments and passes pageDirectory to indexBuilder and attempts to open output file for writing.
2. `indexBuilder` creates index data structure and fills it with info from pageDirectory, and then returns it to main. 
3. `indexSave` takes index and output file from main and outputs contents of index into output file. 

###Major Data Structures

Helper modules provide data structures:

1. 	`hashtable` of (word, counters) pairs
2. `set` of (word, counters) pairs used by hashtable
3. `counters` of (docID, wordcount) pairs used as items in hashtable. 

###Testing Plan

To test the indexer, I created a separate module, `indextest`.
Usage:

~~~C
./indextest file1 file2
~~~

By the provided specifications, `indextest` works as follows:

1. Validate commandline parameters - file1 is a valid file for reading, and file2 can be opened for writing. 
2. Initialize index data structure
3. index = indexLoad(file1)
4. indexSave(file2, index)
5. clean up data structures
