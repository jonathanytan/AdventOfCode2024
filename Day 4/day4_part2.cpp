#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

int main() {
	std::ifstream myFile("input-test.txt");
	std::vector<std::string> grid;

	std::string temp_str;

	if (myFile.is_open()) {
		while (getline(myFile, temp_str)) {
			grid.push_back(temp_str);
		}
	}

	myFile.close();

	std::string target_word = "MAS";
	std::string target_word_reversed = target_word;
	std::reverse(target_word_reversed.begin(), target_word_reversed.end());

	int target_figure_occurences = 0;

	for (int i = 1; i < grid.size() - 1; i++) {
		for (int j = 1; j < grid[i].size() - 1; j++) {

			// only begin searching if we encounter the first letter of our target word.
			// X figures require all bounds
			if (grid[i][j] != target_word[1])
				continue;

			std::string first_wing; // the line "\" in an X
			std::string second_wing;// the line "/" in an X

			for(int k = -1; k < target_word.size() - 1; k++) {
				first_wing.push_back(grid[i + k][j + k]);
				second_wing.push_back(grid[i - k][j - k]);
			}

			if((first_wing == target_word || first_wing == target_word_reversed) 
				&& (second_wing == target_word || second_wing == target_word_reversed)) {
					target_figure_occurences++;
				}
		}
	}

	std::cout << target_figure_occurences << std::endl;

	return 0;	
}