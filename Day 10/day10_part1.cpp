#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <stack>

int hash_coord(int grid_length, std::pair<int, int> coord) {
    return grid_length * coord.first + coord.second;
}

int hash_coord(int grid_length, int row, int col) {
    return grid_length * row + col;
}

std::pair<int, int> unhash_coord(int grid_length, int hash_value) {
    return std::make_pair(hash_value/grid_length, hash_value % grid_length);
}

bool coordinateIsInBounds(const std::vector<std::vector<int>>& topo_map, std::pair<int, int> coordinate) {
    if(coordinate.first >= 0 && coordinate.first < topo_map.size() && coordinate.second >= 0 && coordinate.second < topo_map[0].size())
        return true;
    else
        return false;
}

bool elevationGainViable(const std::vector<std::vector<int>>& topo_map, std::pair<int,int> current_step, std::pair<int,int> next_step) {
    if(topo_map[current_step.first][current_step.second] + 1 == topo_map[next_step.first][next_step.second])
        return true;
    else
        return false;
}

bool potentialNextStepIsValid(const std::vector<std::vector<int>>& topo_map, 
                                      std::pair<int,int> current_step, std::pair<int,int> direction) {
    std::pair<int,int> next_step(current_step.first + direction.first, current_step.second + direction.second);
    
    if(coordinateIsInBounds(topo_map, next_step) && elevationGainViable(topo_map, current_step, next_step))
        return true;
    else
        return false;
}

int exploreTrailhead(const std::vector<std::vector<int>>& topo_map, std::pair<int, int> trailhead) {

    std::unordered_set<int> visited_nine_heights;

    // up, right, down, left
    std::vector<std::pair<int,int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    std::stack<std::pair<int,int>> s;
    s.push(trailhead);

    while(!s.empty()) {

        std::pair<int, int> current_step = s.top();
        s.pop();

        // if the current step is a 9-height, check if it's been visited. if it hasn't been, add it to the list
        // of visited 9-heights so we don't double count it in the case that there are multiple paths leading
        // from the trail head to that particular 9-height
        if(topo_map[current_step.first][current_step.second] == 9 
        && visited_nine_heights.find(hash_coord(topo_map.size(), current_step)) == visited_nine_heights.end()) {
            visited_nine_heights.insert(hash_coord(topo_map.size(), current_step));
        }
        
        // iterate through potential neighbors
        for(int i = 0; i < directions.size(); i++) {
            if(potentialNextStepIsValid(topo_map, current_step, directions[i])) {
                s.push(std::make_pair(current_step.first + directions[i].first, current_step.second + directions[i].second));
            }
        }
    }
    return visited_nine_heights.size();
}



int main() {

    std::ifstream my_file;
    my_file.open("input-small.txt");
    std::string str;

    std::vector<std::vector<int>> topo_map;
    std::unordered_set<int> trailheads;

    int all_trailhead_scores = 0;

    if(my_file.is_open()) {
        while(getline(my_file, str)) {
            std::istringstream iss(str);
            char c;
            std::vector<int> topo_map_row;
            while (iss >> c) {
                topo_map_row.push_back(c - '0');
            }
            topo_map.push_back(topo_map_row);
        }

        // find trailheads and place their locations in std::unordered_set<int> trailheads
        for(auto it_r = topo_map.begin(); it_r != topo_map.end(); it_r++) { // iterators for practice
            for(auto it_c = it_r->begin(); it_c != it_r->end(); it_c++) {
                if(*it_c == 0)
                    trailheads.emplace(hash_coord(topo_map.size(), it_r - topo_map.begin(), it_c - it_r->begin()));
            }
        }

        for(const & t : trailheads) {
            all_trailhead_scores += exploreTrailhead(topo_map, unhash_coord(topo_map.size(), t));
        }
    }

    std::cout << all_trailhead_scores << std::endl;

    return 0;
}