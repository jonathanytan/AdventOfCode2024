#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

int computeChecksum (const std::string& str) {
    int check_sum = 0;

    for(int i = 0; i < str.size(); i++) {
        if(str[i] != '.')
            check_sum += (str[i] - '0') * i;
    }
    return check_sum;
}

// Function which appends to block_rep num copies of char c to block_rep based on whether it's empty blocks or occupied by files
void appendBlocks(std::string& block_rep, char c, int num) {
    std::string s(num, c); // not sure why we can't combine these two into one line
    block_rep = block_rep.append(s);
}

std::string convertDiskMapToBlockForm(const std::string& disk_map, std::vector<int>& file_sizes) {
    std::string block_rep;
    int id_number = 0;

    for(int i = 0; i < disk_map.size(); i++) {

        char number_of_memory_blocks = disk_map[i] - '0'; // - '0' converts char into an integer

        if(i % 2 == 0) { // if the number is even, it's file
            // id_number + '0': converts the integer id_number to its char counterpart
            appendBlocks(block_rep, id_number + '0', number_of_memory_blocks);        
            file_sizes.push_back(number_of_memory_blocks);
            id_number++;
        } else // if odd, it's empty space
            appendBlocks(block_rep, '.', number_of_memory_blocks);  
    }
    return block_rep;
}

void defragment(std::vector<int>& file_sizes, std::string& block_rep) {

    std::vector<int>::iterator it = file_sizes.end() - 1;

    int memory_blocks_used = 0;

    for(int& n : file_sizes)
        memory_blocks_used += n;

    while(it != file_sizes.begin()) {

        int fwd_idx = block_rep.find('.');
        int bwd_idx = block_rep.rfind(distance(file_sizes.begin(), it) + '0');
        bool memory_fragmented = fwd_idx < bwd_idx;

        for(int i = 0; i < *it && memory_fragmented; i++) {
            char temp = block_rep[fwd_idx];
            block_rep[fwd_idx] = block_rep[bwd_idx];
            block_rep[bwd_idx] = temp;

            fwd_idx = block_rep.find('.');
            bwd_idx = block_rep.rfind(distance(file_sizes.begin(), it) + '0');
            memory_fragmented = fwd_idx < bwd_idx;
        }
        it--;
    }
}

int main() {

    std::ifstream my_file;
    my_file.open("input.txt");

    std::string disk_map;

    std::string block_rep;
    std::vector<int> file_sizes; // vector indices represent file ID numbers, elements are file sizes

    while(my_file.is_open()) {
        getline(my_file, disk_map);

        block_rep = convertDiskMapToBlockForm(disk_map, file_sizes);

        std::cout << block_rep << std::endl;

        //std::cout << block_rep.size() << std::endl;

        defragment(file_sizes, block_rep);

        std::cout << computeChecksum(block_rep) << std::endl;

        my_file.close();
    }
    return 0;
}