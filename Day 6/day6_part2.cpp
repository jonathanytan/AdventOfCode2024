#include <algorithm> // find
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility> // pair
#include <vector>

// Function to hash <int, int> coordinate pairs into unique ints, as unordered_map doesn't have a hash function for pairs
int hash_int(std::pair<int, int> pos, int grid_size) {
    return pos.first * grid_size + pos.second;
}

// Function to unhash <int, int> coordinate pairs into unique ints, as unordered_map doesn't have a hash function for pairs
std::pair<int, int> unhash_int(int hashed_int, int grid_size) {
    int x = hashed_int / grid_size;   // integer division
    int y = hashed_int % grid_size;
    return {x, y};
}

// Function to print the 2D grid
void printGrid(const std::vector<std::string>& grid) {
    for (auto it = grid.begin(); it != grid.end(); it++)
        std::cout << *it << std::endl;
}

// Function that returns the guard's new orientation (90* CW rotation) when she hits an obstacle
char rightTurn(const char& c) {
    if (c == '^')
        return '>';
    else if (c == '>')
        return 'v';
    else if (c == 'v')
        return '<';
    else // (c == '<')
        return '^';
}

// Function which returns true if the guard is in the bounds of the map, false otherwise
bool guardIsInMap(std::pair<int, int> guard_pos, int map_length, int map_width) {
    if (guard_pos.first < 0 || guard_pos.second < 0 || guard_pos.first > map_length - 1 || guard_pos.second > map_width - 1)
        return false;
    else
        return true;
}

// Function to determine the guard's next step. Also rotates the guard's orientation in place if an obstacle was detected. 
// The final position it returns will be an out-of-bounds location so bounds checking outside of the function needs to be handled.
// Also flags if a right turn is needed by the final function parameter, as this info's needed for our printing functions
// to perform the expected activity
std::pair<int, int> nextLegalPosition(std::vector<std::string>& grid, std::pair<int, int>& guard_pos, bool& need_right_turn) {
    std::pair<int, int> next_step = guard_pos;
    char guard_orientation = grid[guard_pos.first][guard_pos.second];

    if (guard_orientation == '^') {
        next_step.first = guard_pos.first - 1;

    } else if (guard_orientation == '>') {
        next_step.second = guard_pos.second + 1;

    } else if (guard_orientation == 'v') {
        next_step.first = guard_pos.first + 1;

    } else if (guard_orientation == '<') {
        next_step.second = guard_pos.second - 1;
    }

    if (!guardIsInMap(next_step, grid.size(), grid[0].size()))
        return next_step;

    // if in the next step there's an obstacle, change the guard's orientation, make a recursive call
    // to find new next step given updated orientation
    if (grid[next_step.first][next_step.second] == '#') {
        need_right_turn = true;
        grid[guard_pos.first][guard_pos.second] = rightTurn(guard_orientation);
        next_step = nextLegalPosition(grid, guard_pos, need_right_turn);
    }

    return next_step;
}

// Function to mark guard's previous position on the grid with a character that indicates direction of travel (e.g. '+', '|', '-')
void markGuardInitialPosition(std::vector<std::string>& grid, std::unordered_map<int, std::string>& o_map,
    std::pair<int, int> initial_pos, std::pair<int, int> next_pos, bool right_turn_necessary) {

    int h = hash_int(initial_pos, grid.size());

    bool pointIntersectedByGuard = ((o_map[h].find('^') != std::string::npos || o_map[h].find('v') != std::string::npos)) 
                                   && 
                                   (o_map[h].find('<') != std::string::npos || o_map[h].find('>') != std::string::npos);

    if (right_turn_necessary) {
        grid[initial_pos.first][initial_pos.second] = '+';
    } else if (pointIntersectedByGuard) {
        grid[initial_pos.first][initial_pos.second] = '+';
    } else if ((initial_pos.first - next_pos.first != 0) && (initial_pos.second - next_pos.second == 0)) {
        grid[initial_pos.first][initial_pos.second] = '|';
    } else if ((initial_pos.first - next_pos.first == 0) && (initial_pos.second - next_pos.second != 0)) {
        grid[initial_pos.first][initial_pos.second] = '-';
    }
}

// Function that drives the first simulation of the guard's path. Returns false when the guard steps outside of the map and true otherwise.
// Stores the guard's current position and orientation in o_map and updates the guard's position guard_pos.
// Assumes there are no loops.
bool guardPatrolDriver(std::vector<std::string>& gr, std::unordered_map<int, std::string>& o_map, std::pair<int, int>& guard_pos) {
    bool right_turn_necessary = false;

    // (1) copy guard position and orientation into hash table
    o_map[hash_int(guard_pos, gr.size())].push_back(gr[guard_pos.first][guard_pos.second]);

    char guard_init_orientation = gr[guard_pos.first][guard_pos.second];

    // (2) find the guard's next legal position. nextPosition() also rotates guard's direction in grid if a right turn is necessary
    std::pair<int, int> guard_next_position = nextLegalPosition(gr, guard_pos, right_turn_necessary);

    // (3) check if next position is out of bounds
    if (!guardIsInMap(guard_next_position, gr.size(), gr[0].size()))
        return false;

    // copy guard (char representation) to position calculated by nextStep
    gr[guard_next_position.first][guard_next_position.second] = gr[guard_pos.first][guard_pos.second];

    // clean up the extra guard char by marking guard's previous position as visited
    markGuardInitialPosition(gr, o_map, guard_pos, guard_next_position, right_turn_necessary);

    // update guard (pair<int,int> representation) to next position
    guard_pos = guard_next_position;

    printGrid(gr);
    std::cout << "guard's updated position is: " << guard_pos.first << " ," << guard_pos.second << std::endl;
}

