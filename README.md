## Day 1
Used unordered map in part 2 to tally frequencies of right column (2n vs. n^2 run time).

## Day 2
Brushed up on reading input from a file. `.erase()` member function completely slipped my mind for part 2; need a refresher on STL perhaps. Is there a more efficient way? Brute force was simple after that.

## Day 3
Both `regex_match` and `regex_search` check a string against a regex. So what's the difference between the two? The former checks if the entire string passed in matches the pattern of the regex. The latter checks if a subset of the string matches the pattern of the regex.

Hence, given the nature of the problem, regex_search is the function we want to use. Alternately, we can accomplish the work of regex_search with an instantiation of an obj of type sregex_iterator.

At some point will come back and implement solution with regex_search.

`smatch` is a match results type that is filled with match info and submatches by the function `regex_match.` `smatch` has 3 member functions: `str()`, `position()`, and `length()`. Regex resources consulted: 

*https://regexone.com/lesson/matching_characters?
*https://regexr.com/
*https://youtu.be/9K4N6MO_R1Y?si=0tsnSmABvHDySL8a
*https://stackoverflow.com/questions/42983337/how-to-get-only-given-captured-group-regex-c
*https://www.fluentcpp.com/2020/02/28/c-regex-101-simple-code-for-simple-cases-with-regexes/
*https://www.geeksforgeeks.org/smatch-regex-regular-expressions-in-c/

## Day 4
Implementations are a little naive/hacky. Had to really think through and debug some of the bounds checking. I've seen some more elegant solutions out there so I'll have to familiarize myself with the word search type of problem. `.size()` returns an unsigned int so that was giving me some trouble because my conditionals had some index/bounds checking and comparisons with negative numbers so the conditionals were never being satisfied. For part 2, more would have to be adjusted for the code to work for target words of variable length.

`int main() {
    std::string str = "Hello, World!";
    char ch = 'o';

    // Remove all occurrences of 'o'
    auto it = std::remove(str.begin(), str.end(), ch);
    std::cout << str << std::endl;  // Output: Hell, Wrld!
    
    str.erase(it, str.end());

    std::cout << str << std::endl;  // Output: Hell, Wrld!
    return 0;
}``

## Day 5

Topological sort: performed on a directed acyclic graph. The topological sort is a linear ordering of all vertices such that if the graph contains edge (u,v), then u,v appears before v in the ordering. 

Two possible implementations: DFS and Kahn's algorithm. Might want to use the former if one doesn't have easy access to a list of all vertices and edges; more complex because it has explicit use of a stack; might be less desirable if the graph paths are long due to the many recursions required. If you do have all the vertices, may want to use the latter.

Any DAG has at least one topological order. The topological orders need not be unique. Algorithms which find T.O. operate in linear time

Indegree of a node is the count of incoming edges. Outdegree of a node is the count of outgoing edges.

A DAG has at least one node with indegree 0 and at least one node with outdegree 0. As the name suggests, no cycles. 0 indegree means no dependency on other nodes.



Need 3 structures: 
    indegree - hash map or vector
    queue
    sorted - stores the final linear ordering

* Think of each update as its own small graph to "solve."

* In some pedagogical examples, indegree is kept in a vector of ints where the vector indexes represent the node number and the stored values are the indegree values. However, that's not suitable for this problem because unlike in those examples, the nodes (pages) we're given for each update aren't "nice" (non-sequential, don't start from 0). At bottom, we need to keep a list of int-int pairs (page number-indegree number) so `unordered_map<int,int>` is suitable.

* Need to familiarize myself with various iterator syntax in for loops for various data structures.

* If the number of nodes processed by Kahn's algorithm != the number of nodes in the graph, then the graph is not a DAG.

* Indegree struct could have been pair<int, int>

## Day 6

* Need to keep track of which direction the guard is facing

* Could be a good idea to make a class for each grid space? One variable for visited/unvisited, another for char