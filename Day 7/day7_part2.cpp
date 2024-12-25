#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm> // find

bool allOperatorsPlaced(std::vector<char> ops) {
    if (std::find(ops.begin(), ops.end(), '-') == ops.end()) // if no '-' could be found
        return true;
    else
        return false;
}

// assume expression_values.size() > 1
long long getExpressionTotalSoFar(const std::vector<long long>& expression_values, std::vector<char> operators) {
    
    long long total = expression_values[0];
    
    for(int i = 1; i < expression_values.size(); i++) {
        if(operators[i - 1] == '-')
            break;
        else if(operators[i - 1] == '*')
            total *= expression_values[i];
        else if(operators[i - 1] == '|')
            total = stoll(std::to_string(total) + std::to_string(expression_values[i]));
        else if(operators[i - 1] == '+')
            total += expression_values[i];
    }
    return total;
}

bool backtrack(long long test_value, const std::vector<long long>& expression_numbers, std::vector<char>& operators) {

    long long expression_total_so_far = getExpressionTotalSoFar(expression_numbers, operators);

    if(expression_total_so_far == test_value && allOperatorsPlaced(operators)) {
        return true;
    } else {
        for(int i = 0; i < operators.size(); i++) {
            if(operators[i] == '-') {
                operators[i] = '*';
                if(backtrack(test_value, expression_numbers, operators)) 
                    return true;
                else if(operators[i] == '*') {
                    operators[i] = '|';
                    if(backtrack(test_value, expression_numbers, operators)) {
                        return true;
                    } else if(operators[i] == '|') {
                        operators[i] = '+';
                        if(backtrack(test_value, expression_numbers, operators)) {
                            return true;
                        } else if(operators[i] == '+') {
                            operators[i] = '-';
                            return false;
                        }
                    } else if(operators[i] == '+') {
                        operators[i] = '-';
                        return false;
                    }
                } else if(operators[i] == '|') {
                    operators[i] = '+';
                    if(backtrack(test_value, expression_numbers, operators)) {
                        return true;
                    } else if(operators[i] == '+') {
                        operators[i] = '-';
                        return false;
                    }
                } else if(operators[i] == '+') {
                    operators[i] = '-';
                    return false;
                }
            }
        }
        return false; // reached the bottom of the search tree and did not get a valid combination of operators
    }
}

int main () {
    std::ifstream my_file;
    my_file.open("input.txt");
    std::string str;

    std::vector<long long> test_values;
    std::vector<std::vector<long long>> all_expression_numbers;

    long long calibration_result = 0;

    if(my_file.is_open()) {

        // read values from input
        while(getline(my_file, str, ':')) { // reads in string of number before colon
            test_values.push_back(stoull(str));
            getline(my_file, str); // reads rest of string after colon 
            std::istringstream iss(str);
            long long num;
            std::vector<long long> single_expression_numbers;
            while(iss >> num) {
                single_expression_numbers.push_back(num);
            }
            all_expression_numbers.push_back(single_expression_numbers);
        }

        for(int i = 0; i < test_values.size(); i++) {
            std::vector<char> operators(all_expression_numbers[i].size() - 1, '-');
            if (backtrack(test_values[i], all_expression_numbers[i], operators)) {
                calibration_result += test_values[i];
                std::cout << test_values[i] << std::endl;
            }
        }

        std::cout << "Total calibration result is: " << calibration_result << std::endl;

    }
    my_file.close();
    return 0;
}