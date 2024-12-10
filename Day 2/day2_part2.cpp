#include <fstream>
#include <vector>
#include <iterator> // for istream iterator
#include <iostream>
#include <sstream> // for objs of type istringstream


bool safeWithoutDampener(std::vector<int>& report) {
    bool decreasing = true;
    bool increasing = true;
    bool safe_delta = true;

    for(int i = 0; i < report.size() - 1; i++) {
        if (report[i] > report[i+1])
            increasing = false;

        if (report[i] < report[i+1])
            decreasing = false;

        int adjacent_delta = abs(report[i] - report[i+1]);

        if(adjacent_delta < 1 || adjacent_delta > 3)
            safe_delta = false;
    }

    if ((increasing || decreasing) && safe_delta)
        return true;
    else
        return false;
}

bool safeWithDampener(std::vector<int>& report) {
    for(int i = 0; i < report.size(); i++) {
        std::vector<int> temp_report = report;
        temp_report.erase(temp_report.begin() + i);
        if(safeWithoutDampener(temp_report))
            return true;
    }
    return false;
}

int main () {

    std::ifstream myFile("input.txt");

    std::vector<std::vector<int>> report;

    if (myFile.is_open()) {
        std::string str;

        while (std::getline(myFile, str)) {
            std::vector<int> temp_report; 
            std::istringstream iss(str);
            std::copy(std::istream_iterator<int>(iss),
                std::istream_iterator<int>(),
                std::back_inserter(temp_report));
            report.push_back(temp_report);
        }
    }

    myFile.close();
    
    int safe_reports = 0;

    for (int i = 0; i < report.size(); i++) {
        if(safeWithoutDampener(report[i])) {
            safe_reports++;
        } else if(safeWithDampener(report[i]))
            safe_reports++;
        }

    std::cout << safe_reports << std::endl;

    return 0;
}