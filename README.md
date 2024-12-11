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