#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>


int main() {

	std::ifstream myFile("input.txt");\
	std::vector<std::string> grid;
	std::string temp_str;

	if (myFile.is_open()) {
		while (getline(myFile, temp_str)) {
			grid.push_back(temp_str);
		}
	}

	myFile.close();

	std::string target_word = "XMAS";

	std::string target_word_reversed = target_word;
	std::reverse(target_word_reversed.begin(), target_word_reversed.end());

	int target_word_occurrences = 0;

	for (int i = 0; i < grid.size(); i++) {
		for (int j = 0; j < grid[i].size(); j++) {

			// only begin searching if we encounter the first letter of our target word
			if (grid[i][j] != target_word[0])
				continue;

			bool forward_bounds_valid = j <= grid[i].size() - target_word.size();
			bool backward_bounds_valid = j >= target_word.size() - 1;
			bool up_bounds_valid = i >= target_word.size() - 1;
			bool down_bounds_valid = i <= grid.size() - target_word.size();

			// XMAS
			if (forward_bounds_valid) {
				std::string string_inspected = grid[i].substr(j, target_word.size());
				if (string_inspected == target_word) {
					target_word_occurrences++;
				}
			}

			// SMAX
			if (backward_bounds_valid) {
				std::string string_inspected = grid[i].substr(j - target_word.size() + 1, target_word.size());
				if (string_inspected == target_word_reversed) {
					target_word_occurrences++;
				}
			}

			// S...
			// .A..
			// ..M.
			// ...X
			if (backward_bounds_valid && up_bounds_valid) { // NW
				std::string string_inspected;
				for (int k = 0; k < target_word.size(); k++) {
					string_inspected.push_back(grid[i - k][j - k]);
				}
				if (string_inspected == target_word) {
					target_word_occurrences++;
				}
			}

			// ...S
			// ..A.
			// .M..
			// X...
			if (forward_bounds_valid && up_bounds_valid) {
				std::string string_inspected;
				for (int k = 0; k < target_word.size(); k++) {
					string_inspected.push_back(grid[i - k][j + k]);
				}
				if (string_inspected == target_word) {
					target_word_occurrences++;
				}
			}
			
			// ...X
			// ..M.
			// .A..
			// S...
			if (backward_bounds_valid && down_bounds_valid) {
				std::string string_inspected;
				for (int k = 0; k < target_word.size(); k++) {
					string_inspected.push_back(grid[i + k][j - k]);
				}
				if (string_inspected == target_word) {
					target_word_occurrences++;
				}
			}

			// X...
			// .M..
			// ..A.
			// ...S
			if (forward_bounds_valid && down_bounds_valid) {
				std::string string_inspected;
				for (int k = 0; k < target_word.size(); k++) {
					string_inspected.push_back(grid[i + k][j + k]);
				}
				if (string_inspected == target_word) {
					target_word_occurrences++;
				}
			}

			// ..X.
			// ..M.
			// ..A.
			// ..S.
			if (down_bounds_valid) {
				std::string string_inspected;
				for (int k = 0; k < target_word.size(); k++) {
					string_inspected.push_back(grid[i + k][j]);
				}

				if (string_inspected == target_word) {
					target_word_occurrences++;
				}
			}

			// ..S.
			// ..A.
			// ..M.
			// ..X.
			if (up_bounds_valid) {
				std::string string_inspected;
				for (int k = 0; k < target_word.size(); k++) {
					string_inspected.push_back(grid[i - k][j]);
				}

				if (string_inspected == target_word) {
					target_word_occurrences++;
				}

			}
		}
	}

	std::cout << target_word_occurrences << std::endl;

	return 0;
}