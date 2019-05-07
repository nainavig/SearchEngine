####Naina Vig

##Readme for indexer

github username: nainavig

usage: ./indexer pageDirectory indexFilename

pageDirectory must be valid directory


Known errors: Memory leak from normalized string. When I tried to free the string, it caused some sort of corruption when printing the strings and extra characters were added. It also corrupts strings in the larger test cases, most likely because of the memory management issues. 
