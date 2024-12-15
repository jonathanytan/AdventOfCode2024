#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

int main() {
	std::ifstream myFile("input.txt");
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

	// remove all bounds checking statements from part 1, initializing loop variables
	// to the expression below does the bounds checking
	for (int i = target_word.size()/2; i < grid.size() - 1; i++) {
		for (int j = target_word.size()/2; j < grid[i].size() - 1; j++) {

			// only begin searching if we encounter the middle letter of our target word,
			// i.e., the point of intersection - at the center - of each "X"
			if (grid[i][j] != target_word[1])
				continue;

			std::string first_wing; // the line "\" in an X
			std::string second_wing;// the line "/" in an X

			int k = - 1;

			// spent a while to figure out the error, .length returns an unsigned int which 
			// becomes a problem when you compare it to a negative. still having some issue 
			// writing as a for loop but code works currently
			while (k < static_cast<int>(target_word.length() - 1)) {
				first_wing.push_back(grid[i + k][j + k]);
				second_wing.push_back(grid[i + k][j - k]);
				k++;
			}

			if ((first_wing == target_word || first_wing == target_word_reversed)
				&& (second_wing == target_word || second_wing == target_word_reversed)) {
				target_figure_occurences++;
			}
		}
	}

	std::cout << target_figure_occurences << std::endl;

	return 0;
}