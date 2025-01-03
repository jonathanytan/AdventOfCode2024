## Day 1
Used unordered map in part 2 to tally frequencies of right column (2n vs. n^2 run time).

## Day 2
Brushed up on reading input from a file. `.erase()` member function completely slipped my mind for part 2; need a refresher on STL perhaps. Is there a more efficient way? Brute force was simple after that.

## Day 3
Both `regex_match` and `regex_search` check a string against a regex. So what's the difference between the two? The former checks if the entire string passed in matches the pattern of the regex. The latter checks if a subset of the string matches the pattern of the regex.

Hence, given the nature of the problem, regex_search is the function we want to use. Alternately, we can accomplish the work of regex_search with an instantiation of an obj of type sregex_iterator.

At some point will come back and implement solution with regex_search.

`smatch` is a match results type that is filled with match info and submatches by the function `regex_match.` `smatch` has 3 member functions: `str()`, `position()`, and `length()`. Regex resources consulted: 

* https://regexone.com/lesson/matching_characters?
* https://regexr.com/
* https://youtu.be/9K4N6MO_R1Y?si=0tsnSmABvHDySL8a
* https://stackoverflow.com/questions/42983337/how-to-get-only-given-captured-group-regex-c
* https://www.fluentcpp.com/2020/02/28/c-regex-101-simple-code-for-simple-cases-with-regexes/
* https://www.geeksforgeeks.org/smatch-regex-regular-expressions-in-c/

## Day 4
Implementations are a little naive/hacky. Had to really think through and debug some of the bounds checking. I've seen some more elegant solutions out there so I'll have to familiarize myself with the word search type of problem. `.size()` returns an unsigned int so that was giving me some trouble because my conditionals had some index/bounds checking and comparisons with negative numbers so the conditionals were never being satisfied. For part 2, more would have to be adjusted for the code to work for target words of variable length.

## Day 5

Topological sort: performed on a directed acyclic graph. The topological sort is a linear ordering of all vertices such that if the graph contains edge (u,v), then u,v appears before v in the ordering. 

Two possible implementations: DFS and Kahn's algorithm. Might want to use the former if one doesn't have easy access to a list of all vertices and edges; more complex because it has explicit use of a stack; might be less desirable if the graph paths are long due to the many recursions required. If you do have all the vertices, may want to use the latter.

Any DAG has at least one topological order. The topological orders need not be unique. Algorithms which find T.O. operate in linear time

Indegree of a node is the count of incoming edges. Outdegree of a node is the count of outgoing edges.

A DAG has at least one node with indegree 0 and at least one node with outdegree 0. As the name suggests, no cycles. 0 indegree means no dependency on other nodes.

* Think of each update as its own small graph to "solve."

* In some pedagogical examples, indegree is kept in a vector of ints where the vector indexes represent the node number and the stored values are the indegree values. However, that's not suitable for this problem because unlike in those examples, the nodes (pages) we're given for each update aren't "nice" (non-sequential, don't start from 0). At bottom, we need to keep a list of int-int pairs (page number-indegree number) so `unordered_map<int,int>` is suitable.

* If the number of nodes processed by Kahn's algorithm != the number of nodes in the graph, then the graph is not a DAG.

## Day 6

* Two insights to this problem: 
    1. The guard enters a loop when she re-visits a tile with the same orientation. Hence, we need to keep track of both the guard's orientation at each visited tile as well as the coordinates of each visited tile. Keep track of this with unordered_map<int, char>. The `int`: originally tried making the key a coordinate pair stored in a `pair` type, but C++ has no hash function for the type, so we make our own rudimentary hash. The char stores the orientation of the guard
    
    2. In part 1 of the algorithm, the guard simulation is run once, which finds the grid tiles visited as well as the guard's direction. Then, in part 2, we run a second set of simulations. For each, we place a new obstacle in the path traversed by the guard in part 1 (excluding the guard's starting position) and see if the guard enters a loop (this is an improvement over the brute force solution which places an obstacle on every empty tile of the grid)

* I've seen others represent directions numerically rather than with chars and these solutions seem neater so I will have to look into that.
* Also learned about range-based for loops

## Day 7
Identified this as a backtracking problem and needed a major refresher. I first reviewed sample applications of backtracking algorithms in N-Queens and Sudoku solvers. Had integer overflow issues (silly, frustrating). I made significant improvements by pruning the search tree, which reduced run time from 12.96s to 5.32s for part 2. Pruning was based on the following insight: we could exit out of a branch any time a portion of the expression was greater than the test value. 

The code from part 1 could not easily accomodate the addition of more operators. Rather than re-write it for part 2, I managed to extend the solution into its unwieldy final state. Adding a fourth operator would be nightmarish. The function ought to be re-written.

## Day 8
Overloaded binary operators to make adding instances of pair<int,int> easier. Overloading operators for larger projects is a bad idea but our programs are small. The approach was quite simple and bugs were resolved after I re-acquainted myself with vector addition. In part 2, by definition, every antenna is also an antinode.

## Day 9
* In part 1, I made a misstep where I first represented the memory block representation of the diskmap (e.g. `00..111.33) as a string. This works for file IDs from 0-9, but once you get into double digits that becomes a problem. So I switched to a vector of ints where -1 represented empty memory. Keep track of file IDs in a vector<int> wherein the index represents the ID and the data represents a file size.
* In part 2, I added a list of every section of contiguous empty memory (both the index of the first block and the size).
* In debugging figured out that my code was moving files rightward to empty spaces (expected behavior is to move them leftward)

## Day 10

* Had to do a refresher on graph traversal and DFS. Looked at flood fill problem.
    * Recursive implementation is cleaner, iterative implementation is useful because other graph traversal algorithms are structured similarly.
* In part 1, there is no need for DFS. You do not run the risk of visiting a node you had previously visited - which is something you need to keep track of in a DFS implementation - because you can only travel to nodes that are 1 grade higher.
* In part 1, for any particular trailhead, you need to keep track of 9-heights that were already visited in the traversal so you don't double count them for the trailhead score. In part 2, for a particular trailhead and a 9-height that's reachable, to find the number of different paths to that 9-height, simply remove the check - established in part 1 - which verifies if you've already visited that particular 9-height

## Stray observations
* sets and maps do not have a hash function for pair<int,int> so you must make your own
* `emplace()` does in-place insertion. cf. `insert()`
* Char <> int conversion
    * `(integer) + '0'`, where (integer) is a digit between 0-9 converts the integer into its char representation
    * `(char 0-9) - '0'`, where (char 0-9) is the char of a digit between 0-9 converts the char into its integer representation
    * To remember these, you can think of adding `'0'` as adding the property of being a character and subtracting it as removing the quality of being a char
* To get index of an element pointed to by a foward iterator: `fwd_itr - vec.begin()`. 
* You can't compare (<, >, ==) forward and reverse iterators directly. To convert a reverse iterator, use `.base()` like in `bwd_itr.base() - 1`
* for range-based for loops, when iterating on big objects, make sure to access by const reference. can also use `auto`