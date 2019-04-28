#!/bin/bash

# Naina Vig
# Testing for crawler.c

# test 1: tests normal case

echo test1

mkdir test1

echo testing http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html at depth 6

./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html test1 6

#for file in test1; do echo file done;

# test 2: tests depth above allowed max

echo test2: tests invalid depth

mkdir test2

echo testing http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html at depth 11

./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html test2 11

#for file in test2; do echo file done;

# test 3: tests invalid directory

echo test3: tests invalid directory

echo testing http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html with bad dir

./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html baddir 6

# test 4: tests bad URL

echo test4: tests on page with only wikipedia links

mkdir test4

echo testing http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/index.html 

./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/index.html test4 3

#for file in test4; do echo file done;
