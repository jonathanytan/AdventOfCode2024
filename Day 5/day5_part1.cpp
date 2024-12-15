#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <sstream> // isstringstream


std::vector<int> removeCommaAndParseLine1(std::string& str) {
	char removal_char = ',';
	std::replace(str.begin(), str.end(), removal_char, ' ');
	std::istringstream ss(str);
	int temp;
	std::vector<int> vec;
	while (ss >> temp) {
		vec.push_back(temp);
	}
	return vec;
}

std::vector<int> removeCommaAndParseLine2(std::string& str) {
	std::vector<int> vec;

	std::string temp;
	std::istringstream ss(str);

	std::cout << "str = " << str << std::endl;

	while(getline(ss, temp, ',')) {
		std::cout << "temp = " << temp << std::endl;
		vec.push_back(stoi(temp));
	}
	return vec;
}



int main() {

	std::ifstream myFile;
	myFile.open("input.txt");

	std::string str;

	// an adjacency list
	std::unordered_map<int, std::vector<int>> my_map;

	std::vector<std::vector<int>> update_list;

	int key, value;

	bool blank_line_encountered = false;

	int correct_report_middle_sum = 0;

	if (myFile.is_open()) {

		// read page ordering rules and put them into hash map
		while (getline(myFile, str)) {

			if (str.empty()) {
				blank_line_encountered = true;
				continue;
			}

			if (!blank_line_encountered) {
				// generalizes for page numbers with any number of digits although not
				// required for the problem
				int pipe_index = str.find('|');
				int first_num_digits = pipe_index;
				int second_num_digits = str.length() - pipe_index - 1;

				key = stoi(str.substr(0, first_num_digits));
				value = stoi(str.substr(pipe_index + 1, second_num_digits));
				my_map[key].push_back(value);
			
		// read updates and put them into a vector<vector<int>>
			} else { 		
				update_list.push_back(removeCommaAndParseLine2(str));
			}
		}

		

		for (int i = 0; i < update_list.size(); i++) { // iterate through all updates
			bool rule_violated = false;

			// in each individual update, we're making comparisons between two numbers
			// so we need two iterators: j and k. j is "stationary" and k moves.
			for (int j = 0; j < update_list[i].size(); j++) {

				// note the indices for k. we only need to check when required orders are violated,
				for (int k = 0; k < j - 1; k++) {
						auto it = find(my_map[update_list[i][j]].begin(), my_map[update_list[i][j]].end(), update_list[i][k]);
						
						if (it != my_map[update_list[i][j]].end()) {
							rule_violated = true;
							std::cout << "rule violated in report" << i << ": " << update_list[i][j] << '|' << update_list[i][k] << std::endl;
						}
				}
			}
			if (!rule_violated) {
				correct_report_middle_sum += update_list[i][update_list[i].size() / 2];
			}
		}
	}

	std::cout << correct_report_middle_sum << std::endl;

	myFile.close();
	return 0;
}