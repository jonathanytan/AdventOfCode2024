#include <fstream>
#include <iostream>
#include <string>
#include <utility> // pair
#include <vector>

// Prints grid
void printGrid(const std::vector<std::string> &grid) {
    for (auto it = grid.begin(); it != grid.end(); it++)
        std::cout << *it << std::endl;
}

// Function to count all the visited spots on the grid once the guard steps out of bounds
int tabulateXs(const std::vector<std::string> &grid) {
    int counter = 0;

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (grid[i][j] == 'X')
                counter++;
        }
    }

    return counter;
}

// Function to give guard's new orientation when she hits an obstacle
char rightTurn(const char &c) {
    if (c == '^')
        return '>';
    else if (c == '>')
        return 'v';
    else if (c == 'v')
        return '<';
    else if (c == '<')
        return '^';
}

// Function to check if the guard is still in the bounds of the map
bool guardIsInMap(std::pair<int, int> guard_pos, int map_length, int map_width) {
    if (guard_pos.first < 0 || guard_pos.second < 0 || guard_pos.first > map_length - 1 || guard_pos.second > map_width - 1)
        return false;
    else
        return true;
}

// Function to determine the guard's next step. The final position it returns will be an
// out-of-bounds location so be careful.
std::pair<int, int> nextStep(std::vector<std::string> &grid, std::pair<int, int> &guard_pos) {
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
        grid[guard_pos.first][guard_pos.second] = rightTurn(guard_orientation);
        next_step = nextStep(grid, guard_pos);
    }

    return next_step;
}

int main() {

    std::ifstream myFile;

    myFile.open("input.txt");

    std::string str;

    std::vector<std::string> grid;

    std::pair<int, int> guard_position;

    if (myFile.is_open()) {

        while (getline(myFile, str)) {

            std::cout << str << std::endl;

            // find returns npos if the character could not be found in the string
            if (str.find('^') != std::string::npos) {
                guard_position.first = grid.size();
                guard_position.second = str.find('^');
            }

            grid.push_back(str);
        }

        while (guardIsInMap(guard_position, grid.size(), grid[0].size())) {

            // find the guard's next position
            std::pair<int, int> guard_next_position = nextStep(grid, guard_position);

            // check if next position is out of bounds
            if (!guardIsInMap(guard_next_position, grid.size(), grid[0].size()))
                break;

            // move guard to next position
            grid[guard_next_position.first][guard_next_position.second] =
                grid[guard_position.first][guard_position.second];

            // mark guard's previous position as visited
            grid[guard_position.first][guard_position.second] = 'X';

            // update guard's current position
            guard_position = guard_next_position;

            printGrid(grid);
            std::cout << "guard's updated position is: " << guard_position.first << " ," << guard_position.second << std::endl;
        }

        // when guard steps out of bounds, mark its last position in the grid as an "X"
        grid[guard_position.first][guard_position.second] = 'X';
        printGrid(grid);
        std::cout << "guard stepped out of bounds at final position: " << guard_position.first << " ," << guard_position.second << std::endl;
    }

    std::cout << "X count = " << tabulateXs(grid) << std::endl;

    return 0;
}