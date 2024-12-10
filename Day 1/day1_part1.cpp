#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int main() {
    int a, b;

    std::vector<int> listOne, listTwo;

    std::ifstream myFile("input.txt");

    if (myFile.is_open()) {
        while (myFile >> a >> b) {
            listOne.push_back(a);
            listTwo.push_back(b);
        }
    }

    myFile.close();

    sort(listOne.begin(), listOne.end());
    sort(listTwo.begin(), listTwo.end());

    int total_distance = 0;

    // easiest way is probably to use indices.
    // if you wanted to use iterators, use two, one for each vector.
    for (int i = 0; i < listOne.size(); i++) {
        total_distance += abs(listOne[i] - listTwo[i]);
    }

    std::cout << total_distance << std::endl;

    return 0;
}