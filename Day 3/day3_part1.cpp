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
	std::regex reg("mul\\((\\d+),(\\d+)\\)");
	
	// where the magic happens so to speak. using this iterator we can visit every substring which
	// matches the regex
	std::sregex_iterator currentMatch(str.begin(), str.end(), reg);
	std::sregex_iterator lastMatch;

	int sum = 0;

	while(currentMatch != lastMatch) {
		std::smatch match = *currentMatch;

		//.str(x) returns string of the xth captured group defined in the regex
		sum += stoi(match.str(1)) * stoi(match.str(2));
		currentMatch++;
	}

	std::cout << sum << std::endl;

	return 0;
}