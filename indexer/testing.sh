# Naina Vig
# Testing for indexer module

# Testing on crawler output

./indexer ../crawler/data indexer1

gawk -f indexsort.awk indexer1 > indexer1.sorted

echo echoing indexer output

cat indexer1

# Testing index load and index save

./indextest indexer1 indexertest1
gawk -f indexsort.awk indexertest1 > indexertest1.sorted

# Comparing indexer1 with indexertest1

echo differences between indexer and indextest outputs - nothing if no difference:

diff indexertest1.sorted indexer1.sorted

rm indexer1 indexer1.sorted indexertest1 

# testing on toscrape directory

echo testing on toscrape directory

./indexer toscrape-depth-2/ scrape 
cat scrape

./indextest scrape stest

rm scrape stest

# testing on nonexistent directory

echo testing on nonexistent directory

./indexer bad file


