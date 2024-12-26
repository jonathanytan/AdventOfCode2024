#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm> // find

// override binary + operator to add and subtract pair<int,int>
template <typename T,typename U>                                                   
std::pair<T,U> operator+(const std::pair<T,U> & l,const std::pair<T,U> & r) {   
    return {l.first+r.first,l.second+r.second};                                    
}                

template <typename T,typename U>                                                   
std::pair<T,U> operator-(const std::pair<T,U> & l,const std::pair<T,U> & r) {   
    return {l.first-r.first,l.second-r.second};                                    
}

int hash_func(std::pair<int,int> p, int grid_length) {
    return p.first * grid_length + p.second;
}

// Prints grid
void printGrid(const std::vector<std::string> &grid) {
    for (auto it = grid.begin(); it != grid.end(); it++)
        std::cout << *it << std::endl;
}

void addAntinodesToGrid(std::vector<std::string> &grid, std::unordered_set<int>& antinodes) {
    for(int i : antinodes) {
        grid[i/grid.size()][i%grid.size()] = '#';
    }
}

bool nodeIsInGraph(const std::pair<int,int>& p, int graph_length, int graph_width) {
    if(p.first >= 0 && p.first < graph_length && p.second >= 0 && p.second < graph_width)
        return true;
    else
        return false;
}

void calculateAntinodes(const std::vector<std::string>& grid, 
                        const std::unordered_map<char, std::vector<std::pair<int, int>>>& antennas, 
                        std::unordered_set<int>& antinodes) {
    for (const auto& [key, value] : antennas) {
        
        // normally the condition would be i < value.size() - 1 for this loop because we are 
        // evaluating each element against every other element in the vector,
        // so we would not need to compare the last element to itself. however, 
        // we need the two statements at the beginning of this 'i' for loop to run
        // for the last element so we allow the loop to iterate over it by removing the "-1"
        // otherwise, the final node wouldn't get added to the antinode list
        for(int i = 0; i < value.size(); i++) { 
            std::pair<int,int> antinode_antenna = value[i];
            if(antinodes.find(hash_func(antinode_antenna, grid.size())) == antinodes.end())
                antinodes.insert(hash_func(antinode_antenna, grid.size()));

            for(int j = i + 1; j < value.size(); j++) {

                // for each pair of antennae we are evaluating, the vector (magnitude and direction)
                // that goes from one antenna's position to that of the other is given by subtracting 
                // one's position from the other (both positions desceribed as vectors).
                // to get the same vector of the opposite direction, you reverse the terms in the expression
                // (e.g. where v1 and v2 are vectors, v1-v2 and v2-v1 have the same magnitude and opposite
                // directions). so we follow these directional vectors from an initial point - given by
                // the coordinates of each antenna of the pair - to extrapolate other antinodes.
                std::pair<int,int> antinode1 = value[i] + (value[i] - value[j]);
                std::pair<int,int> antinode2 = value[j] + (value[j] - value[i]);

                // check that antinodes are in bounds of graph and that we're not adding duplicates
                while(nodeIsInGraph(antinode1, grid.size(), grid[0].size())) {
                    int hashed_antinode1 = hash_func(antinode1, grid.size());
                    if(antinodes.find(hashed_antinode1) == antinodes.end())
                        antinodes.insert(hashed_antinode1);
                    antinode1 = antinode1 + value[i] - value[j];
                }

                while(nodeIsInGraph(antinode2, grid.size(), grid[0].size())) {
                    int hashed_antinode2 = hash_func(antinode2, grid.size());
                    if(antinodes.find(hashed_antinode2) == antinodes.end())
                        antinodes.insert(hashed_antinode2);
                    antinode2 = antinode2 + value[j] - value[i];
                }
            }
        }
    }
}

int main () {

    std::ifstream my_file;
    my_file.open("input.txt");

    std::vector<std::string> grid;
    std::unordered_map<char, std::vector<std::pair<int, int>>> antennas;
    std::unordered_set<int> antinodes; // stores list of pair<int,int> that get hashed into integers
    std::string str;

    if(my_file.is_open()) {
        while(getline(my_file, str)) {
            for(int i = 0; i < str.size(); i++) {
                if(isalnum(str[i]))
                    antennas[str[i]].push_back(std::make_pair(grid.size(), i));
            }
            grid.push_back(str);
        }

        calculateAntinodes(grid, antennas, antinodes);
        addAntinodesToGrid(grid, antinodes);
        printGrid(grid);
    }

    std::cout << "number of antinodes in grid: " << antinodes.size() << std::endl;

    my_file.close();
    return 0;
}