// Function that drives all simulations of the guard's path after the first. Returns false when either 
// 1) the guard steps outside of the map, 2) the guard enters a loop
bool guardPatrolDriver(std::vector<std::string>& gr, std::unordered_map<int, std::string>& o_map, std::unordered_map<int, int>& obstacle_locations, std::pair<int, int>& guard_pos, int& obs_found) {
    bool right_turn_necessary = false;

    char guard_init_orientation = gr[guard_pos.first][guard_pos.second];

    // the guard enters a loop if: 1) her current position is one that she had already visited, 
    //                             2) her second visit to this position is done with the same orientation as the first 
    //                                (e.g. first visit orientation: ^, second visit orientation: ^)
    if (o_map[hash_int(guard_pos, gr.size())].find(guard_init_orientation) != std::string::npos) {
        std::cout << "new obstacle entered causes loop" << guard_pos.first << ", " << guard_pos.second << std::endl;
        obs_found++;
        // determine obstacle's location based on guard's initial position
        if (guard_init_orientation == '<')
            obstacle_locations[guard_pos.first] = guard_pos.second - 1;
        else if (guard_init_orientation == '^')
            obstacle_locations[guard_pos.first - 1] = guard_pos.second;
        else if (guard_init_orientation == '>')
            obstacle_locations[guard_pos.first] = guard_pos.second + 1;
        else if (guard_init_orientation == 'v')
            obstacle_locations[guard_pos.first + 1] = guard_pos.second;

        return false;
    }

    o_map[hash_int(guard_pos, gr.size())].push_back(gr[guard_pos.first][guard_pos.second]);

    std::pair<int, int> guard_next_position = nextLegalPosition(gr, guard_pos, right_turn_necessary);

    if (!guardIsInMap(guard_next_position, gr.size(), gr[0].size()))
        return false;

    gr[guard_next_position.first][guard_next_position.second] = gr[guard_pos.first][guard_pos.second];

    markGuardInitialPosition(gr, o_map, guard_pos, guard_next_position, right_turn_necessary);

    guard_pos = guard_next_position;

    printGrid(gr);
    std::cout << "guard's updated position is: " << guard_pos.first << " ," << guard_pos.second << std::endl;
}

int main() {

    std::ifstream myFile;

    myFile.open("input.txt");

    std::string str;

    std::vector<std::string> grid; // Grid used in first simulation

	// Clean copy of the untraversed grid. We will use it to run simulations 2 and on
    std::vector<std::string> loop_detection_grid; 

    std::pair<int, int> guard_starting_position; 

    std::pair<int, int> guard_position;

    int obstacles_found = 0;

    // key: tiles that were visited by the guard; value: orientation of the guard at that tile
    // this struct used to determine when the guard enters a loop
    std::unordered_map<int, std::string> orientation_map;

    std::unordered_map<int, int> obstacle_locations; // key: row of obstacle, value: column of obstacle

    if (myFile.is_open()) {

        while (getline(myFile, str)) {

            if (str.find('^') != std::string::npos) { // find returns npos if the character could not be found in the string
                guard_position.first = guard_starting_position.first = grid.size();
                guard_position.second = guard_starting_position.second = str.find('^');
            }

            grid.push_back(str);
        }

        loop_detection_grid = grid;

        bool guardInMapNotLooped = true;

        // First simulation of the guard. Marks the grid, saves visited coordinates and associated orientations in orientation_map, 
        // and updates guard_position with each loop. No loop finding or new obstacle placing yet.
        while (guardInMapNotLooped) {
            guardInMapNotLooped = guardPatrolDriver(grid, orientation_map, guard_position);
        }

        grid[guard_starting_position.first][guard_starting_position.second] = '^'; // Re-mark guard's starting position in case it was re-occupied
        printGrid(grid);
        std::cout << "guard stepped out of bounds at final position: " << guard_position.first << " ," << guard_position.second << std::endl;

        // Set up of all future simulations where we place new obstacles along the path traveled in the first simulation and try to find loops
        for (const auto& [key, value] : orientation_map) {
            std::pair<int, int> unhashed_pair = unhash_int(key, loop_detection_grid.size());

            if (unhashed_pair == guard_starting_position) // do not try to place an obstacle at the guard's starting location
                continue;
            else {
                auto temp_grid = loop_detection_grid;
                auto temp_guard_pos = guard_starting_position;
                std::unordered_map<int, std::string> temp_omap;
                
                temp_grid[unhashed_pair.first][unhashed_pair.second] = '#'; // Add new obstacle

                printGrid(temp_grid);
                std::cout << "new obstacle placed at " << unhashed_pair.first << ", " << unhashed_pair.second << std::endl;

                guardInMapNotLooped = true;

                while (guardInMapNotLooped) {
                    guardInMapNotLooped = guardPatrolDriver(temp_grid, temp_omap, obstacle_locations, temp_guard_pos, obstacles_found);
                }
            }
        }
    }
    std::cout << "loops found = " << obstacles_found << std::endl;
    return 0;
}