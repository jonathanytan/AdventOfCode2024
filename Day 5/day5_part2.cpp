#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <queue>

std::vector<int> topologicalSort(std::unordered_map<int, std::vector<int>>& adj, std::vector<int>& update_list) {
	std::unordered_map<int, int> indegree;
	std::vector<int> linear_ordering; // the solution the function returns

	// begin by inserting the pages (nodes) of a particular update into the map and initialize them to zero
	for (auto& it : update_list) {
		indegree[it] = 0;
	}

	// populate the indegree list by traversing the adjacency list
	// we are only interested in keys and values which are present in the update so we need one check for the former
	// and latter
	for (const auto& [key, value] : adj) {

		auto itr = find(update_list.begin(), update_list.end(), key);

		// if current key is present in update list...
		if (itr != update_list.end()) {

			// ...iterate through that key's associated values, i.e. the nodes (pages) which are dependent on that key
			for (int i = 0; i < value.size(); i++) {

				// if that key is in the update list...
				auto itr_1 = find(update_list.begin(), update_list.end(), value[i]);

				if (itr_1 != update_list.end()) {
					// ... then increment its indegree
					indegree[value[i]]++;
				}
			}
		}
	}

	std::queue<int> q;

	// find the nodes with 0 indegree and add them to the queue
	for (const auto& [key, value] : indegree) {
		if (value == 0)
			q.push(key);
	}

	int count = 0;

	while (!q.empty()) {
		int current_node = q.front();

		for (int i = 0; i < adj[current_node].size(); i++) {

			// "remove" the outgoing edges of the current node
			// by decrementing the indegree of their target
			indegree[adj[current_node][i]]--;

			// if the target's indegree is zero after the decrement,
			// add it to the queue
			if (indegree[adj[current_node][i]] == 0)
				q.push(adj[current_node][i]);
		}

		linear_ordering.push_back(current_node);
		q.pop();
		count++;
	}

	return linear_ordering;
}

std::vector<int> removeCommaAndParseLine2(std::string& str) {
	std::vector<int> vec;

	std::string temp;
	std::istringstream ss(str);

	std::cout << "str = " << str << std::endl;

	while (getline(ss, temp, ',')) {
		std::cout << "temp = " << temp << std::endl;
		vec.push_back(stoi(temp));
	}
	return vec;
}

int main() {

	int v = 0;

	std::ifstream myFile("input.txt");

	std::unordered_map<int, std::vector<int>> adjacency_list;
	std::vector<std::vector<int>> update_list;

	std::vector<int> nodes;

	bool blank_line_encountered = false;

	std::string str;

	int corrected_report_midpoint_sum = 0;

	if (myFile.is_open()) {


		while (getline(myFile, str)) {

			if (str.empty()) {
				blank_line_encountered = true;
				continue;
			}

			// 1) read page ordering rules and put them into a hash map
			if (!blank_line_encountered) {
				// generalizes for page numbers with any number of digits although not
				// required for the problem
				int pipe_index = str.find('|');
				int first_num_digits = pipe_index;
				int second_num_digits = str.length() - pipe_index - 1;

				int from_node = stoi(str.substr(0, first_num_digits));
				int to_node = stoi(str.substr(pipe_index + 1, second_num_digits));

				auto it = find(adjacency_list[from_node].begin(), adjacency_list[from_node].end(), to_node);
				if (it == adjacency_list[from_node].end()) {
					adjacency_list[from_node].push_back(to_node);
				}

				// 2) read updates and put them into a vector<vector<int>>
			}
			else {
				update_list.push_back(removeCommaAndParseLine2(str));
			}
		}

		for (int i = 0; i < update_list.size(); i++) { // iterate through all updates
			bool rule_violated = false;

			// in each individual update, we're making comparisons between two numbers
			// so we need two iterators: j and k. j is "stationary" and k moves.
			for (int j = 0; j < update_list[i].size(); j++) {

				// note the indices for k. we only need to check when required orders are violated,
				for (int k = 0; k < j; k++) {
					auto it = find(adjacency_list[update_list[i][j]].begin(), adjacency_list[update_list[i][j]].end(), update_list[i][k]);

					if (it != adjacency_list[update_list[i][j]].end()) {
						rule_violated = true;
						std::cout << "rule violated in report" << i << ": " << update_list[i][j] << '|' << update_list[i][k] << std::endl;
					}
				}
			}

			if (rule_violated) {
				std::vector<int> linear_ordering = topologicalSort(adjacency_list, update_list[i]);

				corrected_report_midpoint_sum += linear_ordering[linear_ordering.size() / 2];
			}
		}
	}

	std::cout << corrected_report_midpoint_sum << std::endl;

	myFile.close();

	return 0;
}