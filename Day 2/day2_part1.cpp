#include <fstream>
#include <vector>
#include <iterator> // for istream iterator
#include <iostream>
#include <sstream> // for objs of type istringstream

int main () {

    std::ifstream myFile("input.txt");

    std::vector<std::vector<int>> all_reports;

    if (myFile.is_open()) {
        std::string str;

        // found on stackoverflow, need to understand how this works
        while (std::getline(myFile, str)) {
            std::vector<int> temp_report; 
            std::istringstream iss(str);
            std::copy(std::istream_iterator<int>(iss),
                std::istream_iterator<int>(),
                std::back_inserter(temp_report));
            all_reports.push_back(temp_report);
        }
    }

    myFile.close();
    
    int safe_reports = 0;

    for (int i = 0; i < all_reports.size(); i++) {
        bool decreasing = true;
        bool increasing = true;
        bool safe_delta = true;

        for (int j = 0; j < all_reports[i].size() - 1; j++) {

            if (all_reports[i][j] > all_reports[i][j + 1])
                increasing = false;

            if (all_reports[i][j] < all_reports[i][j + 1])
                decreasing = false;

            int adjacent_delta = abs(all_reports[i][j] - all_reports[i][j + 1]);

            if(adjacent_delta < 1 || adjacent_delta > 3)
                safe_delta = false;
        }

        if ((increasing || decreasing) && safe_delta)
            safe_reports++;
    }

    std::cout << safe_reports << std::endl;

    return 0;
}