# Naina Vig
# Testing for Querier

#!/bin/bash

echo testing that backus naur form will be validated, see design spec for specific edge cases

./querier tests/toscrape-depth-2 tests/toscrape-index-2.txt <bntest

echo only first and last queries should have outputs, rest are bad form

echo testing that parse argument will be able to handle edge cases, such as weird spacing and capitalization
echo also shows that ranking works

./querier tests/toscrape-depth-2 tests/toscrape-index-2.txt <testparse

echo testing wikipedia at depth 1 

echo demonstrating associativity works: 

./querier tests/wikipedia-depth-1 tests/wikipedia-index-1.txt <associativitytest

echo first two outputs should be null, last three outputs should be same

echo testing longer queries

./querier tests/wikipedia-depth-1 tests/wikipedia-index-1.txt <longtest


echo testing that commandline arguments are validated

./querier baddir tests/wikipedia-index-1.txt
./querier tests/wikipedia-depth-1 badfile
