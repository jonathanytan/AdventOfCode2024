#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>

int main () {
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

    int similarity_score = 0;

    std::unordered_map<int, int> map;

    for (int i = 0; i < listTwo.size(); i++) {
        map[listTwo[i]]++;
    }

    for (int i = 0; i < listOne.size(); i++) {
        similarity_score += listOne[i] * map[listOne[i]];
    }

    std::cout << similarity_score << std::endl;

    return 0;
}