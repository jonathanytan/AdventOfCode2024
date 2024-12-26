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

// Prints grid
void printGrid(const std::vector<std::string> &grid) {
    for (auto it = grid.begin(); it != grid.end(); it++)
        std::cout << *it << std::endl;
}

int hash_func(std::pair<int,int> p, int grid_length) {
    return p.first * grid_length + p.second;
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
        for(int i = 0; i < value.size() - 1; i++) {
            for(int j = i + 1; j < value.size(); j++) {

                // find antinode positions using vector addition learned in high school physics
                std::pair<int,int> antinode1 = value[i] + (value[i] - value[j]);
                std::pair<int,int> antinode2 = value[j] + (value[j] - value[i]);

                int hashed_antinode1 = hash_func(antinode1, grid.size());
                int hashed_antinode2 = hash_func(antinode2, grid.size());

                // check that antinodes are in bounds of graph and that we're not adding duplicates
                if(nodeIsInGraph(antinode1, grid.size(), grid[0].size()) && antinodes.find(hashed_antinode1) == antinodes.end())
                    antinodes.insert(hashed_antinode1);
                if(nodeIsInGraph(antinode2, grid.size(), grid[0].size()) && antinodes.find(hashed_antinode2) == antinodes.end())
                    antinodes.insert(hashed_antinode2);
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
        
        printGrid(grid);
        calculateAntinodes(grid, antennas, antinodes);
    }

    std::cout << "number of antinodes in grid: " << antinodes.size() << std::endl;

    my_file.close();
    return 0;
}