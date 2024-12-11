#include <fstream>
#include <iostream>
#include <regex>

int main() {
	std::string str;
	std::string temp_str;

	std::ifstream myFile("input.txt");

	if(myFile.is_open()) {
		while(getline(myFile, temp_str)) {
			str += temp_str;
		}
	}

	myFile.close();

	std::smatch str_match;

	// which regexes do we actually need? this is the hard part of the problem.
	// it turns out we don't need the first three listed here:
	std::regex do_reg("do\\(\\)");
	std::regex dont_reg("don't\\(\\)");
	std::regex muls("mul\\((\\d+),(\\d+)\\)");
	std::regex combined_reg("mul\\((\\d+),(\\d+)\\)|do(n't)?\\(\\)"); // slightly more elegant to use ?
	
	// which regex do we need for the iterator? the combined one because the iterator needs to visit
	// every instance of mul, do(), and dont(). we use the other regexes to check exactly which one we are at
	// if we are at do() or dont() then toggle, if we are at a mul, then multiply and add to running tally
	std::sregex_iterator current_match_itr(str.begin(), str.end(), combined_reg);
	std::sregex_iterator last_match_itr;

	int sum = 0;
	bool multiply_enabled = true;

	// for loop looks better than while loop
	for(auto it = current_match_itr; it != last_match_itr; it++) {
		std::smatch match = *it;

		// we actually do not need any of these calls to regex_match
		// conditional can simply be (match.str() == "do()").
		// this is because instantiating the sregex_iterator did all the work that could also be performed by
		// regex_search. it will iterate through every valid mul(), do(), and don't()
		if(regex_match(match.str(), do_reg)) {
			multiply_enabled = true;
		} else if(regex_match(match.str(), dont_reg)) {
			multiply_enabled = false;
		} else if(multiply_enabled && regex_match(match.str(), muls)) {
			sum += stoi(match.str(1)) * stoi(match.str(2));
		}
	}

	std::cout << sum << std::endl;

	return 0;
}